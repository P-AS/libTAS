/*
    Copyright 2015-2026 Clément Gallet <clement.gallet@ens-lyon.org>

    This file is part of libTAS.

    libTAS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libTAS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libTAS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "sdlaudio.h"

#include "sdl/sdldynapi.h"
#include "sdl/sdlversion.h"
#include "checkpoint/ThreadManager.h" // getThreadId()
#include "logging.h"
#include "hook.h"
#include "global.h"
#include "audio/AudioContext.h"
#include "audio/AudioSource.h"
#include "audio/AudioBuffer.h"

#include <cstring> // strncpy
#include <mutex>
#include <vector>
#include <algorithm>
#include <map>

namespace libtas {

static const char* dummy_sdl_driver_name = "libtas";

struct SdlDevice
{
    int id;
    SDL_AudioFormat format;
    int channels;
    int freq;
};

/* Keep a copy of the SDL_AudioFormat used to open the audio device, because
 * SDL_MixAudio uses the current audio format, but as we actually don't open the
 * audio device, we call instead SDL_MixAudioFormat with the saved audio format.
 */
static SDL_AudioFormat audioFormat;
// static Uint16 bufferSamplesSize;

/* All SDL devices. From SDL implementation, there is a limit of 16 devices */
#define MAX_SDL_DEVICES 16
static SdlDevice sdl_devices[MAX_SDL_DEVICES];
static std::vector<AudioSource*> sdl_device_sources[MAX_SDL_DEVICES];

static const char* dummy_sdl_device_name = "libTAS device";
static std::string curDriver;
static std::recursive_mutex mutex;

/* Override */ int SDL_GetNumAudioDrivers(void)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    return 1;
}

/* Override */ const char *SDL_GetAudioDriver(int index)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if (index == 0)
        return dummy_sdl_driver_name;
    return NULL;
}

/* Override */ sdl3::SDL_AudioDeviceID * SDL_GetAudioPlaybackDevices(int *count)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if (count)
        *count = 1;

    sdl3::SDL_AudioDeviceID *ids = static_cast<sdl3::SDL_AudioDeviceID*>(ORIG_SDL3_CALL(SDL_malloc, (2 * sizeof(sdl3::SDL_AudioDeviceID))));
    ids[0] = 1;
    ids[1] = 0; // null-terminated list of devices, as per SDL documentation
    return ids;
}

char * SDL_AudioDriverName(char *namebuf, int maxlen)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    strncpy(namebuf, dummy_sdl_driver_name, maxlen);
    /* I don't know if we are in charge of returning a
     * null-terminated string
     */
    if (maxlen > 0)
        namebuf[maxlen-1] = '\0';

    return namebuf;
}

/* Override */ int SDL_AudioInit(const char *driver_name)
{
    if (driver_name == NULL) {
        LOGTRACE(LCF_SDL | LCF_SOUND, "Init SDL Audio with default driver");
        curDriver = dummy_sdl_device_name;
    }
    else {
        LOGTRACE(LCF_SDL | LCF_SOUND, "Init SDL Audio with driver %s", driver_name);
        curDriver = driver_name;
    }
    return 0;
}

/* Override */ void SDL_AudioQuit(void)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
}

/* Override */ const char *SDL_GetCurrentAudioDriver(void)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if (! curDriver.empty()) {
        return curDriver.c_str();
    }
    return nullptr;
}

/* Helper function to convert SDL audio format to internal format */
static AudioBuffer::SampleFormat sdlFormatToFormat(SDL_AudioFormat sdlFormat)
{
    switch(sdlFormat) {
        case SDL_AUDIO_U8:
            return AudioBuffer::SAMPLE_FMT_U8;
        case SDL_AUDIO_S16:
            return AudioBuffer::SAMPLE_FMT_S16;
        case SDL_AUDIO_S32:
            return AudioBuffer::SAMPLE_FMT_S32;
        case SDL_AUDIO_F32:
            return AudioBuffer::SAMPLE_FMT_FLT;
        default:
            LOG(LL_DEBUG, LCF_SDL | LCF_SOUND, "Unsupported audio format: %x", sdlFormat);
            return AudioBuffer::SAMPLE_FMT_UNKNOWN;
    }
}

