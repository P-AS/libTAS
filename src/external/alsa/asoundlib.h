/*
 * Stub header for alsa/asoundlib.h - macOS compatibility layer
 * Provides stub ALSA types and macros
 */

#ifndef _ALSA_ASOUNDLIB_H
#define _ALSA_ASOUNDLIB_H

#include <stdint.h>
#include <stddef.h>
#include <poll.h>

/* ALSA error codes */
#define SND_PCM_STATE_OPEN 0
#define SND_PCM_STATE_SETUP 1
#define SND_PCM_STATE_PREPARED 2
#define SND_PCM_STATE_RUNNING 3
#define SND_PCM_STATE_XRUN 4
#define SND_PCM_STATE_DRAINING 5
#define SND_PCM_STATE_PAUSED 6
#define SND_PCM_STATE_SUSPENDED 7
#define SND_PCM_STATE_DISCONNECTED 8

/* ALSA PCM types */
typedef int snd_pcm_type_t;
#define SND_PCM_TYPE_PLUG 0
#define SND_PCM_TYPE_SHM 1
#define SND_PCM_TYPE_NETWORK 2
#define SND_PCM_TYPE_IEC958 3
#define SND_PCM_TYPE_HW 4
#define SND_PCM_TYPE_DUMYMIX 5
#define SND_PCM_TYPE_MULTI 6
#define SND_PCM_TYPE_ROUTE 7
#define SND_PCM_TYPE_SHAREDMEM 8
#define SND_PCM_TYPE_LINEAR 9
#define SND_PCM_TYPE_FLOAT 10
#define SND_PCM_TYPE_INTEGER 11
#define SND_PCM_TYPE_SIGNED 12
#define SND_PCM_TYPE_UNSIGNED 13
#define SND_PCM_TYPE_IMA_ADPCM 14
#define SND_PCM_TYPE_MPEG 15
#define SND_PCM_TYPE_GSM 16
#define SND_PCM_TYPE_SPECIAL 17
#define SND_PCM_TYPE_SOFTVOL 18
#define SND_PCM_TYPE_DISCOVER 19
#define SND_PCM_TYPE_MODULE 20
#define SND_PCM_TYPE_HOOK 21
#define SND_PCM_TYPE_TBA 22
#define SND_PCM_TYPE_BYTE 23
#define SND_PCM_TYPE_MMAP 24
#define SND_PCM_TYPE_EXTEND 25

/* PCM stream */
typedef int snd_pcm_stream_t;
#define SND_PCM_STREAM_PLAYBACK 0
#define SND_PCM_STREAM_CAPTURE 1

/* PCM state */
typedef int snd_pcm_state_t;
#define SND_PCM_STATE_OPEN 0
#define SND_PCM_STATE_SETUP 1
#define SND_PCM_STATE_PREPARED 2
#define SND_PCM_STATE_RUNNING 3
#define SND_PCM_STATE_XRUN 4
#define SND_PCM_STATE_DRAINING 5
#define SND_PCM_STATE_PAUSED 6
#define SND_PCM_STATE_SUSPENDED 7
#define SND_PCM_STATE_DISCONNECTED 8

/* PCM access type */
typedef int snd_pcm_access_t;
#define SND_PCM_ACCESS_NONE 0
#define SND_PCM_ACCESS_RW_INTERLEAVED 1
#define SND_PCM_ACCESS_RW_NONINTERLEAVED 2
#define SND_PCM_ACCESS_MMAP_INTERLEAVED 3
#define SND_PCM_ACCESS_MMAP_NONINTERLEAVED 4
#define SND_PCM_ACCESS_MMAP_COMPLEX 5