/* Helper function for SDL_OpenAudio() and SDL_OpenAudioDevice() */
static int open_audio_device(const sdl2::SDL_AudioSpec * desired, sdl2::SDL_AudioSpec * obtained, int min_id)
{
    sdl2::SDL_AudioSpec _obtained;

    if (Global::shared_config.audio_disabled)
        return -1;

    /* Look for the next available device. Id 1 is reserved for SDL_OpenAudio() device */
    int id = min_id;
    for (; id < MAX_SDL_DEVICES; id++)
        if (sdl_devices[id].id == 0)
            break;

    if (id >= MAX_SDL_DEVICES) {
        LOG(LL_DEBUG, LCF_SDL | LCF_SOUND, "   no available device");
        return -1;
    }

    sdl_devices[id].id = id;

    if (!obtained) {
        obtained = &_obtained;
    }

    memcpy(obtained, desired, sizeof(sdl2::SDL_AudioSpec));
    AudioContext& audiocontext = AudioContext::get();
    std::lock_guard<std::mutex> lock(audiocontext.mutex);

    /* Sanity checks done by SDL */
    if (desired->freq == 0) obtained->freq = 22050;

    LOG(LL_DEBUG, LCF_SDL | LCF_SOUND, "Frequency %d Hz", obtained->freq);

    if (desired->format == 0) obtained->format = SDL_AUDIO_S16;

    if (desired->samples == 0) {
		/* Pick a default of ~46 ms at desired frequency */
		int samples = (desired->freq / 1000) * 46;
		int power2 = 1;
		while (power2 < samples) {
			power2 *= 2;
		}
		obtained->samples = power2;
	}

    /* Sanity check done by SDL */
    if (desired->channels == 0) obtained->channels = 2;

    LOG(LL_DEBUG, LCF_SDL | LCF_SOUND, "Channels %d", obtained->channels);

    /* Store the specs into the sdl device*/
    sdl_devices[id].channels = obtained->channels;
    sdl_devices[id].format = static_cast<SDL_AudioFormat>(obtained->format);
    sdl_devices[id].freq = obtained->freq;

    /* Create a source to hold the buffers */
    std::shared_ptr<AudioSource> sdl_source = audiocontext.createSource();

    sdl_source->channels = obtained->channels;
    sdl_source->format = sdlFormatToFormat(static_cast<SDL_AudioFormat>(obtained->format));
    sdl_source->frequency = obtained->freq;

    /* If some audio output parameters are set to auto, fill them with these values */
    audiocontext.initValues(sdl_source->format, sdl_source->channels, sdl_source->frequency);

    sdl_device_sources[id].push_back(sdl_source.get());

    if (sdl_source->format == AudioBuffer::SAMPLE_FMT_UNKNOWN) {
        LOG(LL_DEBUG, LCF_SDL | LCF_SOUND, "Unsupported audio format");
        return -1;
    }

    if (obtained->callback != NULL) {
        /* We are using the callback mechanism.
         * When the buffer is depleted, we are supposed to call the
         * callback function so that it fills the buffer again.
         * We will use a single AudioSource and a single AudioBuffer for this.
         * When the source plays the buffer until the end,
         * it calls the callback function to fill the buffer again.
         */

        auto buffer = audiocontext.createBuffer();
        if (!buffer) {
            LOG(LL_WARN, LCF_SDL | LCF_SOUND, "No more audio buffers available");
            return -1;
        }

        buffer->frequency = sdl_source->frequency;
        buffer->channels = sdl_source->channels;
        buffer->format = sdl_source->format;

        buffer->update();
        buffer->size = obtained->samples * buffer->alignSize;
        buffer->update(); // Yes, a second time, to fill sampleSize based on size.
        buffer->samples.resize(buffer->size);

        sdl_source->source = AudioSource::SOURCE_CALLBACK;
        sdl2::SDL_AudioCallback audioCallback = obtained->callback;
        void* callbackArg = obtained->userdata;
        sdl_source->callback = [audioCallback, callbackArg](AudioBuffer* ab){
            /* Emptying the audio buffer */
            ab->makeSilent();

            mutex.lock();
            audioCallback(callbackArg, ab->samples.data(), ab->size);
            mutex.unlock();
        };
        
        /* We simulate an empty buffer by setting the position at the end */
        sdl_source->position = buffer->sampleSize;

        sdl_source->queueBuffer(buffer);

        /* Specify the thread that opens the audio, to optimize Lock/UnlockAudio
         * and match its implementation */
        audiocontext.audio_thread = ThreadManager::getThreadId();
    }
    else {
        /* We are using the push mechanism. The game will use
         * SDL_QueueAudio() to push audio buffers.
         */
        sdl_source->source = AudioSource::SOURCE_STREAMING_CONTINUOUS;
    }

    /* We must fill some information in the structs */
    obtained->size = obtained->samples * obtained->channels * (obtained->format & 0xFF) / 8;

    /* Filling silence value. Not sure what to put here */
    obtained->silence = (obtained->format==SDL_AUDIO_U8)?0x80:0x00;

    /* Keep a copy of the audio format */
    audioFormat = static_cast<SDL_AudioFormat>(obtained->format);

    return id;
}

/* Override */ int SDL_OpenAudio(sdl2::SDL_AudioSpec * desired, sdl2::SDL_AudioSpec * obtained)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);

    /* SDL1 has only one device which is id 1 */    
    if (sdl_devices[1].id != 0) {
        LOG(LL_DEBUG, LCF_SDL | LCF_SOUND, "   audio device already opened");
        return -1;
    }

    sdl2::SDL_AudioDeviceID id = 0;
    
    if (obtained) {
        id = open_audio_device(desired, obtained, 1);
    }
    else {
        sdl2::SDL_AudioSpec _obtained = {};
        id = open_audio_device(desired, &_obtained, 1);
        /* On successful open, copy calculated values into 'desired'. */
        if (id > 0) {
            desired->size = _obtained.size;
            desired->silence = _obtained.silence;
            desired->samples = _obtained.samples;
        }
    }
    
    return id==1?0:-1;
}