/* PCM format */
typedef int snd_pcm_format_t;
#define SND_PCM_FORMAT_UNKNOWN -1
#define SND_PCM_FORMAT_S8 0
#define SND_PCM_FORMAT_U8 1
#define SND_PCM_FORMAT_S16_LE 2
#define SND_PCM_FORMAT_S16_BE 3
#define SND_PCM_FORMAT_U16_LE 4
#define SND_PCM_FORMAT_U16_BE 5
#define SND_PCM_FORMAT_S24_LE 6
#define SND_PCM_FORMAT_S24_BE 7
#define SND_PCM_FORMAT_U24_LE 8
#define SND_PCM_FORMAT_U24_BE 9
#define SND_PCM_FORMAT_S32_LE 10
#define SND_PCM_FORMAT_S32_BE 11
#define SND_PCM_FORMAT_U32_LE 12
#define SND_PCM_FORMAT_U32_BE 13
#define SND_PCM_FORMAT_FLOAT_LE 14
#define SND_PCM_FORMAT_FLOAT_BE 15
#define SND_PCM_FORMAT_FLOAT64_LE 16
#define SND_PCM_FORMAT_FLOAT64_BE 17
#define SND_PCM_FORMAT_IEC958_SUBFRAME_LE 18
#define SND_PCM_FORMAT_IEC958_SUBFRAME_BE 19
#define SND_PCM_FORMAT_MU_LAW 20
#define SND_PCM_FORMAT_A_LAW 21
#define SND_PCM_FORMAT_IMA_ADPCM 22
#define SND_PCM_FORMAT_MPEG 23
#define SND_PCM_FORMAT_GSM 24
#define SND_PCM_FORMAT_SPECIAL 31
#define SND_PCM_FORMAT_S24_3BE 32
#define SND_PCM_FORMAT_U24_3BE 33
#define SND_PCM_FORMAT_S20_3BE 34
#define SND_PCM_FORMAT_U20_3BE 35
#define SND_PCM_FORMAT_S18_3BE 36
#define SND_PCM_FORMAT_U18_3BE 37
#define SND_PCM_FORMAT_G723_24 40
#define SND_PCM_FORMAT_G723_24_1B 41
#define SND_PCM_FORMAT_G723_40 42
#define SND_PCM_FORMAT_G723_40_1B 43
#define SND_PCM_FORMAT_DSD_U8 50
#define SND_PCM_FORMAT_DSD_U16_LE 51
#define SND_PCM_FORMAT_DSD_U32_LE 52
#define SND_PCM_FORMAT_DSD_U16_BE 53
#define SND_PCM_FORMAT_DSD_U32_BE 54

/* PCM sample position */
typedef long snd_pcm_sframes_t;
typedef unsigned int snd_pcm_uframes_t;

/* PCM channel map */
#define SND_CHMAP_UNKNOWN 0
#define SND_CHMAP_NA 1
#define SND_CHMAP_MONO 2
#define SND_CHMAP_FRONT_LEFT 3
#define SND_CHMAP_FRONT_RIGHT 4
#define SND_CHMAP_FRONT_CENTER 5
#define SND_CHMAP_REAR_CENTER 6
#define SND_CHMAP_REAR_LEFT 7
#define SND_CHMAP_REAR_RIGHT 8
#define SND_CHMAP_LFE 9
#define SND_CHMAP_FRONT_LEFT_CENTER 10
#define SND_CHMAP_FRONT_RIGHT_CENTER 11
#define SND_CHMAP_SIDE_LEFT 12
#define SND_CHMAP_SIDE_RIGHT 13
#define SND_CHMAP_AUX_0 14
#define SND_CHMAP_AUX_1 15

/* PCM channel map type */
typedef enum {
    SND_CHMAP_TYPE_NONE = 0,
    SND_CHMAP_TYPE_FIXED = 1,
    SND_CHMAP_TYPE_VARIED = 2
} snd_pcm_chmap_type_t;

/* PCM channel position */
typedef enum {
    SND_CHMAP_POSITION_UNKNOWN = 0,
    SND_CHMAP_POSITION_NA = 1,
    SND_CHMAP_POSITION_MONO = 2,
    SND_CHMAP_POSITION_FRONT_LEFT = 3,
    SND_CHMAP_POSITION_FRONT_RIGHT = 4,
    SND_CHMAP_POSITION_FRONT_CENTER = 5,
    SND_CHMAP_POSITION_REAR_CENTER = 6,
    SND_CHMAP_POSITION_REAR_LEFT = 7,
    SND_CHMAP_POSITION_REAR_RIGHT = 8,
    SND_CHMAP_POSITION_LFE = 9,
    SND_CHMAP_POSITION_FRONT_LEFT_CENTER = 10,
    SND_CHMAP_POSITION_FRONT_RIGHT_CENTER = 11,
    SND_CHMAP_POSITION_SIDE_LEFT = 12,
    SND_CHMAP_POSITION_SIDE_RIGHT = 13,
    SND_CHMAP_POSITION_AUX_0 = 14,
    SND_CHMAP_POSITION_AUX_1 = 15
} snd_pcm_chmap_position_t;

/* PCM channel map */
typedef struct {
    unsigned int channels;
    snd_pcm_chmap_position_t map[16];
} snd_pcm_chmap_t;

/* PCM channel map query */
typedef struct {
    snd_pcm_chmap_type_t type;
    snd_pcm_chmap_t map;
} snd_pcm_chmap_query_t;