/* Override */ int SDL_GetNumAudioDevices(int iscapture)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if (iscapture != 0)
        return 0;
    return 1;
}

/* Override */ const char *SDL_GetAudioDeviceName(std::uintptr_t p1, std::uintptr_t p2)
{
    const std::uintptr_t storage[] = {p1, p2};
    return invoke_sdl2_or_sdl3_from_storage(&sdl2::SDL_GetAudioDeviceName, &sdl3::SDL_GetAudioDeviceName, storage);
}

/* Override */ const char *sdl2::SDL_GetAudioDeviceName(int index, int iscapture)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if (iscapture != 0)
        return NULL;
    if (index == 0)
        return dummy_sdl_device_name;
    return NULL;
}

/* Override */ const char * sdl3::SDL_GetAudioDeviceName(SDL_AudioDeviceID devid)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    return dummy_sdl_device_name;
}

/* Override */ int SDL_GetAudioDeviceSpec(int index, int iscapture, sdl2::SDL_AudioSpec *spec)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if (iscapture != 0)
        return -1;
    if (!spec)
        return -1;    
    if (index != 0)
        return -1;

    spec->freq = Global::shared_config.audio_frequency;
    switch (Global::shared_config.audio_bitdepth) {
        case 8:
            spec->format = SDL_AUDIO_U8;
            break;
        case 16:
            spec->format = SDL_AUDIO_S16;
            break;
    }
    spec->channels = Global::shared_config.audio_channels;

    return 0;
}

/* Override */ bool SDL_GetAudioDeviceFormat(sdl3::SDL_AudioDeviceID devid, sdl3::SDL_AudioSpec *spec, int *sample_frames)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if (devid >= MAX_SDL_DEVICES && devid != sdl3::SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK && devid != sdl3::SDL_AUDIO_DEVICE_DEFAULT_RECORDING)
        return false;
    if (!spec)
        return false;

    if (devid == sdl3::SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK || devid == sdl3::SDL_AUDIO_DEVICE_DEFAULT_RECORDING || sdl_devices[devid].id == 0){
        /* Ask for default or device not opened, fill with global values */
        spec->freq = Global::shared_config.audio_frequency;
        switch (Global::shared_config.audio_bitdepth) {
            case 8:
                spec->format = SDL_AUDIO_U8;
                break;
            case 16:
                spec->format = SDL_AUDIO_S16LE;
                break;
        }
        spec->channels = Global::shared_config.audio_channels;
    }
    else {
        spec->freq = sdl_devices[devid].freq;
        spec->format = sdl_devices[devid].format;
        spec->channels = sdl_devices[devid].channels;
    }

    if (sample_frames) {
        *sample_frames = 512;
        LOG(LL_DEBUG, LCF_SDL | LCF_SOUND, "Game wants to set the buffer size to %d frames", *sample_frames);
    }

    return true;
}

/* Override */ int SDL_GetDefaultAudioInfo(char **name, sdl2::SDL_AudioSpec *spec, int iscapture)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if (iscapture != 0)
        return -1;
    if (!spec)
        return -1;
    if (name)
        *name = nullptr;

    spec->freq = Global::shared_config.audio_frequency;
    switch (Global::shared_config.audio_bitdepth) {
        case 8:
            spec->format = SDL_AUDIO_U8;
            break;
        case 16:
            spec->format = SDL_AUDIO_S16;
            break;
    }
    spec->channels = Global::shared_config.audio_channels;

    return 0;
}

/* Override */ int SDL_OpenAudioDevice(std::uintptr_t p1, std::uintptr_t p2, std::uintptr_t p3,
                                             std::uintptr_t p4, std::uintptr_t p5)
{
    const std::uintptr_t storage[] = {p1, p2, p3, p4, p5};
    return invoke_sdl2_or_sdl3_from_storage(&sdl2::SDL_OpenAudioDevice, &sdl3::SDL_OpenAudioDevice, storage);
}

/* Override */ sdl2::SDL_AudioDeviceID sdl2::SDL_OpenAudioDevice(const char *device,
                   int iscapture, const sdl2::SDL_AudioSpec *desired,
                   sdl2::SDL_AudioSpec *obtained, int allowed_changes)
{
    LOGTRACE(LCF_SDL | LCF_SOUND, "%s called for device %s", __func__, device?device:"NULL");
    if (iscapture != 0)
        return 0;

    /* Negative on failure */
    int id = open_audio_device(desired, obtained, 2);
    return (id > 0)? id : 0;
}

/* Override */ sdl3::SDL_AudioDeviceID sdl3::SDL_OpenAudioDevice(SDL_AudioDeviceID devid, const SDL_AudioSpec *spec)
{
    LOGTRACE(LCF_SDL | LCF_SOUND, "%s called for device %d", __func__, devid);
    if (devid == 0)
        return 0;

    if (devid == SDL_AUDIO_DEVICE_DEFAULT_RECORDING)
        return 0;

    if (devid == SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK)
        devid = 2;

    sdl2::SDL_AudioSpec specs_sdl2 = {};
    // memset(&specs_sdl2, 0, sizeof(sdl2::SDL_AudioSpec));

    if (spec) {
        specs_sdl2.freq = spec->freq;
        specs_sdl2.format = static_cast<SDL_AudioFormat>(spec->format);
        specs_sdl2.channels = spec->channels;

        /* Streams convert to the device format, so the device only needs a
         * format that we can mix */
        if (sdlFormatToFormat(static_cast<SDL_AudioFormat>(specs_sdl2.format)) == AudioBuffer::SAMPLE_FMT_UNKNOWN)
            specs_sdl2.format = SDL_AUDIO_F32;
    }
    else {
        specs_sdl2.freq = Global::shared_config.audio_frequency;
        switch (Global::shared_config.audio_bitdepth) {
            case 8:
                specs_sdl2.format = SDL_AUDIO_U8;
                break;
            case 16:
                specs_sdl2.format = SDL_AUDIO_S16LE;
                break;
        }
        specs_sdl2.channels = Global::shared_config.audio_channels;
    }

    /* Negative on failure */
    int id = open_audio_device(&specs_sdl2, nullptr, devid);

    return (id > 0)? id : 0;
}

/* Override */ sdl2::SDL_AudioStatus SDL_GetAudioStatus(void)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    return SDL_GetAudioDeviceStatus(1);
}

/* Override */ sdl2::SDL_AudioStatus SDL_GetAudioDeviceStatus(sdl2::SDL_AudioDeviceID dev)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);

    if ((dev <= 1) || (dev > MAX_SDL_DEVICES) || (sdl_devices[dev].id == 0))
        return sdl2::SDL_AUDIO_STOPPED;

    if (sdl_device_sources[dev].empty()) {
        LOG(LL_WARN, LCF_SDL | LCF_SOUND, "No source associated to SDL device");
        return sdl2::SDL_AUDIO_STOPPED;
    }

    auto& source = sdl_device_sources[dev][0];
    switch(source->state) {
        case AudioSource::SOURCE_INITIAL:
        case AudioSource::SOURCE_STOPPED:
            return sdl2::SDL_AUDIO_STOPPED;
        case AudioSource::SOURCE_PLAYING:
            return sdl2::SDL_AUDIO_PLAYING;
        case AudioSource::SOURCE_PAUSED:
            return sdl2::SDL_AUDIO_PAUSED;
        default:
            LOG(LL_FATAL, LCF_SDL | LCF_SOUND, "Unknown source state");
            return sdl2::SDL_AUDIO_STOPPED;
    }
}

/* Override */ void SDL_PauseAudio(int pause_on)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    sdl2::SDL_PauseAudioDevice(1, pause_on);
}

/* Override */ bool SDL_PauseAudioDevice(std::uintptr_t p1, std::uintptr_t p2)
{
    const std::uintptr_t storage[] = {p1, p2};
    return invoke_sdl2_or_sdl3_from_storage(&sdl2::SDL_PauseAudioDevice, &sdl3::SDL_PauseAudioDevice, storage);
}

/* Override */ void sdl2::SDL_PauseAudioDevice(sdl2::SDL_AudioDeviceID dev, int pause_on)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if ((dev <= 1) || (dev > MAX_SDL_DEVICES) || (sdl_devices[dev].id == 0))
        return;

    std::lock_guard<std::mutex> lock(AudioContext::get().mutex);
    for (auto& sdl_source : sdl_device_sources[dev]) {
        sdl_source->state = pause_on ? AudioSource::SOURCE_PAUSED : AudioSource::SOURCE_PLAYING;
    }
}

bool sdl3::SDL_PauseAudioDevice(SDL_AudioDeviceID devid)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if ((devid <= 1) || (devid > MAX_SDL_DEVICES) || (sdl_devices[devid].id == 0))
        return false;

    std::lock_guard<std::mutex> lock(AudioContext::get().mutex);
    for (auto& sdl_source : sdl_device_sources[devid]) {
        sdl_source->state = AudioSource::SOURCE_PAUSED;
    }
    return true;
}

bool SDL_ResumeAudioDevice(sdl3::SDL_AudioDeviceID devid)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if ((devid <= 1) || (devid > MAX_SDL_DEVICES) || (sdl_devices[devid].id == 0))
        return false;

    std::lock_guard<std::mutex> lock(AudioContext::get().mutex);
    for (auto& sdl_source : sdl_device_sources[devid]) {
        sdl_source->state = AudioSource::SOURCE_PLAYING;
    }
    return true;
}