/* PCM info */
typedef struct _snd_pcm_info snd_pcm_info_t;

/* PCM hw params */
typedef struct _snd_pcm_hw_params snd_pcm_hw_params_t;

/* PCM sw params */
typedef struct _snd_pcm_sw_params snd_pcm_sw_params_t;

/* PCM status */
typedef struct _snd_pcm_status snd_pcm_status_t;

/* PCM format mask */
typedef struct _snd_pcm_format_mask snd_pcm_format_mask_t;

/* PCM handle */
typedef struct _snd_pcm snd_pcm_t;

/* PCM async handler */
typedef struct _snd_async_handler snd_async_handler_t;

/* PCM config */
typedef struct snd_config snd_config_t;

/* PCM device info */
typedef struct _snd_device_info snd_device_info_t;

/* PCM ctl info */
typedef struct _snd_ctl_info snd_ctl_info_t;

/* PCM async callback */
typedef void (*snd_async_callback_t)(snd_async_handler_t *handler);

/* PCM tstamp mode */
typedef int snd_pcm_tstamp_t;
#define SND_PCM_TSTAMP_NONE 0
#define SND_PCM_TSTAMP_GETTIME 1
#define SND_PCM_TSTAMP_ENABLE 2

/* PCM tstamp type */
typedef int snd_pcm_tstamp_type_t;
#define SND_PCM_TSTAMP_TYPE_DEFAULT 0
#define SND_PCM_TSTAMP_TYPE_MONOTONIC 1
#define SND_PCM_TSTAMP_TYPE_MONOTONIC_RAW 2

/* PCM htstamp */
typedef struct {
    int tv_sec;
    int tv_nsec;
} snd_htimestamp_t;

/* PCM channel area */
typedef struct {
    unsigned int first;
    unsigned int step;
    void *addr;
} snd_pcm_channel_area_t;

/* Macros */
#define SND_PCM_ASYNC 0x0001
#define SND_PCM_NONBLOCK 0x0002
#define SND_PCM_ASYNC_CB 0x0004