bool SDL_AudioDevicePaused(sdl3::SDL_AudioDeviceID devid)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if ((devid <= 1) || (devid > MAX_SDL_DEVICES) || (sdl_devices[devid].id == 0))
        return false;

    std::lock_guard<std::mutex> lock(AudioContext::get().mutex);
    if (sdl_device_sources[devid].empty()) {
        return false;
    }
    return sdl_device_sources[devid][0]->state == AudioSource::SOURCE_PAUSED;
}

bool SDL_MixAudio(Uint8 * dst, const Uint8 * src, Uint32 p3, int p4, float sdl3_volume)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);

    int SDLver = get_sdlversion();

    if (SDLver == 3)
        return ORIG_SDL3_CALL(SDL_MixAudio, (dst, src, static_cast<SDL_AudioFormat>(p3), static_cast<Uint32>(p4), sdl3_volume));

    Uint32 len = p3;
    int volume = p4;

    if (SDLver == 2) {
        /* SDL_MixAudio uses the opened audio device, but we don't open any,
         * so we use SDL_MixAudioFormat which don't required a device to be opened
         * and instead let us specify the audio format.
         */

        ORIG_SDL2_CALL(SDL_MixAudioFormat, (dst, src, audioFormat, len, volume));
        return true;
    }

    ORIG_SDL2_CALL(SDL_MixAudio, (dst, src, len, volume));
    return true;
}

/* Override */ float SDL_GetAudioDeviceGain(sdl3::SDL_AudioDeviceID devid)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if ((devid <= 1) || (devid > MAX_SDL_DEVICES) || (sdl_devices[devid].id == 0))
        return -1.0f;

    /* We don't support per-device gain. Returning the global audio context gain.*/
    std::lock_guard<std::mutex> lock(AudioContext::get().mutex);
    AudioContext& audiocontext = AudioContext::get();
    return audiocontext.volume;
}

/* Override */ bool SDL_SetAudioDeviceGain(sdl3::SDL_AudioDeviceID devid, float gain)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if ((devid <= 1) || (devid > MAX_SDL_DEVICES) || (sdl_devices[devid].id == 0))
        return false;

    /* We don't support per-device gain. Setting the global audio context gain.*/
    std::lock_guard<std::mutex> lock(AudioContext::get().mutex);
    AudioContext& audiocontext = AudioContext::get();
    audiocontext.volume = gain;
    return true;
}

/* Override */ int SDL_QueueAudio(sdl2::SDL_AudioDeviceID dev, const void *data, Uint32 len)
{
    LOGTRACE(LCF_SDL | LCF_SOUND, "%s call with %d bytes of data", __func__, len);

    if ((dev <= 1) || (dev > MAX_SDL_DEVICES) || (sdl_devices[dev].id == 0))
        return -1;

    if (sdl_device_sources[dev].empty())
        return -1;

    auto& sdl_source = sdl_device_sources[dev][0];
    if (sdl_source->source == AudioSource::SOURCE_CALLBACK) {
        /* We cannot queue samples when using the callback mechanism */
        return -1;
    }

    AudioContext& audiocontext = AudioContext::get();
    std::lock_guard<std::mutex> lock(audiocontext.mutex);

    /* We try to reuse a buffer that has been processed from the source */
    std::shared_ptr<AudioBuffer> ab = audiocontext.reuseBufferFromSourceOrCreate(sdl_source);
    if (!ab) {
        /* No more audio buffers, they are only processed at frame boundaries */
        LOG(LL_WARN, LCF_SDL | LCF_SOUND, "    No more audio buffers available");
        return -1;
    }

    /* Filling buffer */
    ab->samples.clear();
    ab->samples.insert(ab->samples.end(), static_cast<const uint8_t*>(data), &(static_cast<const uint8_t*>(data))[len]);
    ab->size = len;
    ab->update();
    sdl_source->queueBuffer(ab);

    /* If an underrun occurred, resume the playback */
    if (sdl_source->state == AudioSource::SOURCE_UNDERRUN)
        sdl_source->state = AudioSource::SOURCE_PLAYING;

    return 0;
}

/* Override */ Uint32 SDL_GetQueuedAudioSize(sdl2::SDL_AudioDeviceID dev)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);

    if ((dev <= 1) || (dev > MAX_SDL_DEVICES) || (sdl_devices[dev].id == 0))
        return -1;

    if (sdl_device_sources[dev].empty())
        return -1;

    auto& sdl_source = sdl_device_sources[dev][0];
    if (sdl_source->source == AudioSource::SOURCE_CALLBACK) {
        /* We cannot get queue samples when using the callback mechanism */
        return 0;
    }

    std::lock_guard<std::mutex> lock(AudioContext::get().mutex);
    Uint32 qsize = sdl_source->queueSize() - sdl_source->getPosition();
    LOG(LL_DEBUG, LCF_SDL | LCF_SOUND, "Returning %d", qsize);
    return qsize;
}

/* Override */ void SDL_ClearQueuedAudio(sdl2::SDL_AudioDeviceID dev)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);

    if ((dev <= 1) || (dev > MAX_SDL_DEVICES) || (sdl_devices[dev].id == 0))
        return;

    if (sdl_device_sources[dev].empty())
        return;

    auto& sdl_source = sdl_device_sources[dev][0];
    if (sdl_source->source == AudioSource::SOURCE_CALLBACK) {
        /* We cannot get queue samples when using the callback mechanism */
        return;
    }

    std::lock_guard<std::mutex> lock(AudioContext::get().mutex);
    /* We simulate clearing the queue by setting the position to the end
     * of the queue.
     */
    sdl_source->setPosition(sdl_source->queueSize());
}

/* SDL3 audio streams
 *
 * An SDL3 audio stream only converts audio: the game puts samples in one
 * format and gets them in another, and the stream never accesses an audio
 * device by itself. So the game uses real SDL streams, with all their
 * functions, and we only emulate the devices. When a stream is bound to one of
 * our devices, an audio source pulls the converted samples from the stream
 * when we mix the audio at the frame boundary. Getting these samples calls the
 * get-callback of the stream when it runs short, like the audio thread of SDL
 * does, but at a deterministic time. */

struct SdlBoundStream
{
    int device;
    AudioSource* source;
};

/* Streams that are bound to one of our devices */
static std::map<SDL_AudioStream*, SdlBoundStream> sdl3_bound_streams;

/* Streams created by SDL_OpenAudioDeviceStream(), with the device that they
 * opened, which is closed when the stream is destroyed */
static std::map<SDL_AudioStream*, int> sdl3_device_streams;

/* Number of sample frames that SDL asks from the streams of a device at once */
static int sdl3SampleFrames(int freq)
{
    if (freq <= 22050)
        return 512;
    if (freq <= 48000)
        return 1024;
    if (freq <= 96000)
        return 2048;
    return 4096;
}

static bool bindStream(int devid, SDL_AudioStream *stream)
{
    std::lock_guard<std::recursive_mutex> lock(mutex);

    if (sdl3_bound_streams.count(stream)) {
        LOG(LL_DEBUG, LCF_SDL | LCF_SOUND, "   stream already bound");
        return false;
    }

    const SdlDevice& device = sdl_devices[devid];

    /* The device sets the output format of the stream */
    sdl3::SDL_AudioSpec dst_spec;
    dst_spec.format = static_cast<SDL_AudioFormat>(device.format);
    dst_spec.channels = device.channels;
    dst_spec.freq = device.freq;
    bool ret;
    NATIVECALL(ret = ORIG_SDL3_CALL(SDL_SetAudioStreamFormat, (stream, nullptr, &dst_spec)));
    if (!ret)
        return false;

    AudioContext& audiocontext = AudioContext::get();
    std::lock_guard<std::mutex> lock_context(audiocontext.mutex);

    auto buffer = audiocontext.createBuffer();
    if (!buffer) {
        LOG(LL_WARN, LCF_SDL | LCF_SOUND, "No more audio buffers available");
        return false;
    }

    std::shared_ptr<AudioSource> source = audiocontext.createSource();
    source->channels = device.channels;
    source->format = sdlFormatToFormat(device.format);
    source->frequency = device.freq;

    buffer->frequency = source->frequency;
    buffer->channels = source->channels;
    buffer->format = source->format;
    buffer->update();
    buffer->size = sdl3SampleFrames(device.freq) * buffer->alignSize;
    buffer->update(); // Yes, a second time, to fill sampleSize based on size.
    buffer->samples.resize(buffer->size);

    source->source = AudioSource::SOURCE_CALLBACK;
    source->callback = [stream](AudioBuffer* ab) {
        /* What the stream does not have is played as silence */
        ab->makeSilent();

        /* The get-callback of the stream may call our audio functions */
        AudioContext::get().mutex.unlock();
        ORIG_SDL3_CALL(SDL_GetAudioStreamData, (stream, ab->samples.data(), ab->size));
        AudioContext::get().mutex.lock();
    };

    /* We simulate an empty buffer by setting the position at the end */
    source->position = buffer->sampleSize;
    source->queueBuffer(buffer);

    /* The stream follows the paused state of the device */
    bool paused = !sdl_device_sources[devid].empty() &&
        (sdl_device_sources[devid][0]->state == AudioSource::SOURCE_PAUSED);
    source->state = paused ? AudioSource::SOURCE_PAUSED : AudioSource::SOURCE_PLAYING;

    sdl_device_sources[devid].push_back(source.get());
    sdl3_bound_streams[stream] = {devid, source.get()};
    return true;
}