/* External function declarations */
extern int snd_pcm_open(snd_pcm_t **pcm, const char *name, snd_pcm_stream_t stream, int mode);
extern int snd_pcm_close(snd_pcm_t *pcm);
extern snd_pcm_state_t snd_pcm_state(snd_pcm_t *pcm);
extern int snd_pcm_hw_params_any(snd_pcm_t *pcm, snd_pcm_hw_params_t *params);
extern size_t snd_pcm_hw_params_sizeof(void);
extern int snd_pcm_hw_params_malloc(snd_pcm_hw_params_t **ptr);
extern void snd_pcm_hw_params_free(snd_pcm_hw_params_t *obj);
extern int snd_pcm_hw_params_set_access(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, snd_pcm_access_t _access);
extern int snd_pcm_hw_params_set_format(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, snd_pcm_format_t val);
extern int snd_pcm_hw_params_set_channels(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, unsigned int val);
extern int snd_pcm_hw_params(snd_pcm_t *pcm, snd_pcm_hw_params_t *params);
extern int snd_pcm_hw_params_current(snd_pcm_t *pcm, snd_pcm_hw_params_t *params);
extern int snd_pcm_hw_params_test_format(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, snd_pcm_format_t val);
extern int snd_pcm_hw_params_test_channels(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, unsigned int val);
extern int snd_pcm_hw_params_set_rate(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, unsigned int val, int dir);
extern int snd_pcm_hw_params_set_rate_near(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, unsigned int *val, int *dir);
extern int snd_pcm_hw_params_set_period_size_near(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, snd_pcm_uframes_t *val, int *dir);
extern int snd_pcm_hw_params_set_buffer_size_near(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, snd_pcm_uframes_t *val);
extern int snd_pcm_hw_params_get_channels(const snd_pcm_hw_params_t *params, unsigned int *val);
extern int snd_pcm_hw_params_get_channels_min(const snd_pcm_hw_params_t *params, unsigned int *val);
extern int snd_pcm_hw_params_get_channels_max(const snd_pcm_hw_params_t *params, unsigned int *val);
extern int snd_pcm_hw_params_get_access(const snd_pcm_hw_params_t *params, snd_pcm_access_t *_access);
extern int snd_pcm_hw_params_get_period_size(const snd_pcm_hw_params_t *params, snd_pcm_uframes_t *frames, int *dir);
extern int snd_pcm_hw_params_get_buffer_size(const snd_pcm_hw_params_t *params, snd_pcm_uframes_t *val);
extern int snd_pcm_hw_params_get_rate(const snd_pcm_hw_params_t *params, unsigned int *val, int *dir);
extern int snd_pcm_hw_params_set_periods_near(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, unsigned int *val, int *dir);
extern int snd_pcm_hw_params_test_rate(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, unsigned int val, int dir);
extern void snd_pcm_hw_params_get_format_mask(snd_pcm_hw_params_t *params, snd_pcm_format_mask_t *mask);
extern int snd_pcm_hw_params_copy(snd_pcm_hw_params_t *dst, const snd_pcm_hw_params_t *src);
extern int snd_pcm_sw_params_current(snd_pcm_t *pcm, snd_pcm_sw_params_t *params);
extern int snd_pcm_sw_params(snd_pcm_t *pcm, snd_pcm_sw_params_t *params);
extern int snd_pcm_sw_params_set_avail_min(snd_pcm_t *pcm, snd_pcm_sw_params_t *params, snd_pcm_uframes_t val);
extern int snd_pcm_sw_params_set_start_threshold(snd_pcm_t *pcm, snd_pcm_sw_params_t *params, snd_pcm_uframes_t val);
extern int snd_pcm_sw_params_set_stop_threshold(snd_pcm_t *pcm, snd_pcm_sw_params_t *params, snd_pcm_uframes_t val);
extern size_t snd_pcm_sw_params_sizeof(void);
extern int snd_pcm_prepare(snd_pcm_t *pcm);
extern int snd_pcm_reset(snd_pcm_t *pcm);
extern int snd_pcm_start(snd_pcm_t *pcm);
extern int snd_pcm_drop(snd_pcm_t *pcm);
extern int snd_pcm_drain(snd_pcm_t *pcm);
extern int snd_pcm_pause(snd_pcm_t *pcm, int enable);
extern int snd_pcm_hw_params_can_pause(const snd_pcm_hw_params_t *params);
extern int snd_pcm_hwsync(snd_pcm_t *pcm);
extern int snd_pcm_delay(snd_pcm_t *pcm, snd_pcm_sframes_t *delayp);
extern int snd_pcm_resume(snd_pcm_t *pcm);
extern int snd_pcm_htimestamp(snd_pcm_t *pcm, snd_pcm_uframes_t *avail, snd_htimestamp_t *tstamp);
extern snd_pcm_sframes_t snd_pcm_avail(snd_pcm_t *pcm);
extern snd_pcm_sframes_t snd_pcm_avail_update(snd_pcm_t *pcm);
extern snd_pcm_sframes_t snd_pcm_rewindable(snd_pcm_t *pcm);
extern snd_pcm_sframes_t snd_pcm_rewind(snd_pcm_t *pcm, snd_pcm_uframes_t frames);
extern snd_pcm_sframes_t snd_pcm_forwardable(snd_pcm_t *pcm);
extern snd_pcm_sframes_t snd_pcm_forward(snd_pcm_t *pcm, snd_pcm_uframes_t frames);
extern snd_pcm_sframes_t snd_pcm_writei(snd_pcm_t *pcm, const void *buffer, snd_pcm_uframes_t size);
extern snd_pcm_sframes_t snd_pcm_readi(snd_pcm_t *pcm, void *buffer, snd_pcm_uframes_t size);
extern snd_pcm_sframes_t snd_pcm_writen(snd_pcm_t *pcm, void **bufs, snd_pcm_uframes_t size);
extern snd_pcm_sframes_t snd_pcm_readn(snd_pcm_t *pcm, void **bufs, snd_pcm_uframes_t size);
extern int snd_pcm_wait(snd_pcm_t *pcm, int timeout);
extern int snd_pcm_recover(snd_pcm_t *pcm, int err, int silent);
extern snd_pcm_sframes_t snd_pcm_bytes_to_frames(snd_pcm_t *pcm, ssize_t bytes);
extern ssize_t snd_pcm_frames_to_bytes(snd_pcm_t *pcm, snd_pcm_sframes_t frames);
extern int snd_pcm_mmap_begin(snd_pcm_t *pcm, const snd_pcm_channel_area_t **areas, snd_pcm_uframes_t *offset, snd_pcm_uframes_t *frames);
extern snd_pcm_sframes_t snd_pcm_mmap_commit(snd_pcm_t *pcm, snd_pcm_uframes_t offset, snd_pcm_uframes_t frames);
extern int snd_pcm_poll_descriptors_count(snd_pcm_t *pcm);
extern int snd_pcm_poll_descriptors(snd_pcm_t *pcm, struct pollfd *pfds, unsigned int space);
extern int snd_pcm_poll_descriptors_revents(snd_pcm_t *pcm, struct pollfd *pfds, unsigned int nfds, unsigned short *revents);
extern int snd_pcm_nonblock(snd_pcm_t *pcm, int nonblock);
extern int snd_async_add_pcm_handler(snd_async_handler_t **handler, snd_pcm_t *pcm, snd_async_callback_t callback, void *private_data);
extern snd_pcm_t *snd_async_handler_get_pcm(snd_async_handler_t *handler);
extern void *snd_async_handler_get_callback_private(snd_async_handler_t *handler);
extern int snd_async_del_handler(snd_async_handler_t *handler);
extern int snd_pcm_info(snd_pcm_t *pcm, snd_pcm_info_t *info);
extern int snd_pcm_status(snd_pcm_t *pcm, snd_pcm_status_t *status);
extern int snd_pcm_get_params(snd_pcm_t *pcm, snd_pcm_uframes_t *buffer_size, snd_pcm_uframes_t *period_size);
extern int snd_pcm_set_params(snd_pcm_t *pcm, snd_pcm_format_t format, snd_pcm_access_t access, unsigned int channels, unsigned int rate, int soft_resample, unsigned int latency);
extern int snd_pcm_hw_params_get_channels_min(const snd_pcm_hw_params_t *params, unsigned int *val);
extern int snd_pcm_hw_params_get_channels_max(const snd_pcm_hw_params_t *params, unsigned int *val);
extern int snd_pcm_hw_params_set_channels_near(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, unsigned int *val);
extern int snd_pcm_hw_params_set_rate_resample(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, unsigned int val);
extern int snd_pcm_hw_params_get_rate_min(const snd_pcm_hw_params_t *params, unsigned int *val, int *dir);
extern int snd_pcm_hw_params_get_rate_max(const snd_pcm_hw_params_t *params, unsigned int *val, int *dir);
extern int snd_pcm_hw_params_set_periods_min(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, unsigned int *val, int *dir);
extern int snd_pcm_hw_params_set_periods_first(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, unsigned int *val, int *dir);
extern int snd_pcm_hw_params_get_periods(const snd_pcm_hw_params_t *params, unsigned int *val, int *dir);
extern int snd_pcm_hw_params_get_buffer_time_max(const snd_pcm_hw_params_t *params, unsigned int *val, int *dir);
extern int snd_pcm_hw_params_set_buffer_time_near(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, unsigned int *val, int *dir);
extern int snd_pcm_hw_params_get_period_time_min(const snd_pcm_hw_params_t *params, unsigned int *val, int *dir);
extern int snd_pcm_hw_params_set_period_time_near(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, unsigned int *val, int *dir);
extern int snd_pcm_hw_params_get_buffer_size_min(const snd_pcm_hw_params_t *params, snd_pcm_uframes_t *val);
extern int snd_pcm_hw_params_get_buffer_size_max(const snd_pcm_hw_params_t *params, snd_pcm_uframes_t *val);
extern int snd_pcm_hw_params_get_format(const snd_pcm_hw_params_t *params, snd_pcm_format_t *val);
extern int snd_pcm_format_mask_malloc(snd_pcm_format_mask_t **ptr);
extern void snd_pcm_format_mask_free(snd_pcm_format_mask_t *obj);
extern int snd_pcm_format_mask_test(const snd_pcm_format_mask_t *mask, snd_pcm_format_t val);
extern snd_pcm_chmap_query_t **snd_pcm_query_chmaps(snd_pcm_t *pcm);
extern void snd_pcm_free_chmaps(snd_pcm_chmap_query_t **maps);
extern snd_pcm_chmap_t *snd_pcm_get_chmap(snd_pcm_t *pcm);
extern int snd_pcm_set_chmap(snd_pcm_t *pcm, const snd_pcm_chmap_t *map);
extern int snd_pcm_hw_params_copy(snd_pcm_hw_params_t *dst, const snd_pcm_hw_params_t *src);
extern int snd_pcm_hw_params_get_format_mask(const snd_pcm_hw_params_t *params, snd_pcm_format_mask_t *mask);

/* Device name hint functions */
extern int snd_device_name_hint(int card, const char *iface, void ***hints);
extern int snd_device_name_free_hint(void **hints);
extern char *snd_device_name_get_hint(const void *hint, const char *id);

/* Config functions */
extern int snd_config_update(void);
extern int snd_config_load(snd_config_t *config, FILE *stream);
extern int snd_config_search(snd_config_t *config, const char *key, snd_config_t **result);

#endif /* _ALSA_ASOUNDLIB_H */