static void unbindStream(SDL_AudioStream *stream)
{
    std::lock_guard<std::recursive_mutex> lock(mutex);

    auto it = sdl3_bound_streams.find(stream);
    if (it == sdl3_bound_streams.end())
        return;

    AudioContext& audiocontext = AudioContext::get();
    std::lock_guard<std::mutex> lock_context(audiocontext.mutex);

    AudioSource* source = it->second.source;
    auto& sources = sdl_device_sources[it->second.device];
    sources.erase(std::remove(sources.begin(), sources.end(), source), sources.end());
    audiocontext.deleteSource(source->id);

    sdl3_bound_streams.erase(it);
}

/* Get the device that a stream is bound to, or 0 */
static int getDeviceFromStream(SDL_AudioStream *stream)
{
    std::lock_guard<std::recursive_mutex> lock(mutex);
    auto it = sdl3_bound_streams.find(stream);
    if (it == sdl3_bound_streams.end())
        return 0;
    return it->second.device;
}

/* Override */ bool SDL_BindAudioStreams(sdl3::SDL_AudioDeviceID devid, SDL_AudioStream * const *streams, int num_streams)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);

    if ((devid <= 1) || (devid >= MAX_SDL_DEVICES) || (sdl_devices[devid].id == 0))
        return false;

    for (int i = 0; i < num_streams; i++) {
        if (!streams[i] || getDeviceFromStream(streams[i]))
            return false;
    }

    for (int i = 0; i < num_streams; i++) {
        if (!bindStream(devid, streams[i])) {
            /* SDL binds all the streams or none */
            for (int j = 0; j < i; j++)
                unbindStream(streams[j]);
            return false;
        }
    }

    return true;
}

/* Override */ bool SDL_BindAudioStream(sdl3::SDL_AudioDeviceID devid, SDL_AudioStream *stream)
{
    return SDL_BindAudioStreams(devid, &stream, 1);
}

/* Override */ void SDL_UnbindAudioStreams(SDL_AudioStream * const *streams, int num_streams)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);

    if (!streams)
        return;

    for (int i = 0; i < num_streams; i++) {
        if (streams[i])
            unbindStream(streams[i]);
    }
}

/* Override */ void SDL_UnbindAudioStream(SDL_AudioStream *stream)
{
    SDL_UnbindAudioStreams(&stream, 1);
}

/* Override */ sdl3::SDL_AudioDeviceID SDL_GetAudioStreamDevice(SDL_AudioStream *stream)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);

    if (!stream)
        return 0;

    return getDeviceFromStream(stream);
}

/* Override */ bool SDL_SetAudioStreamFormat(SDL_AudioStream *stream, const sdl3::SDL_AudioSpec *src_spec, const sdl3::SDL_AudioSpec *dst_spec)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);

    /* The output format of a stream bound to a device is the device format */
    if (getDeviceFromStream(stream))
        dst_spec = nullptr;

    return ORIG_SDL3_CALL(SDL_SetAudioStreamFormat, (stream, src_spec, dst_spec));
}

static bool setStreamDevicePaused(SDL_AudioStream *stream, bool paused)
{
    int stream_device = stream ? getDeviceFromStream(stream) : 0;
    if (stream_device == 0)
        return false;

    std::lock_guard<std::mutex> lock(AudioContext::get().mutex);
    for (auto& sdl_source : sdl_device_sources[stream_device]) {
        sdl_source->state = paused ? AudioSource::SOURCE_PAUSED : AudioSource::SOURCE_PLAYING;
    }
    return true;
}

/* Override */ bool SDL_PauseAudioStreamDevice(SDL_AudioStream *stream)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    return setStreamDevicePaused(stream, true);
}

/* Override */ bool SDL_ResumeAudioStreamDevice(SDL_AudioStream *stream)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    return setStreamDevicePaused(stream, false);
}

/* Override */ bool SDL_AudioStreamDevicePaused(SDL_AudioStream *stream)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);

    std::lock_guard<std::recursive_mutex> lock(mutex);
    auto it = sdl3_bound_streams.find(stream);
    if (it == sdl3_bound_streams.end())
        return false;

    return it->second.source->state == AudioSource::SOURCE_PAUSED;
}

/* Override */ void SDL_LockAudio(void)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    return SDL_LockAudioDevice(1);
}

/* Override */ void SDL_LockAudioDevice(sdl2::SDL_AudioDeviceID dev)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if (ThreadManager::getThreadId() == AudioContext::get().audio_thread)
        return;
    mutex.lock();
}


/* Override */ void SDL_UnlockAudio(void)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    return SDL_UnlockAudioDevice(1);
}

/* Override */ void SDL_UnlockAudioDevice(sdl2::SDL_AudioDeviceID dev)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    if (ThreadManager::getThreadId() == AudioContext::get().audio_thread)
        return;
    mutex.unlock();
}


/* Override */ void SDL_DestroyAudioStream(SDL_AudioStream *stream)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);

    if (!stream)
        return;

    unbindStream(stream);

    /* A stream created with SDL_OpenAudioDeviceStream() closes its device */
    int device = 0;
    {
        std::lock_guard<std::recursive_mutex> lock(mutex);
        auto it = sdl3_device_streams.find(stream);
        if (it != sdl3_device_streams.end()) {
            device = it->second;
            sdl3_device_streams.erase(it);
        }
    }
    if (device)
        SDL_CloseAudioDevice(device);

    ORIG_SDL3_CALL(SDL_DestroyAudioStream, (stream));
}

/* Override */ SDL_AudioStream * SDL_OpenAudioDeviceStream(sdl3::SDL_AudioDeviceID devid, const sdl3::SDL_AudioSpec *spec, SDL_AudioStreamCallback callback, void *userdata)
{
    LOGTRACE(LCF_SDL | LCF_SOUND, "%s called for device %d", __func__, devid);
    if (devid == 0)
        return nullptr;

    if (devid == sdl3::SDL_AUDIO_DEVICE_DEFAULT_RECORDING)
        return nullptr;

    if (devid == sdl3::SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK)
        devid = 2;

    sdl2::SDL_AudioSpec specs_sdl2 = {};

    if (spec) {
        specs_sdl2.freq = spec->freq;
        specs_sdl2.format = static_cast<SDL_AudioFormat>(spec->format);
        specs_sdl2.channels = spec->channels;

        /* The stream converts from any format, so the device only needs a
         * format that we can mix */
        if (sdlFormatToFormat(static_cast<SDL_AudioFormat>(specs_sdl2.format)) == AudioBuffer::SAMPLE_FMT_UNKNOWN)
            specs_sdl2.format = SDL_AUDIO_F32;
    }
    else {
        specs_sdl2.freq = Global::shared_config.audio_frequency;
        switch (Global::shared_config.audio_bitdepth) {
            case 8:
                specs_sdl2.format = SDL_AUDIO_U8;
                break;
            case 16:
            default:
                specs_sdl2.format = SDL_AUDIO_S16LE;
                break;
        }
        specs_sdl2.channels = Global::shared_config.audio_channels;
    }

    sdl2::SDL_AudioSpec _obtained = {};
    /* Negative on failure */
    int id = open_audio_device(&specs_sdl2, &_obtained, devid);

    if (id <= 0)
        return nullptr;

    /* The device of SDL_OpenAudioDeviceStream() starts paused */
    {
        std::lock_guard<std::mutex> lock(AudioContext::get().mutex);
        for (auto& sdl_source : sdl_device_sources[id])
            sdl_source->state = AudioSource::SOURCE_PAUSED;
    }

    /* Without a spec, the stream takes the format of the device */
    sdl3::SDL_AudioSpec src_spec;
    if (spec) {
        src_spec = *spec;
    }
    else {
        src_spec.format = static_cast<SDL_AudioFormat>(_obtained.format);
        src_spec.channels = _obtained.channels;
        src_spec.freq = _obtained.freq;
    }

    SDL_AudioStream *stream;
    NATIVECALL(stream = ORIG_SDL3_CALL(SDL_CreateAudioStream, (&src_spec, nullptr)));
    if (!stream) {
        SDL_CloseAudioDevice(id);
        return nullptr;
    }

    if (!bindStream(id, stream)) {
        ORIG_SDL3_CALL(SDL_DestroyAudioStream, (stream));
        SDL_CloseAudioDevice(id);
        return nullptr;
    }

    {
        std::lock_guard<std::recursive_mutex> lock(mutex);
        sdl3_device_streams[stream] = id;
    }

    if (callback)
        ORIG_SDL3_CALL(SDL_SetAudioStreamGetCallback, (stream, callback, userdata));

    return stream;
}

/* Override */ void SDL_CloseAudio(void)
{
    LOGTRACE_SIMPLE(LCF_SDL | LCF_SOUND);
    return SDL_CloseAudioDevice(1);
}

/* Override */ void SDL_CloseAudioDevice(sdl2::SDL_AudioDeviceID dev)
{
    LOGTRACE(LCF_SDL | LCF_SOUND, "%s called with dev %d", __func__, dev);

    if (dev <= 1 || dev >= MAX_SDL_DEVICES)
        return;

    /* Closing a device unbinds its SDL3 streams. Their sources are deleted
     * below with the other sources of the device. The two mutexes are not
     * held together, because SDL2 callbacks lock our mutex while the audio
     * context is locked. */
    {
        std::lock_guard<std::recursive_mutex> lock_streams(mutex);
        for (auto it = sdl3_bound_streams.begin(); it != sdl3_bound_streams.end();) {
            if (it->second.device == static_cast<int>(dev))
                it = sdl3_bound_streams.erase(it);
            else
                ++it;
        }
    }

    AudioContext& audiocontext = AudioContext::get();
    std::lock_guard<std::mutex> lock(audiocontext.mutex);

    if (sdl_devices[dev].id != 0) {

        /* Remove the source from the audio context */
        for (auto& sdl_source : sdl_device_sources[dev]) {
            audiocontext.deleteSource(sdl_source->id);
        }
        sdl_device_sources[dev].clear();
        sdl_devices[dev].id = 0;
    }
}

}
