/*
    Weak symbol definitions for SDL2 functions that libTAS does not override.

    Auto-generated from SDL2_dynapi_procs.h.
*/

#ifdef __APPLE__

#include "../../external/SDL2.h"

namespace libtas {
namespace sdl2 {

__attribute__((weak)) int SDL_SetError(const char *a, ...) { return 0; }
__attribute__((weak)) void SDL_Log(const char *a, ...) { return (void)0; }
__attribute__((weak)) void SDL_LogVerbose(int a,  const char *b, ...) { return (void)0; }
__attribute__((weak)) void SDL_LogDebug(int a,  const char *b, ...) { return (void)0; }
__attribute__((weak)) void SDL_LogInfo(int a,  const char *b, ...) { return (void)0; }
__attribute__((weak)) void SDL_LogWarn(int a,  const char *b, ...) { return (void)0; }
__attribute__((weak)) void SDL_LogError(int a,  const char *b, ...) { return (void)0; }
__attribute__((weak)) void SDL_LogCritical(int a,  const char *b, ...) { return (void)0; }
__attribute__((weak)) void SDL_LogMessage(int a, SDL_LogPriority b,  const char *c, ...) { return (void)0; }
__attribute__((weak)) int SDL_sscanf(const char *a,  const char *b, ...) { return 0; }
__attribute__((weak)) int SDL_snprintf(char *a, size_t b,  const char *c, ...) { return 0; }
__attribute__((weak)) SDL_Thread* SDL_CreateThread(SDL_ThreadFunction a, const char *b, void *c) { return nullptr; }
__attribute__((weak)) SDL_RWops* SDL_RWFromFP(FILE *a, SDL_bool b) { return nullptr; }
__attribute__((weak)) SDL_RWops* SDL_RWFromFP(void *a, SDL_bool b) { return nullptr; }
__attribute__((weak)) int SDL_RegisterApp(const char *a, Uint32 b, void *c) { return 0; }
__attribute__((weak)) void SDL_UnregisterApp(void) { return (void)0; }
__attribute__((weak)) int SDL_Direct3D9GetAdapterIndex(int a) { return 0; }
__attribute__((weak)) int SDL_iPhoneSetAnimationCallback(SDL_Window *a, int b, void (SDLCALL *c)(void *), void *d) { return 0; }
__attribute__((weak)) void SDL_iPhoneSetEventPump(SDL_bool a) { return (void)0; }
__attribute__((weak)) void* SDL_AndroidGetJNIEnv(void) { return nullptr; }
__attribute__((weak)) void* SDL_AndroidGetActivity(void) { return nullptr; }
__attribute__((weak)) const char* SDL_AndroidGetInternalStoragePath(void) { return nullptr; }
__attribute__((weak)) int SDL_AndroidGetExternalStorageState(void) { return 0; }
__attribute__((weak)) const char* SDL_AndroidGetExternalStoragePath(void) { return nullptr; }
__attribute__((weak)) int SDL_Init(Uint32 a) { return 0; }
__attribute__((weak)) int SDL_InitSubSystem(Uint32 a) { return 0; }
__attribute__((weak)) void SDL_QuitSubSystem(Uint32 a) { return (void)0; }
__attribute__((weak)) Uint32 SDL_WasInit(Uint32 a) { return 0; }
__attribute__((weak)) void SDL_Quit(void) { return (void)0; }
__attribute__((weak)) SDL_assert_state SDL_ReportAssertion(SDL_assert_data *a, const char *b, const char *c, int d) { return (SDL_assert_state)0; }
__attribute__((weak)) void SDL_SetAssertionHandler(SDL_AssertionHandler a, void *b) { return (void)0; }
__attribute__((weak)) const SDL_assert_data* SDL_GetAssertionReport(void) { return nullptr; }
__attribute__((weak)) void SDL_ResetAssertionReport(void) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_AtomicTryLock(SDL_SpinLock *a) { return (SDL_bool)0; }
__attribute__((weak)) void SDL_AtomicLock(SDL_SpinLock *a) { return (void)0; }
__attribute__((weak)) void SDL_AtomicUnlock(SDL_SpinLock *a) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_AtomicCAS(SDL_atomic_t *a, int b, int c) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_AtomicSet(SDL_atomic_t *a, int b) { return 0; }
__attribute__((weak)) int SDL_AtomicGet(SDL_atomic_t *a) { return 0; }
__attribute__((weak)) int SDL_AtomicAdd(SDL_atomic_t *a, int b) { return 0; }
__attribute__((weak)) SDL_bool SDL_AtomicCASPtr(void **a, void *b, void *c) { return (SDL_bool)0; }
__attribute__((weak)) void* SDL_AtomicSetPtr(void **a, void *b) { return nullptr; }
__attribute__((weak)) void* SDL_AtomicGetPtr(void **a) { return nullptr; }
__attribute__((weak)) int SDL_GetNumAudioDrivers(void) { return 0; }
__attribute__((weak)) const char* SDL_GetAudioDriver(int a) { return nullptr; }
__attribute__((weak)) int SDL_AudioInit(const char *a) { return 0; }
__attribute__((weak)) void SDL_AudioQuit(void) { return (void)0; }
__attribute__((weak)) const char* SDL_GetCurrentAudioDriver(void) { return nullptr; }
__attribute__((weak)) int SDL_OpenAudio(SDL_AudioSpec *a, SDL_AudioSpec *b) { return 0; }
__attribute__((weak)) int SDL_GetNumAudioDevices(int a) { return 0; }
__attribute__((weak)) const char* SDL_GetAudioDeviceName(int a, int b) { return nullptr; }
__attribute__((weak)) SDL_AudioDeviceID SDL_OpenAudioDevice(const char *a, int b, const SDL_AudioSpec *c, SDL_AudioSpec *d, int e) { return (SDL_AudioDeviceID)0; }
__attribute__((weak)) SDL_AudioStatus SDL_GetAudioStatus(void) { return (SDL_AudioStatus)0; }
__attribute__((weak)) SDL_AudioStatus SDL_GetAudioDeviceStatus(SDL_AudioDeviceID a) { return (SDL_AudioStatus)0; }
__attribute__((weak)) void SDL_PauseAudio(int a) { return (void)0; }
__attribute__((weak)) void SDL_PauseAudioDevice(SDL_AudioDeviceID a, int b) { return (void)0; }
__attribute__((weak)) SDL_AudioSpec* SDL_LoadWAV_RW(SDL_RWops *a, int b, SDL_AudioSpec *c, Uint8 **d, Uint32 *e) { return nullptr; }
__attribute__((weak)) void SDL_FreeWAV(Uint8 *a) { return (void)0; }
__attribute__((weak)) int SDL_BuildAudioCVT(SDL_AudioCVT *a, SDL_AudioFormat b, Uint8 c, int d, SDL_AudioFormat e, Uint8 f, int g) { return 0; }
__attribute__((weak)) int SDL_ConvertAudio(SDL_AudioCVT *a) { return 0; }
__attribute__((weak)) void SDL_MixAudio(Uint8 *a, const Uint8 *b, Uint32 c, int d) { return (void)0; }
__attribute__((weak)) void SDL_MixAudioFormat(Uint8 *a, const Uint8 *b, SDL_AudioFormat c, Uint32 d, int e) { return (void)0; }
__attribute__((weak)) void SDL_LockAudio(void) { return (void)0; }
__attribute__((weak)) void SDL_LockAudioDevice(SDL_AudioDeviceID a) { return (void)0; }
__attribute__((weak)) void SDL_UnlockAudio(void) { return (void)0; }
__attribute__((weak)) void SDL_UnlockAudioDevice(SDL_AudioDeviceID a) { return (void)0; }
__attribute__((weak)) void SDL_CloseAudio(void) { return (void)0; }
__attribute__((weak)) void SDL_CloseAudioDevice(SDL_AudioDeviceID a) { return (void)0; }
__attribute__((weak)) int SDL_SetClipboardText(const char *a) { return 0; }
__attribute__((weak)) char* SDL_GetClipboardText(void) { return nullptr; }
__attribute__((weak)) SDL_bool SDL_HasClipboardText(void) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_GetCPUCount(void) { return 0; }
__attribute__((weak)) int SDL_GetCPUCacheLineSize(void) { return 0; }
__attribute__((weak)) SDL_bool SDL_HasRDTSC(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_HasAltiVec(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_HasMMX(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_Has3DNow(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_HasSSE(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_HasSSE2(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_HasSSE3(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_HasSSE41(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_HasSSE42(void) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_GetSystemRAM(void) { return 0; }
__attribute__((weak)) const char* SDL_GetError(void) { return nullptr; }
__attribute__((weak)) void SDL_ClearError(void) { return (void)0; }
__attribute__((weak)) int SDL_Error(SDL_errorcode a) { return 0; }
__attribute__((weak)) void SDL_PumpEvents(void) { return (void)0; }
__attribute__((weak)) int SDL_PeepEvents(SDL_Event *a, int b, SDL_eventaction c, Uint32 d, Uint32 e) { return 0; }
__attribute__((weak)) SDL_bool SDL_HasEvent(Uint32 a) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_HasEvents(Uint32 a, Uint32 b) { return (SDL_bool)0; }
__attribute__((weak)) void SDL_FlushEvent(Uint32 a) { return (void)0; }
__attribute__((weak)) void SDL_FlushEvents(Uint32 a, Uint32 b) { return (void)0; }
__attribute__((weak)) int SDL_PollEvent(SDL_Event *a) { return 0; }
__attribute__((weak)) int SDL_WaitEvent(SDL_Event *a) { return 0; }
__attribute__((weak)) int SDL_WaitEventTimeout(SDL_Event *a, int b) { return 0; }
__attribute__((weak)) int SDL_PushEvent(SDL_Event *a) { return 0; }
__attribute__((weak)) void SDL_SetEventFilter(SDL_EventFilter a, void *b) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_GetEventFilter(SDL_EventFilter *a, void **b) { return (SDL_bool)0; }
__attribute__((weak)) void SDL_AddEventWatch(SDL_EventFilter a, void *b) { return (void)0; }
__attribute__((weak)) void SDL_DelEventWatch(SDL_EventFilter a, void *b) { return (void)0; }
__attribute__((weak)) void SDL_FilterEvents(SDL_EventFilter a, void *b) { return (void)0; }
__attribute__((weak)) Uint8 SDL_EventState(Uint32 a, int b) { return 0; }
__attribute__((weak)) Uint32 SDL_RegisterEvents(int a) { return 0; }
__attribute__((weak)) char* SDL_GetBasePath(void) { return nullptr; }
__attribute__((weak)) char* SDL_GetPrefPath(const char *a, const char *b) { return nullptr; }
__attribute__((weak)) int SDL_GameControllerAddMapping(const char *a) { return 0; }
__attribute__((weak)) char* SDL_GameControllerMappingForGUID(SDL_JoystickGUID a) { return nullptr; }
__attribute__((weak)) char* SDL_GameControllerMapping(SDL_GameController *a) { return nullptr; }
__attribute__((weak)) SDL_bool SDL_IsGameController(int a) { return (SDL_bool)0; }
__attribute__((weak)) const char* SDL_GameControllerNameForIndex(int a) { return nullptr; }
__attribute__((weak)) SDL_GameController* SDL_GameControllerOpen(int a) { return nullptr; }
__attribute__((weak)) const char* SDL_GameControllerName(SDL_GameController *a) { return nullptr; }
__attribute__((weak)) SDL_bool SDL_GameControllerGetAttached(SDL_GameController *a) { return (SDL_bool)0; }
__attribute__((weak)) SDL_Joystick* SDL_GameControllerGetJoystick(SDL_GameController *a) { return nullptr; }
__attribute__((weak)) int SDL_GameControllerEventState(int a) { return 0; }
__attribute__((weak)) void SDL_GameControllerUpdate(void) { return (void)0; }
__attribute__((weak)) SDL_GameControllerAxis SDL_GameControllerGetAxisFromString(const char *a) { return (SDL_GameControllerAxis)0; }
__attribute__((weak)) const char* SDL_GameControllerGetStringForAxis(SDL_GameControllerAxis a) { return nullptr; }
__attribute__((weak)) SDL_GameControllerButtonBind SDL_GameControllerGetBindForAxis(SDL_GameController *a, SDL_GameControllerAxis b) { return {}; }
__attribute__((weak)) Sint16 SDL_GameControllerGetAxis(SDL_GameController *a, SDL_GameControllerAxis b) { return 0; }
__attribute__((weak)) SDL_GameControllerButton SDL_GameControllerGetButtonFromString(const char *a) { return (SDL_GameControllerButton)0; }
__attribute__((weak)) const char* SDL_GameControllerGetStringForButton(SDL_GameControllerButton a) { return nullptr; }
__attribute__((weak)) SDL_GameControllerButtonBind SDL_GameControllerGetBindForButton(SDL_GameController *a, SDL_GameControllerButton b) { return {}; }
__attribute__((weak)) Uint8 SDL_GameControllerGetButton(SDL_GameController *a, SDL_GameControllerButton b) { return 0; }
__attribute__((weak)) void SDL_GameControllerClose(SDL_GameController *a) { return (void)0; }
__attribute__((weak)) int SDL_RecordGesture(SDL_TouchID a) { return 0; }
__attribute__((weak)) int SDL_SaveAllDollarTemplates(SDL_RWops *a) { return 0; }
__attribute__((weak)) int SDL_SaveDollarTemplate(SDL_GestureID a, SDL_RWops *b) { return 0; }
__attribute__((weak)) int SDL_LoadDollarTemplates(SDL_TouchID a, SDL_RWops *b) { return 0; }
__attribute__((weak)) int SDL_NumHaptics(void) { return 0; }
__attribute__((weak)) const char* SDL_HapticName(int a) { return nullptr; }
__attribute__((weak)) SDL_Haptic* SDL_HapticOpen(int a) { return nullptr; }
__attribute__((weak)) int SDL_HapticOpened(int a) { return 0; }
__attribute__((weak)) int SDL_HapticIndex(SDL_Haptic *a) { return 0; }
__attribute__((weak)) int SDL_MouseIsHaptic(void) { return 0; }
__attribute__((weak)) SDL_Haptic* SDL_HapticOpenFromMouse(void) { return nullptr; }
__attribute__((weak)) int SDL_JoystickIsHaptic(SDL_Joystick *a) { return 0; }
__attribute__((weak)) SDL_Haptic* SDL_HapticOpenFromJoystick(SDL_Joystick *a) { return nullptr; }
__attribute__((weak)) void SDL_HapticClose(SDL_Haptic *a) { return (void)0; }
__attribute__((weak)) int SDL_HapticNumEffects(SDL_Haptic *a) { return 0; }
__attribute__((weak)) int SDL_HapticNumEffectsPlaying(SDL_Haptic *a) { return 0; }
__attribute__((weak)) unsigned int SDL_HapticQuery(SDL_Haptic *a) { return (unsigned int)0; }
__attribute__((weak)) int SDL_HapticNumAxes(SDL_Haptic *a) { return 0; }
__attribute__((weak)) int SDL_HapticEffectSupported(SDL_Haptic *a, SDL_HapticEffect *b) { return 0; }
__attribute__((weak)) int SDL_HapticNewEffect(SDL_Haptic *a, SDL_HapticEffect *b) { return 0; }
__attribute__((weak)) int SDL_HapticUpdateEffect(SDL_Haptic *a, int b, SDL_HapticEffect *c) { return 0; }
__attribute__((weak)) int SDL_HapticRunEffect(SDL_Haptic *a, int b, Uint32 c) { return 0; }
__attribute__((weak)) int SDL_HapticStopEffect(SDL_Haptic *a, int b) { return 0; }
__attribute__((weak)) void SDL_HapticDestroyEffect(SDL_Haptic *a, int b) { return (void)0; }
__attribute__((weak)) int SDL_HapticGetEffectStatus(SDL_Haptic *a, int b) { return 0; }
__attribute__((weak)) int SDL_HapticSetGain(SDL_Haptic *a, int b) { return 0; }
__attribute__((weak)) int SDL_HapticSetAutocenter(SDL_Haptic *a, int b) { return 0; }
__attribute__((weak)) int SDL_HapticPause(SDL_Haptic *a) { return 0; }
__attribute__((weak)) int SDL_HapticUnpause(SDL_Haptic *a) { return 0; }
__attribute__((weak)) int SDL_HapticStopAll(SDL_Haptic *a) { return 0; }
__attribute__((weak)) int SDL_HapticRumbleSupported(SDL_Haptic *a) { return 0; }
__attribute__((weak)) int SDL_HapticRumbleInit(SDL_Haptic *a) { return 0; }
__attribute__((weak)) int SDL_HapticRumblePlay(SDL_Haptic *a, float b, Uint32 c) { return 0; }
__attribute__((weak)) int SDL_HapticRumbleStop(SDL_Haptic *a) { return 0; }
__attribute__((weak)) SDL_bool SDL_SetHintWithPriority(const char *a, const char *b, SDL_HintPriority c) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_SetHint(const char *a, const char *b) { return (SDL_bool)0; }
__attribute__((weak)) const char* SDL_GetHint(const char *a) { return nullptr; }
__attribute__((weak)) void SDL_AddHintCallback(const char *a, SDL_HintCallback b, void *c) { return (void)0; }
__attribute__((weak)) void SDL_DelHintCallback(const char *a, SDL_HintCallback b, void *c) { return (void)0; }
__attribute__((weak)) void SDL_ClearHints(void) { return (void)0; }
__attribute__((weak)) int SDL_NumJoysticks(void) { return 0; }
__attribute__((weak)) const char* SDL_JoystickNameForIndex(int a) { return nullptr; }
__attribute__((weak)) SDL_Joystick* SDL_JoystickOpen(int a) { return nullptr; }
__attribute__((weak)) const char* SDL_JoystickName(SDL_Joystick *a) { return nullptr; }
__attribute__((weak)) SDL_JoystickGUID SDL_JoystickGetDeviceGUID(int a) { return {}; }
__attribute__((weak)) SDL_JoystickGUID SDL_JoystickGetGUID(SDL_Joystick *a) { return {}; }
__attribute__((weak)) void SDL_JoystickGetGUIDString(SDL_JoystickGUID a, char *b, int c) { return (void)0; }
__attribute__((weak)) SDL_JoystickGUID SDL_JoystickGetGUIDFromString(const char *a) { return {}; }
__attribute__((weak)) SDL_bool SDL_JoystickGetAttached(SDL_Joystick *a) { return (SDL_bool)0; }
__attribute__((weak)) SDL_JoystickID SDL_JoystickInstanceID(SDL_Joystick *a) { return (SDL_JoystickID)0; }
__attribute__((weak)) int SDL_JoystickNumAxes(SDL_Joystick *a) { return 0; }
__attribute__((weak)) int SDL_JoystickNumBalls(SDL_Joystick *a) { return 0; }
__attribute__((weak)) int SDL_JoystickNumHats(SDL_Joystick *a) { return 0; }
__attribute__((weak)) int SDL_JoystickNumButtons(SDL_Joystick *a) { return 0; }
__attribute__((weak)) void SDL_JoystickUpdate(void) { return (void)0; }
__attribute__((weak)) int SDL_JoystickEventState(int a) { return 0; }
__attribute__((weak)) Sint16 SDL_JoystickGetAxis(SDL_Joystick *a, int b) { return 0; }
__attribute__((weak)) Uint8 SDL_JoystickGetHat(SDL_Joystick *a, int b) { return 0; }
__attribute__((weak)) int SDL_JoystickGetBall(SDL_Joystick *a, int b, int *c, int *d) { return 0; }
__attribute__((weak)) Uint8 SDL_JoystickGetButton(SDL_Joystick *a, int b) { return 0; }
__attribute__((weak)) void SDL_JoystickClose(SDL_Joystick *a) { return (void)0; }
__attribute__((weak)) SDL_Window* SDL_GetKeyboardFocus(void) { return nullptr; }
__attribute__((weak)) const Uint8* SDL_GetKeyboardState(int *a) { return nullptr; }
__attribute__((weak)) SDL_Keymod SDL_GetModState(void) { return (SDL_Keymod)0; }
__attribute__((weak)) void SDL_SetModState(SDL_Keymod a) { return (void)0; }
__attribute__((weak)) SDL_Keycode SDL_GetKeyFromScancode(SDL_Scancode a) { return (SDL_Keycode)0; }
__attribute__((weak)) SDL_Scancode SDL_GetScancodeFromKey(SDL_Keycode a) { return (SDL_Scancode)0; }
__attribute__((weak)) const char* SDL_GetScancodeName(SDL_Scancode a) { return nullptr; }
__attribute__((weak)) SDL_Scancode SDL_GetScancodeFromName(const char *a) { return (SDL_Scancode)0; }
__attribute__((weak)) const char* SDL_GetKeyName(SDL_Keycode a) { return nullptr; }
__attribute__((weak)) SDL_Keycode SDL_GetKeyFromName(const char *a) { return (SDL_Keycode)0; }
__attribute__((weak)) void SDL_StartTextInput(void) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_IsTextInputActive(void) { return (SDL_bool)0; }
__attribute__((weak)) void SDL_StopTextInput(void) { return (void)0; }
__attribute__((weak)) void SDL_SetTextInputRect(const SDL_Rect *a) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_HasScreenKeyboardSupport(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_IsScreenKeyboardShown(SDL_Window *a) { return (SDL_bool)0; }
__attribute__((weak)) void* SDL_LoadObject(const char *a) { return nullptr; }
__attribute__((weak)) void* SDL_LoadFunction(void *a, const char *b) { return nullptr; }
__attribute__((weak)) void SDL_UnloadObject(void *a) { return (void)0; }
__attribute__((weak)) void SDL_LogSetAllPriority(SDL_LogPriority a) { return (void)0; }
__attribute__((weak)) void SDL_LogSetPriority(int a, SDL_LogPriority b) { return (void)0; }
__attribute__((weak)) SDL_LogPriority SDL_LogGetPriority(int a) { return (SDL_LogPriority)0; }
__attribute__((weak)) void SDL_LogResetPriorities(void) { return (void)0; }
__attribute__((weak)) void SDL_LogMessageV(int a, SDL_LogPriority b, const char *c, va_list d) { return (void)0; }
__attribute__((weak)) void SDL_LogGetOutputFunction(SDL_LogOutputFunction *a, void **b) { return (void)0; }
__attribute__((weak)) void SDL_LogSetOutputFunction(SDL_LogOutputFunction a, void *b) { return (void)0; }
__attribute__((weak)) void SDL_SetMainReady(void) { return (void)0; }
__attribute__((weak)) int SDL_ShowMessageBox(const SDL_MessageBoxData *a, int *b) { return 0; }
__attribute__((weak)) int SDL_ShowSimpleMessageBox(Uint32 a, const char *b, const char *c, SDL_Window *d) { return 0; }
__attribute__((weak)) SDL_Window* SDL_GetMouseFocus(void) { return nullptr; }
__attribute__((weak)) Uint32 SDL_GetMouseState(int *a, int *b) { return 0; }
__attribute__((weak)) Uint32 SDL_GetRelativeMouseState(int *a, int *b) { return 0; }
__attribute__((weak)) void SDL_WarpMouseInWindow(SDL_Window *a, int b, int c) { return (void)0; }
__attribute__((weak)) int SDL_SetRelativeMouseMode(SDL_bool a) { return 0; }
__attribute__((weak)) SDL_bool SDL_GetRelativeMouseMode(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_Cursor* SDL_CreateCursor(const Uint8 *a, const Uint8 *b, int c, int d, int e, int f) { return nullptr; }
__attribute__((weak)) SDL_Cursor* SDL_CreateColorCursor(SDL_Surface *a, int b, int c) { return nullptr; }
__attribute__((weak)) SDL_Cursor* SDL_CreateSystemCursor(SDL_SystemCursor a) { return nullptr; }
__attribute__((weak)) void SDL_SetCursor(SDL_Cursor *a) { return (void)0; }
__attribute__((weak)) SDL_Cursor* SDL_GetCursor(void) { return nullptr; }
__attribute__((weak)) SDL_Cursor* SDL_GetDefaultCursor(void) { return nullptr; }
__attribute__((weak)) void SDL_FreeCursor(SDL_Cursor *a) { return (void)0; }
__attribute__((weak)) int SDL_ShowCursor(int a) { return 0; }
__attribute__((weak)) SDL_mutex* SDL_CreateMutex(void) { return nullptr; }
__attribute__((weak)) int SDL_LockMutex(SDL_mutex *a) { return 0; }
__attribute__((weak)) int SDL_TryLockMutex(SDL_mutex *a) { return 0; }
__attribute__((weak)) int SDL_UnlockMutex(SDL_mutex *a) { return 0; }
__attribute__((weak)) void SDL_DestroyMutex(SDL_mutex *a) { return (void)0; }
__attribute__((weak)) SDL_sem* SDL_CreateSemaphore(Uint32 a) { return nullptr; }
__attribute__((weak)) void SDL_DestroySemaphore(SDL_sem *a) { return (void)0; }
__attribute__((weak)) int SDL_SemWait(SDL_sem *a) { return 0; }
__attribute__((weak)) int SDL_SemTryWait(SDL_sem *a) { return 0; }
__attribute__((weak)) int SDL_SemWaitTimeout(SDL_sem *a, Uint32 b) { return 0; }
__attribute__((weak)) int SDL_SemPost(SDL_sem *a) { return 0; }
__attribute__((weak)) Uint32 SDL_SemValue(SDL_sem *a) { return 0; }
__attribute__((weak)) SDL_cond* SDL_CreateCond(void) { return nullptr; }
__attribute__((weak)) void SDL_DestroyCond(SDL_cond *a) { return (void)0; }
__attribute__((weak)) int SDL_CondSignal(SDL_cond *a) { return 0; }
__attribute__((weak)) int SDL_CondBroadcast(SDL_cond *a) { return 0; }
__attribute__((weak)) int SDL_CondWait(SDL_cond *a, SDL_mutex *b) { return 0; }
__attribute__((weak)) int SDL_CondWaitTimeout(SDL_cond *a, SDL_mutex *b, Uint32 c) { return 0; }
__attribute__((weak)) const char* SDL_GetPixelFormatName(Uint32 a) { return nullptr; }
__attribute__((weak)) SDL_bool SDL_PixelFormatEnumToMasks(Uint32 a, int *b, Uint32 *c, Uint32 *d, Uint32 *e, Uint32 *f) { return (SDL_bool)0; }
__attribute__((weak)) Uint32 SDL_MasksToPixelFormatEnum(int a, Uint32 b, Uint32 c, Uint32 d, Uint32 e) { return 0; }
__attribute__((weak)) SDL_PixelFormat* SDL_AllocFormat(Uint32 a) { return nullptr; }
__attribute__((weak)) void SDL_FreeFormat(SDL_PixelFormat *a) { return (void)0; }
__attribute__((weak)) SDL_Palette* SDL_AllocPalette(int a) { return nullptr; }
__attribute__((weak)) int SDL_SetPixelFormatPalette(SDL_PixelFormat *a, SDL_Palette *b) { return 0; }
__attribute__((weak)) int SDL_SetPaletteColors(SDL_Palette *a, const SDL_Color *b, int c, int d) { return 0; }
__attribute__((weak)) void SDL_FreePalette(SDL_Palette *a) { return (void)0; }
__attribute__((weak)) Uint32 SDL_MapRGB(const SDL_PixelFormat *a, Uint8 b, Uint8 c, Uint8 d) { return 0; }
__attribute__((weak)) Uint32 SDL_MapRGBA(const SDL_PixelFormat *a, Uint8 b, Uint8 c, Uint8 d, Uint8 e) { return 0; }
__attribute__((weak)) void SDL_GetRGB(Uint32 a, const SDL_PixelFormat *b, Uint8 *c, Uint8 *d, Uint8 *e) { return (void)0; }
__attribute__((weak)) void SDL_GetRGBA(Uint32 a, const SDL_PixelFormat *b, Uint8 *c, Uint8 *d, Uint8 *e, Uint8 *f) { return (void)0; }
__attribute__((weak)) void SDL_CalculateGammaRamp(float a, Uint16 *b) { return (void)0; }
__attribute__((weak)) const char* SDL_GetPlatform(void) { return nullptr; }
__attribute__((weak)) SDL_PowerState SDL_GetPowerInfo(int *a, int *b) { return (SDL_PowerState)0; }
__attribute__((weak)) SDL_bool SDL_HasIntersection(const SDL_Rect *a, const SDL_Rect *b) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_IntersectRect(const SDL_Rect *a, const SDL_Rect *b, SDL_Rect *c) { return (SDL_bool)0; }
__attribute__((weak)) void SDL_UnionRect(const SDL_Rect *a, const SDL_Rect *b, SDL_Rect *c) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_EnclosePoints(const SDL_Point *a, int b, const SDL_Rect *c, SDL_Rect *d) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_IntersectRectAndLine(const SDL_Rect *a, int *b, int *c, int *d, int *e) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_GetNumRenderDrivers(void) { return 0; }
__attribute__((weak)) int SDL_GetRenderDriverInfo(int a, SDL_RendererInfo *b) { return 0; }
__attribute__((weak)) int SDL_CreateWindowAndRenderer(int a, int b, Uint32 c, SDL_Window **d, SDL_Renderer **e) { return 0; }
__attribute__((weak)) SDL_Renderer* SDL_CreateRenderer(SDL_Window *a, int b, Uint32 c) { return nullptr; }
__attribute__((weak)) SDL_Renderer* SDL_CreateSoftwareRenderer(SDL_Surface *a) { return nullptr; }
__attribute__((weak)) SDL_Renderer* SDL_GetRenderer(SDL_Window *a) { return nullptr; }
__attribute__((weak)) int SDL_GetRendererInfo(SDL_Renderer *a, SDL_RendererInfo *b) { return 0; }
__attribute__((weak)) int SDL_GetRendererOutputSize(SDL_Renderer *a, int *b, int *c) { return 0; }
__attribute__((weak)) SDL_Texture* SDL_CreateTexture(SDL_Renderer *a, Uint32 b, int c, int d, int e) { return nullptr; }
__attribute__((weak)) SDL_Texture* SDL_CreateTextureFromSurface(SDL_Renderer *a, SDL_Surface *b) { return nullptr; }
__attribute__((weak)) int SDL_QueryTexture(SDL_Texture *a, Uint32 *b, int *c, int *d, int *e) { return 0; }
__attribute__((weak)) int SDL_SetTextureColorMod(SDL_Texture *a, Uint8 b, Uint8 c, Uint8 d) { return 0; }
__attribute__((weak)) int SDL_GetTextureColorMod(SDL_Texture *a, Uint8 *b, Uint8 *c, Uint8 *d) { return 0; }
__attribute__((weak)) int SDL_SetTextureAlphaMod(SDL_Texture *a, Uint8 b) { return 0; }
__attribute__((weak)) int SDL_GetTextureAlphaMod(SDL_Texture *a, Uint8 *b) { return 0; }
__attribute__((weak)) int SDL_SetTextureBlendMode(SDL_Texture *a, SDL_BlendMode b) { return 0; }
__attribute__((weak)) int SDL_GetTextureBlendMode(SDL_Texture *a, SDL_BlendMode *b) { return 0; }
__attribute__((weak)) int SDL_UpdateTexture(SDL_Texture *a, const SDL_Rect *b, const void *c, int d) { return 0; }
__attribute__((weak)) int SDL_UpdateYUVTexture(SDL_Texture *a, const SDL_Rect *b, const Uint8 *c, int d, const Uint8 *e, int f, const Uint8 *g, int h) { return 0; }
__attribute__((weak)) int SDL_LockTexture(SDL_Texture *a, const SDL_Rect *b, void **c, int *d) { return 0; }
__attribute__((weak)) void SDL_UnlockTexture(SDL_Texture *a) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_RenderTargetSupported(SDL_Renderer *a) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_SetRenderTarget(SDL_Renderer *a, SDL_Texture *b) { return 0; }
__attribute__((weak)) SDL_Texture* SDL_GetRenderTarget(SDL_Renderer *a) { return nullptr; }
__attribute__((weak)) int SDL_RenderSetLogicalSize(SDL_Renderer *a, int b, int c) { return 0; }
__attribute__((weak)) void SDL_RenderGetLogicalSize(SDL_Renderer *a, int *b, int *c) { return (void)0; }
__attribute__((weak)) int SDL_RenderSetViewport(SDL_Renderer *a, const SDL_Rect *b) { return 0; }
__attribute__((weak)) void SDL_RenderGetViewport(SDL_Renderer *a, SDL_Rect *b) { return (void)0; }
__attribute__((weak)) int SDL_RenderSetClipRect(SDL_Renderer *a, const SDL_Rect *b) { return 0; }
__attribute__((weak)) void SDL_RenderGetClipRect(SDL_Renderer *a, SDL_Rect *b) { return (void)0; }
__attribute__((weak)) int SDL_RenderSetScale(SDL_Renderer *a, float b, float c) { return 0; }
__attribute__((weak)) void SDL_RenderGetScale(SDL_Renderer *a, float *b, float *c) { return (void)0; }
__attribute__((weak)) int SDL_SetRenderDrawColor(SDL_Renderer *a, Uint8 b, Uint8 c, Uint8 d, Uint8 e) { return 0; }
__attribute__((weak)) int SDL_GetRenderDrawColor(SDL_Renderer *a, Uint8 *b, Uint8 *c, Uint8 *d, Uint8 *e) { return 0; }
__attribute__((weak)) int SDL_SetRenderDrawBlendMode(SDL_Renderer *a, SDL_BlendMode b) { return 0; }
__attribute__((weak)) int SDL_GetRenderDrawBlendMode(SDL_Renderer *a, SDL_BlendMode *b) { return 0; }
__attribute__((weak)) int SDL_RenderClear(SDL_Renderer *a) { return 0; }
__attribute__((weak)) int SDL_RenderDrawPoint(SDL_Renderer *a, int b, int c) { return 0; }
__attribute__((weak)) int SDL_RenderDrawPoints(SDL_Renderer *a, const SDL_Point *b, int c) { return 0; }
__attribute__((weak)) int SDL_RenderDrawLine(SDL_Renderer *a, int b, int c, int d, int e) { return 0; }
__attribute__((weak)) int SDL_RenderDrawLines(SDL_Renderer *a, const SDL_Point *b, int c) { return 0; }
__attribute__((weak)) int SDL_RenderDrawRect(SDL_Renderer *a, const SDL_Rect *b) { return 0; }
__attribute__((weak)) int SDL_RenderDrawRects(SDL_Renderer *a, const SDL_Rect *b, int c) { return 0; }
__attribute__((weak)) int SDL_RenderFillRect(SDL_Renderer *a, const SDL_Rect *b) { return 0; }
__attribute__((weak)) int SDL_RenderFillRects(SDL_Renderer *a, const SDL_Rect *b, int c) { return 0; }
__attribute__((weak)) int SDL_RenderCopy(SDL_Renderer *a, SDL_Texture *b, const SDL_Rect *c, const SDL_Rect *d) { return 0; }
__attribute__((weak)) int SDL_RenderCopyEx(SDL_Renderer *a, SDL_Texture *b, const SDL_Rect *c, const SDL_Rect *d, const double e, const SDL_Point *f, const SDL_RendererFlip g) { return 0; }
__attribute__((weak)) int SDL_RenderReadPixels(SDL_Renderer *a, const SDL_Rect *b, Uint32 c, void *d, int e) { return 0; }
__attribute__((weak)) void SDL_RenderPresent(SDL_Renderer *a) { return (void)0; }
__attribute__((weak)) void SDL_DestroyTexture(SDL_Texture *a) { return (void)0; }
__attribute__((weak)) void SDL_DestroyRenderer(SDL_Renderer *a) { return (void)0; }
__attribute__((weak)) int SDL_GL_BindTexture(SDL_Texture *a, float *b, float *c) { return 0; }
__attribute__((weak)) int SDL_GL_UnbindTexture(SDL_Texture *a) { return 0; }
__attribute__((weak)) SDL_RWops* SDL_RWFromFile(const char *a, const char *b) { return nullptr; }
__attribute__((weak)) SDL_RWops* SDL_RWFromMem(void *a, int b) { return nullptr; }
__attribute__((weak)) SDL_RWops* SDL_RWFromConstMem(const void *a, int b) { return nullptr; }
__attribute__((weak)) SDL_RWops* SDL_AllocRW(void) { return nullptr; }
__attribute__((weak)) void SDL_FreeRW(SDL_RWops *a) { return (void)0; }
__attribute__((weak)) Uint8 SDL_ReadU8(SDL_RWops *a) { return 0; }
__attribute__((weak)) Uint16 SDL_ReadLE16(SDL_RWops *a) { return 0; }
__attribute__((weak)) Uint16 SDL_ReadBE16(SDL_RWops *a) { return 0; }
__attribute__((weak)) Uint32 SDL_ReadLE32(SDL_RWops *a) { return 0; }
__attribute__((weak)) Uint32 SDL_ReadBE32(SDL_RWops *a) { return 0; }
__attribute__((weak)) Uint64 SDL_ReadLE64(SDL_RWops *a) { return 0; }
__attribute__((weak)) Uint64 SDL_ReadBE64(SDL_RWops *a) { return 0; }
__attribute__((weak)) size_t SDL_WriteU8(SDL_RWops *a, Uint8 b) { return 0; }
__attribute__((weak)) size_t SDL_WriteLE16(SDL_RWops *a, Uint16 b) { return 0; }
__attribute__((weak)) size_t SDL_WriteBE16(SDL_RWops *a, Uint16 b) { return 0; }
__attribute__((weak)) size_t SDL_WriteLE32(SDL_RWops *a, Uint32 b) { return 0; }
__attribute__((weak)) size_t SDL_WriteBE32(SDL_RWops *a, Uint32 b) { return 0; }
__attribute__((weak)) size_t SDL_WriteLE64(SDL_RWops *a, Uint64 b) { return 0; }
__attribute__((weak)) size_t SDL_WriteBE64(SDL_RWops *a, Uint64 b) { return 0; }
__attribute__((weak)) SDL_Window* SDL_CreateShapedWindow(const char *a, unsigned int b, unsigned int c, unsigned int d, unsigned int e, Uint32 f) { return nullptr; }
__attribute__((weak)) SDL_bool SDL_IsShapedWindow(const SDL_Window *a) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_SetWindowShape(SDL_Window *a, SDL_Surface *b, SDL_WindowShapeMode *c) { return 0; }
__attribute__((weak)) int SDL_GetShapedWindowMode(SDL_Window *a, SDL_WindowShapeMode *b) { return 0; }
__attribute__((weak)) void* SDL_malloc(size_t a) { return nullptr; }
__attribute__((weak)) void* SDL_calloc(size_t a, size_t b) { return nullptr; }
__attribute__((weak)) void* SDL_realloc(void *a, size_t b) { return nullptr; }
__attribute__((weak)) void SDL_free(void *a) { return (void)0; }
__attribute__((weak)) char* SDL_getenv(const char *a) { return nullptr; }
__attribute__((weak)) int SDL_setenv(const char *a, const char *b, int c) { return 0; }
__attribute__((weak)) void SDL_qsort(void *a, size_t b, size_t c, int (SDLCALL *d)(const void *, const void *)) { return (void)0; }
__attribute__((weak)) int SDL_abs(int a) { return 0; }
__attribute__((weak)) int SDL_isdigit(int a) { return 0; }
__attribute__((weak)) int SDL_isspace(int a) { return 0; }
__attribute__((weak)) int SDL_toupper(int a) { return 0; }
__attribute__((weak)) int SDL_tolower(int a) { return 0; }
__attribute__((weak)) void* SDL_memset(void *a, int b, size_t c) { return nullptr; }
__attribute__((weak)) void* SDL_memcpy(void *a,  const void *b, size_t c) { return nullptr; }
__attribute__((weak)) void* SDL_memmove(void *a,  const void *b, size_t c) { return nullptr; }
__attribute__((weak)) int SDL_memcmp(const void *a, const void *b, size_t c) { return 0; }
__attribute__((weak)) size_t SDL_wcslen(const wchar_t *a) { return 0; }
__attribute__((weak)) size_t SDL_wcslcpy(wchar_t *a, const wchar_t *b, size_t c) { return 0; }
__attribute__((weak)) size_t SDL_wcslcat(wchar_t *a, const wchar_t *b, size_t c) { return 0; }
__attribute__((weak)) size_t SDL_strlen(const char *a) { return 0; }
__attribute__((weak)) size_t SDL_strlcpy(char *a, const char *b, size_t c) { return 0; }
__attribute__((weak)) size_t SDL_utf8strlcpy(char *a, const char *b, size_t c) { return 0; }
__attribute__((weak)) size_t SDL_strlcat(char *a, const char *b, size_t c) { return 0; }
__attribute__((weak)) char* SDL_strdup(const char *a) { return nullptr; }
__attribute__((weak)) char* SDL_strrev(char *a) { return nullptr; }
__attribute__((weak)) char* SDL_strupr(char *a) { return nullptr; }
__attribute__((weak)) char* SDL_strlwr(char *a) { return nullptr; }
__attribute__((weak)) char* SDL_strchr(const char *a, int b) { return nullptr; }
__attribute__((weak)) char* SDL_strrchr(const char *a, int b) { return nullptr; }
__attribute__((weak)) char* SDL_strstr(const char *a, const char *b) { return nullptr; }
__attribute__((weak)) char* SDL_itoa(int a, char *b, int c) { return nullptr; }
__attribute__((weak)) char* SDL_uitoa(unsigned int a, char *b, int c) { return nullptr; }
__attribute__((weak)) char* SDL_ltoa(long a, char *b, int c) { return nullptr; }
__attribute__((weak)) char* SDL_ultoa(unsigned long a, char *b, int c) { return nullptr; }
__attribute__((weak)) char* SDL_lltoa(Sint64 a, char *b, int c) { return nullptr; }
__attribute__((weak)) char* SDL_ulltoa(Uint64 a, char *b, int c) { return nullptr; }
__attribute__((weak)) int SDL_atoi(const char *a) { return 0; }
__attribute__((weak)) double SDL_atof(const char *a) { return 0.0; }
__attribute__((weak)) long SDL_strtol(const char *a, char **b, int c) { return 0; }
__attribute__((weak)) unsigned long SDL_strtoul(const char *a, char **b, int c) { return (unsigned long)0; }
__attribute__((weak)) Sint64 SDL_strtoll(const char *a, char **b, int c) { return 0; }
__attribute__((weak)) Uint64 SDL_strtoull(const char *a, char **b, int c) { return 0; }
__attribute__((weak)) double SDL_strtod(const char *a, char **b) { return 0.0; }
__attribute__((weak)) int SDL_strcmp(const char *a, const char *b) { return 0; }
__attribute__((weak)) int SDL_strncmp(const char *a, const char *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_strcasecmp(const char *a, const char *b) { return 0; }
__attribute__((weak)) int SDL_strncasecmp(const char *a, const char *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_vsnprintf(char *a, size_t b, const char *c, va_list d) { return 0; }
__attribute__((weak)) double SDL_acos(double a) { return 0.0; }
__attribute__((weak)) double SDL_asin(double a) { return 0.0; }
__attribute__((weak)) double SDL_atan(double a) { return 0.0; }
__attribute__((weak)) double SDL_atan2(double a, double b) { return 0.0; }
__attribute__((weak)) double SDL_ceil(double a) { return 0.0; }
__attribute__((weak)) double SDL_copysign(double a, double b) { return 0.0; }
__attribute__((weak)) double SDL_cos(double a) { return 0.0; }
__attribute__((weak)) float SDL_cosf(float a) { return 0.0f; }
__attribute__((weak)) double SDL_fabs(double a) { return 0.0; }
__attribute__((weak)) double SDL_floor(double a) { return 0.0; }
__attribute__((weak)) double SDL_log(double a) { return 0.0; }
__attribute__((weak)) double SDL_pow(double a, double b) { return 0.0; }
__attribute__((weak)) double SDL_scalbn(double a, int b) { return 0.0; }
__attribute__((weak)) double SDL_sin(double a) { return 0.0; }
__attribute__((weak)) float SDL_sinf(float a) { return 0.0f; }
__attribute__((weak)) double SDL_sqrt(double a) { return 0.0; }
__attribute__((weak)) SDL_iconv_t SDL_iconv_open(const char *a, const char *b) { return (SDL_iconv_t)0; }
__attribute__((weak)) int SDL_iconv_close(SDL_iconv_t a) { return 0; }
__attribute__((weak)) size_t SDL_iconv(SDL_iconv_t a, const char **b, size_t *c, char **d, size_t *e) { return 0; }
__attribute__((weak)) char* SDL_iconv_string(const char *a, const char *b, const char *c, size_t d) { return nullptr; }
__attribute__((weak)) SDL_Surface* SDL_CreateRGBSurface(Uint32 a, int b, int c, int d, Uint32 e, Uint32 f, Uint32 g, Uint32 h) { return nullptr; }
__attribute__((weak)) SDL_Surface* SDL_CreateRGBSurfaceFrom(void *a, int b, int c, int d, int e, Uint32 f, Uint32 g, Uint32 h, Uint32 i) { return nullptr; }
__attribute__((weak)) void SDL_FreeSurface(SDL_Surface *a) { return (void)0; }
__attribute__((weak)) int SDL_SetSurfacePalette(SDL_Surface *a, SDL_Palette *b) { return 0; }
__attribute__((weak)) int SDL_LockSurface(SDL_Surface *a) { return 0; }
__attribute__((weak)) void SDL_UnlockSurface(SDL_Surface *a) { return (void)0; }
__attribute__((weak)) SDL_Surface* SDL_LoadBMP_RW(SDL_RWops *a, int b) { return nullptr; }
__attribute__((weak)) int SDL_SaveBMP_RW(SDL_Surface *a, SDL_RWops *b, int c) { return 0; }
__attribute__((weak)) int SDL_SetSurfaceRLE(SDL_Surface *a, int b) { return 0; }
__attribute__((weak)) int SDL_SetColorKey(SDL_Surface *a, int b, Uint32 c) { return 0; }
__attribute__((weak)) int SDL_GetColorKey(SDL_Surface *a, Uint32 *b) { return 0; }
__attribute__((weak)) int SDL_SetSurfaceColorMod(SDL_Surface *a, Uint8 b, Uint8 c, Uint8 d) { return 0; }
__attribute__((weak)) int SDL_GetSurfaceColorMod(SDL_Surface *a, Uint8 *b, Uint8 *c, Uint8 *d) { return 0; }
__attribute__((weak)) int SDL_SetSurfaceAlphaMod(SDL_Surface *a, Uint8 b) { return 0; }
__attribute__((weak)) int SDL_GetSurfaceAlphaMod(SDL_Surface *a, Uint8 *b) { return 0; }
__attribute__((weak)) int SDL_SetSurfaceBlendMode(SDL_Surface *a, SDL_BlendMode b) { return 0; }
__attribute__((weak)) int SDL_GetSurfaceBlendMode(SDL_Surface *a, SDL_BlendMode *b) { return 0; }
__attribute__((weak)) SDL_bool SDL_SetClipRect(SDL_Surface *a, const SDL_Rect *b) { return (SDL_bool)0; }
__attribute__((weak)) void SDL_GetClipRect(SDL_Surface *a, SDL_Rect *b) { return (void)0; }
__attribute__((weak)) SDL_Surface* SDL_ConvertSurface(SDL_Surface *a, const SDL_PixelFormat *b, Uint32 c) { return nullptr; }
__attribute__((weak)) SDL_Surface* SDL_ConvertSurfaceFormat(SDL_Surface *a, Uint32 b, Uint32 c) { return nullptr; }
__attribute__((weak)) int SDL_ConvertPixels(int a, int b, Uint32 c, const void *d, int e, Uint32 f, void *g, int h) { return 0; }
__attribute__((weak)) int SDL_FillRect(SDL_Surface *a, const SDL_Rect *b, Uint32 c) { return 0; }
__attribute__((weak)) int SDL_FillRects(SDL_Surface *a, const SDL_Rect *b, int c, Uint32 d) { return 0; }
__attribute__((weak)) int SDL_UpperBlit(SDL_Surface *a, const SDL_Rect *b, SDL_Surface *c, SDL_Rect *d) { return 0; }
__attribute__((weak)) int SDL_LowerBlit(SDL_Surface *a, SDL_Rect *b, SDL_Surface *c, SDL_Rect *d) { return 0; }
__attribute__((weak)) int SDL_SoftStretch(SDL_Surface *a, const SDL_Rect *b, SDL_Surface *c, const SDL_Rect *d) { return 0; }
__attribute__((weak)) int SDL_UpperBlitScaled(SDL_Surface *a, const SDL_Rect *b, SDL_Surface *c, SDL_Rect *d) { return 0; }
__attribute__((weak)) int SDL_LowerBlitScaled(SDL_Surface *a, SDL_Rect *b, SDL_Surface *c, SDL_Rect *d) { return 0; }
__attribute__((weak)) SDL_bool SDL_GetWindowWMInfo(SDL_Window *a, SDL_SysWMinfo *b) { return (SDL_bool)0; }
__attribute__((weak)) const char* SDL_GetThreadName(SDL_Thread *a) { return nullptr; }
__attribute__((weak)) SDL_threadID SDL_ThreadID(void) { return (SDL_threadID)0; }
__attribute__((weak)) SDL_threadID SDL_GetThreadID(SDL_Thread *a) { return (SDL_threadID)0; }
__attribute__((weak)) int SDL_SetThreadPriority(SDL_ThreadPriority a) { return 0; }
__attribute__((weak)) void SDL_WaitThread(SDL_Thread *a, int *b) { return (void)0; }
__attribute__((weak)) void SDL_DetachThread(SDL_Thread *a) { return (void)0; }
__attribute__((weak)) SDL_TLSID SDL_TLSCreate(void) { return (SDL_TLSID)0; }
__attribute__((weak)) void* SDL_TLSGet(SDL_TLSID a) { return nullptr; }
__attribute__((weak)) int SDL_TLSSet(SDL_TLSID a, const void *b, void (SDLCALL *c)(void*)) { return 0; }
__attribute__((weak)) Uint32 SDL_GetTicks(void) { return 0; }
__attribute__((weak)) Uint64 SDL_GetPerformanceCounter(void) { return 0; }
__attribute__((weak)) Uint64 SDL_GetPerformanceFrequency(void) { return 0; }
__attribute__((weak)) void SDL_Delay(Uint32 a) { return (void)0; }
__attribute__((weak)) SDL_TimerID SDL_AddTimer(Uint32 a, SDL_TimerCallback b, void *c) { return (SDL_TimerID)0; }
__attribute__((weak)) SDL_bool SDL_RemoveTimer(SDL_TimerID a) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_GetNumTouchDevices(void) { return 0; }
__attribute__((weak)) SDL_TouchID SDL_GetTouchDevice(int a) { return (SDL_TouchID)0; }
__attribute__((weak)) int SDL_GetNumTouchFingers(SDL_TouchID a) { return 0; }
__attribute__((weak)) SDL_Finger* SDL_GetTouchFinger(SDL_TouchID a, int b) { return nullptr; }
__attribute__((weak)) void SDL_GetVersion(SDL_version *a) { return (void)0; }
__attribute__((weak)) const char* SDL_GetRevision(void) { return nullptr; }
__attribute__((weak)) int SDL_GetRevisionNumber(void) { return 0; }
__attribute__((weak)) int SDL_GetNumVideoDrivers(void) { return 0; }
__attribute__((weak)) const char* SDL_GetVideoDriver(int a) { return nullptr; }
__attribute__((weak)) int SDL_VideoInit(const char *a) { return 0; }
__attribute__((weak)) void SDL_VideoQuit(void) { return (void)0; }
__attribute__((weak)) const char* SDL_GetCurrentVideoDriver(void) { return nullptr; }
__attribute__((weak)) int SDL_GetNumVideoDisplays(void) { return 0; }
__attribute__((weak)) const char* SDL_GetDisplayName(int a) { return nullptr; }
__attribute__((weak)) int SDL_GetDisplayBounds(int a, SDL_Rect *b) { return 0; }
__attribute__((weak)) int SDL_GetNumDisplayModes(int a) { return 0; }
__attribute__((weak)) int SDL_GetDisplayMode(int a, int b, SDL_DisplayMode *c) { return 0; }
__attribute__((weak)) int SDL_GetDesktopDisplayMode(int a, SDL_DisplayMode *b) { return 0; }
__attribute__((weak)) int SDL_GetCurrentDisplayMode(int a, SDL_DisplayMode *b) { return 0; }
__attribute__((weak)) SDL_DisplayMode* SDL_GetClosestDisplayMode(int a, const SDL_DisplayMode *b, SDL_DisplayMode *c) { return nullptr; }
__attribute__((weak)) int SDL_GetWindowDisplayIndex(SDL_Window *a) { return 0; }
__attribute__((weak)) int SDL_SetWindowDisplayMode(SDL_Window *a, const SDL_DisplayMode *b) { return 0; }
__attribute__((weak)) int SDL_GetWindowDisplayMode(SDL_Window *a, SDL_DisplayMode *b) { return 0; }
__attribute__((weak)) Uint32 SDL_GetWindowPixelFormat(SDL_Window *a) { return 0; }
__attribute__((weak)) SDL_Window* SDL_CreateWindow(const char *a, int b, int c, int d, int e, Uint32 f) { return nullptr; }
__attribute__((weak)) SDL_Window* SDL_CreateWindowFrom(const void *a) { return nullptr; }
__attribute__((weak)) Uint32 SDL_GetWindowID(SDL_Window *a) { return 0; }
__attribute__((weak)) SDL_Window* SDL_GetWindowFromID(Uint32 a) { return nullptr; }
__attribute__((weak)) Uint32 SDL_GetWindowFlags(SDL_Window *a) { return 0; }
__attribute__((weak)) void SDL_SetWindowTitle(SDL_Window *a, const char *b) { return (void)0; }
__attribute__((weak)) const char* SDL_GetWindowTitle(SDL_Window *a) { return nullptr; }
__attribute__((weak)) void SDL_SetWindowIcon(SDL_Window *a, SDL_Surface *b) { return (void)0; }
__attribute__((weak)) void* SDL_SetWindowData(SDL_Window *a, const char *b, void *c) { return nullptr; }
__attribute__((weak)) void* SDL_GetWindowData(SDL_Window *a, const char *b) { return nullptr; }
__attribute__((weak)) void SDL_SetWindowPosition(SDL_Window *a, int b, int c) { return (void)0; }
__attribute__((weak)) void SDL_GetWindowPosition(SDL_Window *a, int *b, int *c) { return (void)0; }
__attribute__((weak)) void SDL_SetWindowSize(SDL_Window *a, int b, int c) { return (void)0; }
__attribute__((weak)) void SDL_GetWindowSize(SDL_Window *a, int *b, int *c) { return (void)0; }
__attribute__((weak)) void SDL_SetWindowMinimumSize(SDL_Window *a, int b, int c) { return (void)0; }
__attribute__((weak)) void SDL_GetWindowMinimumSize(SDL_Window *a, int *b, int *c) { return (void)0; }
__attribute__((weak)) void SDL_SetWindowMaximumSize(SDL_Window *a, int b, int c) { return (void)0; }
__attribute__((weak)) void SDL_GetWindowMaximumSize(SDL_Window *a, int *b, int *c) { return (void)0; }
__attribute__((weak)) void SDL_SetWindowBordered(SDL_Window *a, SDL_bool b) { return (void)0; }
__attribute__((weak)) void SDL_ShowWindow(SDL_Window *a) { return (void)0; }
__attribute__((weak)) void SDL_HideWindow(SDL_Window *a) { return (void)0; }
__attribute__((weak)) void SDL_RaiseWindow(SDL_Window *a) { return (void)0; }
__attribute__((weak)) void SDL_MaximizeWindow(SDL_Window *a) { return (void)0; }
__attribute__((weak)) void SDL_MinimizeWindow(SDL_Window *a) { return (void)0; }
__attribute__((weak)) void SDL_RestoreWindow(SDL_Window *a) { return (void)0; }
__attribute__((weak)) int SDL_SetWindowFullscreen(SDL_Window *a, Uint32 b) { return 0; }
__attribute__((weak)) SDL_Surface* SDL_GetWindowSurface(SDL_Window *a) { return nullptr; }
__attribute__((weak)) int SDL_UpdateWindowSurface(SDL_Window *a) { return 0; }
__attribute__((weak)) int SDL_UpdateWindowSurfaceRects(SDL_Window *a, const SDL_Rect *b, int c) { return 0; }
__attribute__((weak)) void SDL_SetWindowGrab(SDL_Window *a, SDL_bool b) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_GetWindowGrab(SDL_Window *a) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_SetWindowBrightness(SDL_Window *a, float b) { return 0; }
__attribute__((weak)) float SDL_GetWindowBrightness(SDL_Window *a) { return 0.0f; }
__attribute__((weak)) int SDL_SetWindowGammaRamp(SDL_Window *a, const Uint16 *b, const Uint16 *c, const Uint16 *d) { return 0; }
__attribute__((weak)) int SDL_GetWindowGammaRamp(SDL_Window *a, Uint16 *b, Uint16 *c, Uint16 *d) { return 0; }
__attribute__((weak)) void SDL_DestroyWindow(SDL_Window *a) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_IsScreenSaverEnabled(void) { return (SDL_bool)0; }
__attribute__((weak)) void SDL_EnableScreenSaver(void) { return (void)0; }
__attribute__((weak)) void SDL_DisableScreenSaver(void) { return (void)0; }
__attribute__((weak)) int SDL_GL_LoadLibrary(const char *a) { return 0; }
__attribute__((weak)) void* SDL_GL_GetProcAddress(const char *a) { return nullptr; }
__attribute__((weak)) void SDL_GL_UnloadLibrary(void) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_GL_ExtensionSupported(const char *a) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_GL_SetAttribute(SDL_GLattr a, int b) { return 0; }
__attribute__((weak)) int SDL_GL_GetAttribute(SDL_GLattr a, int *b) { return 0; }
__attribute__((weak)) SDL_GLContext SDL_GL_CreateContext(SDL_Window *a) { return (SDL_GLContext)0; }
__attribute__((weak)) int SDL_GL_MakeCurrent(SDL_Window *a, SDL_GLContext b) { return 0; }
__attribute__((weak)) SDL_Window* SDL_GL_GetCurrentWindow(void) { return nullptr; }
__attribute__((weak)) SDL_GLContext SDL_GL_GetCurrentContext(void) { return (SDL_GLContext)0; }
__attribute__((weak)) void SDL_GL_GetDrawableSize(SDL_Window *a, int *b, int *c) { return (void)0; }
__attribute__((weak)) int SDL_GL_SetSwapInterval(int a) { return 0; }
__attribute__((weak)) int SDL_GL_GetSwapInterval(void) { return 0; }
__attribute__((weak)) void SDL_GL_SwapWindow(SDL_Window *a) { return (void)0; }
__attribute__((weak)) void SDL_GL_DeleteContext(SDL_GLContext a) { return (void)0; }
__attribute__((weak)) int SDL_vsscanf(const char *a, const char *b, va_list c) { return 0; }
__attribute__((weak)) int SDL_GameControllerAddMappingsFromRW(SDL_RWops *a, int b) { return 0; }
__attribute__((weak)) void SDL_GL_ResetAttributes(void) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_HasAVX(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_AssertionHandler SDL_GetDefaultAssertionHandler(void) { return (SDL_AssertionHandler)0; }
__attribute__((weak)) SDL_AssertionHandler SDL_GetAssertionHandler(void **a) { return (SDL_AssertionHandler)0; }
__attribute__((weak)) SDL_bool SDL_DXGIGetOutputInfo(int a,int *b, int *c) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_RenderIsClipEnabled(SDL_Renderer *a) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_WinRTRunApp(SDL_main_func a, void *b) { return 0; }
__attribute__((weak)) int SDL_WarpMouseGlobal(int a, int b) { return 0; }
__attribute__((weak)) float SDL_sqrtf(float a) { return 0.0f; }
__attribute__((weak)) double SDL_tan(double a) { return 0.0; }
__attribute__((weak)) float SDL_tanf(float a) { return 0.0f; }
__attribute__((weak)) int SDL_CaptureMouse(SDL_bool a) { return 0; }
__attribute__((weak)) int SDL_SetWindowHitTest(SDL_Window *a, SDL_HitTest b, void *c) { return 0; }
__attribute__((weak)) Uint32 SDL_GetGlobalMouseState(int *a, int *b) { return 0; }
__attribute__((weak)) SDL_bool SDL_HasAVX2(void) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_QueueAudio(SDL_AudioDeviceID a, const void *b, Uint32 c) { return 0; }
__attribute__((weak)) Uint32 SDL_GetQueuedAudioSize(SDL_AudioDeviceID a) { return 0; }
__attribute__((weak)) void SDL_ClearQueuedAudio(SDL_AudioDeviceID a) { return (void)0; }
__attribute__((weak)) SDL_Window* SDL_GetGrabbedWindow(void) { return nullptr; }
__attribute__((weak)) void SDL_SetWindowsMessageHook(SDL_WindowsMessageHook a, void *b) { return (void)0; }
__attribute__((weak)) int SDL_GetDisplayDPI(int a, float *b, float *c, float *d) { return 0; }
__attribute__((weak)) SDL_JoystickPowerLevel SDL_JoystickCurrentPowerLevel(SDL_Joystick *a) { return (SDL_JoystickPowerLevel)0; }
__attribute__((weak)) SDL_GameController* SDL_GameControllerFromInstanceID(SDL_JoystickID a) { return nullptr; }
__attribute__((weak)) SDL_Joystick* SDL_JoystickFromInstanceID(SDL_JoystickID a) { return nullptr; }
__attribute__((weak)) int SDL_GetDisplayUsableBounds(int a, SDL_Rect *b) { return 0; }
__attribute__((weak)) int SDL_GetWindowBordersSize(SDL_Window *a, int *b, int *c, int *d, int *e) { return 0; }
__attribute__((weak)) int SDL_SetWindowOpacity(SDL_Window *a, float b) { return 0; }
__attribute__((weak)) int SDL_GetWindowOpacity(SDL_Window *a, float *b) { return 0; }
__attribute__((weak)) int SDL_SetWindowInputFocus(SDL_Window *a) { return 0; }
__attribute__((weak)) int SDL_SetWindowModalFor(SDL_Window *a, SDL_Window *b) { return 0; }
__attribute__((weak)) int SDL_RenderSetIntegerScale(SDL_Renderer *a, SDL_bool b) { return 0; }
__attribute__((weak)) SDL_bool SDL_RenderGetIntegerScale(SDL_Renderer *a) { return (SDL_bool)0; }
__attribute__((weak)) Uint32 SDL_DequeueAudio(SDL_AudioDeviceID a, void *b, Uint32 c) { return 0; }
__attribute__((weak)) void SDL_SetWindowResizable(SDL_Window *a, SDL_bool b) { return (void)0; }
__attribute__((weak)) SDL_Surface* SDL_CreateRGBSurfaceWithFormat(Uint32 a, int b, int c, int d, Uint32 e) { return nullptr; }
__attribute__((weak)) SDL_Surface* SDL_CreateRGBSurfaceWithFormatFrom(void *a, int b, int c, int d, int e, Uint32 f) { return nullptr; }
__attribute__((weak)) SDL_bool SDL_GetHintBoolean(const char *a, SDL_bool b) { return (SDL_bool)0; }
__attribute__((weak)) Uint16 SDL_JoystickGetDeviceVendor(int a) { return 0; }
__attribute__((weak)) Uint16 SDL_JoystickGetDeviceProduct(int a) { return 0; }
__attribute__((weak)) Uint16 SDL_JoystickGetDeviceProductVersion(int a) { return 0; }
__attribute__((weak)) Uint16 SDL_JoystickGetVendor(SDL_Joystick *a) { return 0; }
__attribute__((weak)) Uint16 SDL_JoystickGetProduct(SDL_Joystick *a) { return 0; }
__attribute__((weak)) Uint16 SDL_JoystickGetProductVersion(SDL_Joystick *a) { return 0; }
__attribute__((weak)) Uint16 SDL_GameControllerGetVendor(SDL_GameController *a) { return 0; }
__attribute__((weak)) Uint16 SDL_GameControllerGetProduct(SDL_GameController *a) { return 0; }
__attribute__((weak)) Uint16 SDL_GameControllerGetProductVersion(SDL_GameController *a) { return 0; }
__attribute__((weak)) SDL_bool SDL_HasNEON(void) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_GameControllerNumMappings(void) { return 0; }
__attribute__((weak)) char* SDL_GameControllerMappingForIndex(int a) { return nullptr; }
__attribute__((weak)) SDL_bool SDL_JoystickGetAxisInitialState(SDL_Joystick *a, int b, Sint16 *c) { return (SDL_bool)0; }
__attribute__((weak)) SDL_JoystickType SDL_JoystickGetDeviceType(int a) { return (SDL_JoystickType)0; }
__attribute__((weak)) SDL_JoystickType SDL_JoystickGetType(SDL_Joystick *a) { return (SDL_JoystickType)0; }
__attribute__((weak)) void SDL_MemoryBarrierReleaseFunction(void) { return (void)0; }
__attribute__((weak)) void SDL_MemoryBarrierAcquireFunction(void) { return (void)0; }
__attribute__((weak)) SDL_JoystickID SDL_JoystickGetDeviceInstanceID(int a) { return (SDL_JoystickID)0; }
__attribute__((weak)) size_t SDL_utf8strlen(const char *a) { return 0; }
__attribute__((weak)) void* SDL_LoadFile_RW(SDL_RWops *a, size_t *b, int c) { return nullptr; }
__attribute__((weak)) int SDL_wcscmp(const wchar_t *a, const wchar_t *b) { return 0; }
__attribute__((weak)) SDL_BlendMode SDL_ComposeCustomBlendMode(SDL_BlendFactor a, SDL_BlendFactor b, SDL_BlendOperation c, SDL_BlendFactor d, SDL_BlendFactor e, SDL_BlendOperation f) { return (SDL_BlendMode)0; }
__attribute__((weak)) SDL_Surface* SDL_DuplicateSurface(SDL_Surface *a) { return nullptr; }
__attribute__((weak)) int SDL_Vulkan_LoadLibrary(const char *a) { return 0; }
__attribute__((weak)) void* SDL_Vulkan_GetVkGetInstanceProcAddr(void) { return nullptr; }
__attribute__((weak)) void SDL_Vulkan_UnloadLibrary(void) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_Vulkan_GetInstanceExtensions(SDL_Window *a, unsigned int *b, const char **c) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_Vulkan_CreateSurface(SDL_Window *a, VkInstance b, VkSurfaceKHR *c) { return (SDL_bool)0; }
__attribute__((weak)) void SDL_Vulkan_GetDrawableSize(SDL_Window *a, int *b, int *c) { return (void)0; }
__attribute__((weak)) void SDL_LockJoysticks(void) { return (void)0; }
__attribute__((weak)) void SDL_UnlockJoysticks(void) { return (void)0; }
__attribute__((weak)) void SDL_GetMemoryFunctions(SDL_malloc_func *a, SDL_calloc_func *b, SDL_realloc_func *c, SDL_free_func *d) { return (void)0; }
__attribute__((weak)) int SDL_SetMemoryFunctions(SDL_malloc_func a, SDL_calloc_func b, SDL_realloc_func c, SDL_free_func d) { return 0; }
__attribute__((weak)) int SDL_GetNumAllocations(void) { return 0; }
__attribute__((weak)) SDL_AudioStream* SDL_NewAudioStream(const SDL_AudioFormat a, const Uint8 b, const int c, const SDL_AudioFormat d, const Uint8 e, const int f) { return nullptr; }
__attribute__((weak)) int SDL_AudioStreamPut(SDL_AudioStream *a, const void *b, int c) { return 0; }
__attribute__((weak)) int SDL_AudioStreamGet(SDL_AudioStream *a, void *b, int c) { return 0; }
__attribute__((weak)) void SDL_AudioStreamClear(SDL_AudioStream *a) { return (void)0; }
__attribute__((weak)) int SDL_AudioStreamAvailable(SDL_AudioStream *a) { return 0; }
__attribute__((weak)) void SDL_FreeAudioStream(SDL_AudioStream *a) { return (void)0; }
__attribute__((weak)) int SDL_AudioStreamFlush(SDL_AudioStream *a) { return 0; }
__attribute__((weak)) float SDL_acosf(float a) { return 0.0f; }
__attribute__((weak)) float SDL_asinf(float a) { return 0.0f; }
__attribute__((weak)) float SDL_atanf(float a) { return 0.0f; }
__attribute__((weak)) float SDL_atan2f(float a, float b) { return 0.0f; }
__attribute__((weak)) float SDL_ceilf(float a) { return 0.0f; }
__attribute__((weak)) float SDL_copysignf(float a, float b) { return 0.0f; }
__attribute__((weak)) float SDL_fabsf(float a) { return 0.0f; }
__attribute__((weak)) float SDL_floorf(float a) { return 0.0f; }
__attribute__((weak)) float SDL_logf(float a) { return 0.0f; }
__attribute__((weak)) float SDL_powf(float a, float b) { return 0.0f; }
__attribute__((weak)) float SDL_scalbnf(float a, int b) { return 0.0f; }
__attribute__((weak)) double SDL_fmod(double a, double b) { return 0.0; }
__attribute__((weak)) float SDL_fmodf(float a, float b) { return 0.0f; }
__attribute__((weak)) void SDL_SetYUVConversionMode(SDL_YUV_CONVERSION_MODE a) { return (void)0; }
__attribute__((weak)) SDL_YUV_CONVERSION_MODE SDL_GetYUVConversionMode(void) { return (SDL_YUV_CONVERSION_MODE)0; }
__attribute__((weak)) SDL_YUV_CONVERSION_MODE SDL_GetYUVConversionModeForResolution(int a, int b) { return (SDL_YUV_CONVERSION_MODE)0; }
__attribute__((weak)) void* SDL_RenderGetMetalLayer(SDL_Renderer *a) { return nullptr; }
__attribute__((weak)) void* SDL_RenderGetMetalCommandEncoder(SDL_Renderer *a) { return nullptr; }
__attribute__((weak)) SDL_bool SDL_IsAndroidTV(void) { return (SDL_bool)0; }
__attribute__((weak)) double SDL_log10(double a) { return 0.0; }
__attribute__((weak)) float SDL_log10f(float a) { return 0.0f; }
__attribute__((weak)) char* SDL_GameControllerMappingForDeviceIndex(int a) { return nullptr; }
__attribute__((weak)) int SDL_LinuxSetThreadPriority(Sint64 a, int b) { return 0; }
__attribute__((weak)) SDL_bool SDL_HasAVX512F(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_IsChromebook(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_IsDeXMode(void) { return (SDL_bool)0; }
__attribute__((weak)) void SDL_AndroidBackButton(void) { return (void)0; }
__attribute__((weak)) double SDL_exp(double a) { return 0.0; }
__attribute__((weak)) float SDL_expf(float a) { return 0.0f; }
__attribute__((weak)) wchar_t* SDL_wcsdup(const wchar_t *a) { return nullptr; }
__attribute__((weak)) int SDL_GameControllerRumble(SDL_GameController *a, Uint16 b, Uint16 c, Uint32 d) { return 0; }
__attribute__((weak)) int SDL_JoystickRumble(SDL_Joystick *a, Uint16 b, Uint16 c, Uint32 d) { return 0; }
__attribute__((weak)) int SDL_NumSensors(void) { return 0; }
__attribute__((weak)) const char* SDL_SensorGetDeviceName(int a) { return nullptr; }
__attribute__((weak)) SDL_SensorType SDL_SensorGetDeviceType(int a) { return (SDL_SensorType)0; }
__attribute__((weak)) int SDL_SensorGetDeviceNonPortableType(int a) { return 0; }
__attribute__((weak)) SDL_SensorID SDL_SensorGetDeviceInstanceID(int a) { return (SDL_SensorID)0; }
__attribute__((weak)) SDL_Sensor* SDL_SensorOpen(int a) { return nullptr; }
__attribute__((weak)) SDL_Sensor* SDL_SensorFromInstanceID(SDL_SensorID a) { return nullptr; }
__attribute__((weak)) const char* SDL_SensorGetName(SDL_Sensor *a) { return nullptr; }
__attribute__((weak)) SDL_SensorType SDL_SensorGetType(SDL_Sensor *a) { return (SDL_SensorType)0; }
__attribute__((weak)) int SDL_SensorGetNonPortableType(SDL_Sensor *a) { return 0; }
__attribute__((weak)) SDL_SensorID SDL_SensorGetInstanceID(SDL_Sensor *a) { return (SDL_SensorID)0; }
__attribute__((weak)) int SDL_SensorGetData(SDL_Sensor *a, float *b, int c) { return 0; }
__attribute__((weak)) void SDL_SensorClose(SDL_Sensor *a) { return (void)0; }
__attribute__((weak)) void SDL_SensorUpdate(void) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_IsTablet(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_DisplayOrientation SDL_GetDisplayOrientation(int a) { return (SDL_DisplayOrientation)0; }
__attribute__((weak)) SDL_bool SDL_HasColorKey(SDL_Surface *a) { return (SDL_bool)0; }
__attribute__((weak)) SDL_Thread* SDL_CreateThreadWithStackSize(SDL_ThreadFunction a, const char *b, const size_t c, void *d) { return nullptr; }
__attribute__((weak)) int SDL_JoystickGetDevicePlayerIndex(int a) { return 0; }
__attribute__((weak)) int SDL_JoystickGetPlayerIndex(SDL_Joystick *a) { return 0; }
__attribute__((weak)) int SDL_GameControllerGetPlayerIndex(SDL_GameController *a) { return 0; }
__attribute__((weak)) int SDL_RenderFlush(SDL_Renderer *a) { return 0; }
__attribute__((weak)) int SDL_RenderDrawPointF(SDL_Renderer *a, float b, float c) { return 0; }
__attribute__((weak)) int SDL_RenderDrawPointsF(SDL_Renderer *a, const SDL_FPoint *b, int c) { return 0; }
__attribute__((weak)) int SDL_RenderDrawLineF(SDL_Renderer *a, float b, float c, float d, float e) { return 0; }
__attribute__((weak)) int SDL_RenderDrawLinesF(SDL_Renderer *a, const SDL_FPoint *b, int c) { return 0; }
__attribute__((weak)) int SDL_RenderDrawRectF(SDL_Renderer *a, const SDL_FRect *b) { return 0; }
__attribute__((weak)) int SDL_RenderDrawRectsF(SDL_Renderer *a, const SDL_FRect *b, int c) { return 0; }
__attribute__((weak)) int SDL_RenderFillRectF(SDL_Renderer *a, const SDL_FRect *b) { return 0; }
__attribute__((weak)) int SDL_RenderFillRectsF(SDL_Renderer *a, const SDL_FRect *b, int c) { return 0; }
__attribute__((weak)) int SDL_RenderCopyF(SDL_Renderer *a, SDL_Texture *b, const SDL_Rect *c, const SDL_FRect *d) { return 0; }
__attribute__((weak)) int SDL_RenderCopyExF(SDL_Renderer *a, SDL_Texture *b, const SDL_Rect *c, const SDL_FRect *d, const double e, const SDL_FPoint *f, const SDL_RendererFlip g) { return 0; }
__attribute__((weak)) SDL_TouchDeviceType SDL_GetTouchDeviceType(SDL_TouchID a) { return (SDL_TouchDeviceType)0; }
__attribute__((weak)) int SDL_UIKitRunApp(int a, char *b[], SDL_main_func c) { return 0; }
__attribute__((weak)) size_t SDL_SIMDGetAlignment(void) { return 0; }
__attribute__((weak)) void* SDL_SIMDAlloc(const size_t a) { return nullptr; }
__attribute__((weak)) void SDL_SIMDFree(void *a) { return (void)0; }
__attribute__((weak)) Sint64 SDL_RWsize(SDL_RWops *a) { return 0; }
__attribute__((weak)) Sint64 SDL_RWseek(SDL_RWops *a, Sint64 b, int c) { return 0; }
__attribute__((weak)) Sint64 SDL_RWtell(SDL_RWops *a) { return 0; }
__attribute__((weak)) size_t SDL_RWread(SDL_RWops *a, void *b, size_t c, size_t d) { return 0; }
__attribute__((weak)) size_t SDL_RWwrite(SDL_RWops *a, const void *b, size_t c, size_t d) { return 0; }
__attribute__((weak)) int SDL_RWclose(SDL_RWops *a) { return 0; }
__attribute__((weak)) void* SDL_LoadFile(const char *a, size_t *b) { return nullptr; }
__attribute__((weak)) SDL_MetalView SDL_Metal_CreateView(SDL_Window *a) { return (SDL_MetalView)0; }
__attribute__((weak)) void SDL_Metal_DestroyView(SDL_MetalView a) { return (void)0; }
__attribute__((weak)) int SDL_LockTextureToSurface(SDL_Texture *a, const SDL_Rect *b, SDL_Surface **c) { return 0; }
__attribute__((weak)) SDL_bool SDL_HasARMSIMD(void) { return (SDL_bool)0; }
__attribute__((weak)) char* SDL_strtokr(char *a, const char *b, char **c) { return nullptr; }
__attribute__((weak)) wchar_t* SDL_wcsstr(const wchar_t *a, const wchar_t *b) { return nullptr; }
__attribute__((weak)) int SDL_wcsncmp(const wchar_t *a, const wchar_t *b, size_t c) { return 0; }
__attribute__((weak)) SDL_GameControllerType SDL_GameControllerTypeForIndex(int a) { return (SDL_GameControllerType)0; }
__attribute__((weak)) SDL_GameControllerType SDL_GameControllerGetType(SDL_GameController *a) { return (SDL_GameControllerType)0; }
__attribute__((weak)) SDL_GameController* SDL_GameControllerFromPlayerIndex(int a) { return nullptr; }
__attribute__((weak)) void SDL_GameControllerSetPlayerIndex(SDL_GameController *a, int b) { return (void)0; }
__attribute__((weak)) SDL_Joystick* SDL_JoystickFromPlayerIndex(int a) { return nullptr; }
__attribute__((weak)) void SDL_JoystickSetPlayerIndex(SDL_Joystick *a, int b) { return (void)0; }
__attribute__((weak)) int SDL_SetTextureScaleMode(SDL_Texture *a, SDL_ScaleMode b) { return 0; }
__attribute__((weak)) int SDL_GetTextureScaleMode(SDL_Texture *a, SDL_ScaleMode *b) { return 0; }
__attribute__((weak)) void SDL_OnApplicationWillTerminate(void) { return (void)0; }
__attribute__((weak)) void SDL_OnApplicationDidReceiveMemoryWarning(void) { return (void)0; }
__attribute__((weak)) void SDL_OnApplicationWillResignActive(void) { return (void)0; }
__attribute__((weak)) void SDL_OnApplicationDidEnterBackground(void) { return (void)0; }
__attribute__((weak)) void SDL_OnApplicationWillEnterForeground(void) { return (void)0; }
__attribute__((weak)) void SDL_OnApplicationDidBecomeActive(void) { return (void)0; }
__attribute__((weak)) void SDL_OnApplicationDidChangeStatusBarOrientation(void) { return (void)0; }
__attribute__((weak)) int SDL_GetAndroidSDKVersion(void) { return 0; }
__attribute__((weak)) int SDL_isupper(int a) { return 0; }
__attribute__((weak)) int SDL_islower(int a) { return 0; }
__attribute__((weak)) int SDL_JoystickAttachVirtual(SDL_JoystickType a, int b, int c, int d) { return 0; }
__attribute__((weak)) int SDL_JoystickDetachVirtual(int a) { return 0; }
__attribute__((weak)) SDL_bool SDL_JoystickIsVirtual(int a) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_JoystickSetVirtualAxis(SDL_Joystick *a, int b, Sint16 c) { return 0; }
__attribute__((weak)) int SDL_JoystickSetVirtualButton(SDL_Joystick *a, int b, Uint8 c) { return 0; }
__attribute__((weak)) int SDL_JoystickSetVirtualHat(SDL_Joystick *a, int b, Uint8 c) { return 0; }
__attribute__((weak)) char* SDL_GetErrorMsg(char *a, int b) { return nullptr; }
__attribute__((weak)) void SDL_LockSensors(void) { return (void)0; }
__attribute__((weak)) void SDL_UnlockSensors(void) { return (void)0; }
__attribute__((weak)) void* SDL_Metal_GetLayer(SDL_MetalView a) { return nullptr; }
__attribute__((weak)) void SDL_Metal_GetDrawableSize(SDL_Window *a, int *b, int *c) { return (void)0; }
__attribute__((weak)) double SDL_trunc(double a) { return 0.0; }
__attribute__((weak)) float SDL_truncf(float a) { return 0.0f; }
__attribute__((weak)) SDL_Locale * SDL_GetPreferredLocales(void) { return nullptr; }
__attribute__((weak)) void* SDL_SIMDRealloc(void *a, const size_t b) { return nullptr; }
__attribute__((weak)) SDL_bool SDL_AndroidRequestPermission(const char *a) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_OpenURL(const char *a) { return 0; }
__attribute__((weak)) SDL_bool SDL_HasSurfaceRLE(SDL_Surface *a) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_GameControllerHasLED(SDL_GameController *a) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_GameControllerSetLED(SDL_GameController *a, Uint8 b, Uint8 c, Uint8 d) { return 0; }
__attribute__((weak)) SDL_bool SDL_JoystickHasLED(SDL_Joystick *a) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_JoystickSetLED(SDL_Joystick *a, Uint8 b, Uint8 c, Uint8 d) { return 0; }
__attribute__((weak)) int SDL_GameControllerRumbleTriggers(SDL_GameController *a, Uint16 b, Uint16 c, Uint32 d) { return 0; }
__attribute__((weak)) int SDL_JoystickRumbleTriggers(SDL_Joystick *a, Uint16 b, Uint16 c, Uint32 d) { return 0; }
__attribute__((weak)) SDL_bool SDL_GameControllerHasAxis(SDL_GameController *a, SDL_GameControllerAxis b) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_GameControllerHasButton(SDL_GameController *a, SDL_GameControllerButton b) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_GameControllerGetNumTouchpads(SDL_GameController *a) { return 0; }
__attribute__((weak)) int SDL_GameControllerGetNumTouchpadFingers(SDL_GameController *a, int b) { return 0; }
__attribute__((weak)) int SDL_GameControllerGetTouchpadFinger(SDL_GameController *a, int b, int c, Uint8 *d, float *e, float *f, float *g) { return 0; }
__attribute__((weak)) Uint32 SDL_crc32(Uint32 a, const void *b, size_t c) { return 0; }
__attribute__((weak)) const char* SDL_GameControllerGetSerial(SDL_GameController *a) { return nullptr; }
__attribute__((weak)) const char* SDL_JoystickGetSerial(SDL_Joystick *a) { return nullptr; }
__attribute__((weak)) SDL_bool SDL_GameControllerHasSensor(SDL_GameController *a, SDL_SensorType b) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_GameControllerSetSensorEnabled(SDL_GameController *a, SDL_SensorType b, SDL_bool c) { return 0; }
__attribute__((weak)) SDL_bool SDL_GameControllerIsSensorEnabled(SDL_GameController *a, SDL_SensorType b) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_GameControllerGetSensorData(SDL_GameController *a, SDL_SensorType b, float *c, int d) { return 0; }
__attribute__((weak)) int SDL_wcscasecmp(const wchar_t *a, const wchar_t *b) { return 0; }
__attribute__((weak)) int SDL_wcsncasecmp(const wchar_t *a, const wchar_t *b, size_t c) { return 0; }
__attribute__((weak)) double SDL_round(double a) { return 0.0; }
__attribute__((weak)) float SDL_roundf(float a) { return 0.0f; }
__attribute__((weak)) long SDL_lround(double a) { return 0; }
__attribute__((weak)) long SDL_lroundf(float a) { return 0; }
__attribute__((weak)) int SDL_SoftStretchLinear(SDL_Surface *a, const SDL_Rect *b, SDL_Surface *c, const SDL_Rect *d) { return 0; }
__attribute__((weak)) int SDL_UpdateNVTexture(SDL_Texture *a, const SDL_Rect *b, const Uint8 *c, int d, const Uint8 *e, int f) { return 0; }
__attribute__((weak)) void SDL_SetWindowKeyboardGrab(SDL_Window *a, SDL_bool b) { return (void)0; }
__attribute__((weak)) void SDL_SetWindowMouseGrab(SDL_Window *a, SDL_bool b) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_GetWindowKeyboardGrab(SDL_Window *a) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_GetWindowMouseGrab(SDL_Window *a) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_isalpha(int a) { return 0; }
__attribute__((weak)) int SDL_isalnum(int a) { return 0; }
__attribute__((weak)) int SDL_isblank(int a) { return 0; }
__attribute__((weak)) int SDL_iscntrl(int a) { return 0; }
__attribute__((weak)) int SDL_isxdigit(int a) { return 0; }
__attribute__((weak)) int SDL_ispunct(int a) { return 0; }
__attribute__((weak)) int SDL_isprint(int a) { return 0; }
__attribute__((weak)) int SDL_isgraph(int a) { return 0; }
__attribute__((weak)) int SDL_AndroidShowToast(const char *a, int b, int c, int d, int e) { return 0; }
__attribute__((weak)) int SDL_GetAudioDeviceSpec(int a, int b, SDL_AudioSpec *c) { return 0; }
__attribute__((weak)) void SDL_TLSCleanup(void) { return (void)0; }
__attribute__((weak)) void SDL_SetWindowAlwaysOnTop(SDL_Window *a, SDL_bool b) { return (void)0; }
__attribute__((weak)) int SDL_FlashWindow(SDL_Window *a, SDL_FlashOperation b) { return 0; }
__attribute__((weak)) int SDL_GameControllerSendEffect(SDL_GameController *a, const void *b, int c) { return 0; }
__attribute__((weak)) int SDL_JoystickSendEffect(SDL_Joystick *a, const void *b, int c) { return 0; }
__attribute__((weak)) float SDL_GameControllerGetSensorDataRate(SDL_GameController *a, SDL_SensorType b) { return 0.0f; }
__attribute__((weak)) int SDL_SetTextureUserData(SDL_Texture *a, void *b) { return 0; }
__attribute__((weak)) void* SDL_GetTextureUserData(SDL_Texture *a) { return nullptr; }
__attribute__((weak)) int SDL_RenderGeometry(SDL_Renderer *a, SDL_Texture *b, const SDL_Vertex *c, int d, const int *e, int f) { return 0; }
__attribute__((weak)) int SDL_RenderGeometryRaw(SDL_Renderer *a, SDL_Texture *b, const float *c, int d, const SDL_Color *e, int f, const float *g, int h, int i, const void *j, int k, int l) { return 0; }
__attribute__((weak)) int SDL_RenderSetVSync(SDL_Renderer *a, int b) { return 0; }
__attribute__((weak)) int SDL_asprintf(char **a,  const char *b, ...) { return 0; }
__attribute__((weak)) int SDL_vasprintf(char **a, const char *b, va_list c) { return 0; }
__attribute__((weak)) void* SDL_GetWindowICCProfile(SDL_Window *a, size_t *b) { return nullptr; }
__attribute__((weak)) Uint64 SDL_GetTicks64(void) { return 0; }
__attribute__((weak)) int SDL_LinuxSetThreadPriorityAndPolicy(Sint64 a, int b, int c) { return 0; }
__attribute__((weak)) const char* SDL_GameControllerGetAppleSFSymbolsNameForButton(SDL_GameController *a, SDL_GameControllerButton b) { return nullptr; }
__attribute__((weak)) const char* SDL_GameControllerGetAppleSFSymbolsNameForAxis(SDL_GameController *a, SDL_GameControllerAxis b) { return nullptr; }
__attribute__((weak)) int SDL_hid_init(void) { return 0; }
__attribute__((weak)) int SDL_hid_exit(void) { return 0; }
__attribute__((weak)) Uint32 SDL_hid_device_change_count(void) { return 0; }
__attribute__((weak)) SDL_hid_device_info* SDL_hid_enumerate(unsigned short a, unsigned short b) { return nullptr; }
__attribute__((weak)) void SDL_hid_free_enumeration(SDL_hid_device_info *a) { return (void)0; }
__attribute__((weak)) SDL_hid_device* SDL_hid_open(unsigned short a, unsigned short b, const wchar_t *c) { return nullptr; }
__attribute__((weak)) SDL_hid_device* SDL_hid_open_path(const char *a, int b) { return nullptr; }
__attribute__((weak)) int SDL_hid_write(SDL_hid_device *a, const unsigned char *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_hid_read_timeout(SDL_hid_device *a, unsigned char *b, size_t c, int d) { return 0; }
__attribute__((weak)) int SDL_hid_read(SDL_hid_device *a, unsigned char *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_hid_set_nonblocking(SDL_hid_device *a, int b) { return 0; }
__attribute__((weak)) int SDL_hid_send_feature_report(SDL_hid_device *a, const unsigned char *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_hid_get_feature_report(SDL_hid_device *a, unsigned char *b, size_t c) { return 0; }
__attribute__((weak)) void SDL_hid_close(SDL_hid_device *a) { return (void)0; }
__attribute__((weak)) int SDL_hid_get_manufacturer_string(SDL_hid_device *a, wchar_t *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_hid_get_product_string(SDL_hid_device *a, wchar_t *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_hid_get_serial_number_string(SDL_hid_device *a, wchar_t *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_hid_get_indexed_string(SDL_hid_device *a, int b, wchar_t *c, size_t d) { return 0; }
__attribute__((weak)) int SDL_SetWindowMouseRect(SDL_Window *a, const SDL_Rect *b) { return 0; }
__attribute__((weak)) const SDL_Rect* SDL_GetWindowMouseRect(SDL_Window *a) { return nullptr; }
__attribute__((weak)) void SDL_RenderWindowToLogical(SDL_Renderer *a, int b, int c, float *d, float *e) { return (void)0; }
__attribute__((weak)) void SDL_RenderLogicalToWindow(SDL_Renderer *a, float b, float c, int *d, int *e) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_JoystickHasRumble(SDL_Joystick *a) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_JoystickHasRumbleTriggers(SDL_Joystick *a) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_GameControllerHasRumble(SDL_GameController *a) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_GameControllerHasRumbleTriggers(SDL_GameController *a) { return (SDL_bool)0; }
__attribute__((weak)) void SDL_hid_ble_scan(SDL_bool a) { return (void)0; }
__attribute__((weak)) int SDL_PremultiplyAlpha(int a, int b, Uint32 c, const void *d, int e, Uint32 f, void *g, int h) { return 0; }
__attribute__((weak)) int SDL_AndroidSendMessage(Uint32 a, int b) { return 0; }
__attribute__((weak)) const char* SDL_GetTouchName(int a) { return nullptr; }
__attribute__((weak)) void SDL_ClearComposition(void) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_IsTextInputShown(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_HasIntersectionF(const SDL_FRect *a, const SDL_FRect *b) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_IntersectFRect(const SDL_FRect *a, const SDL_FRect *b, SDL_FRect *c) { return (SDL_bool)0; }
__attribute__((weak)) void SDL_UnionFRect(const SDL_FRect *a, const SDL_FRect *b, SDL_FRect *c) { return (void)0; }
__attribute__((weak)) SDL_bool SDL_EncloseFPoints(const SDL_FPoint *a, int b, const SDL_FRect *c, SDL_FRect *d) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_IntersectFRectAndLine(const SDL_FRect *a, float *b, float *c, float *d, float *e) { return (SDL_bool)0; }
__attribute__((weak)) SDL_Window* SDL_RenderGetWindow(SDL_Renderer *a) { return nullptr; }
__attribute__((weak)) void* SDL_bsearch(const void *a, const void *b, size_t c, size_t d, int (SDLCALL *e)(const void *, const void *)) { return nullptr; }
__attribute__((weak)) const char* SDL_GameControllerPathForIndex(int a) { return nullptr; }
__attribute__((weak)) const char* SDL_GameControllerPath(SDL_GameController *a) { return nullptr; }
__attribute__((weak)) const char* SDL_JoystickPathForIndex(int a) { return nullptr; }
__attribute__((weak)) const char* SDL_JoystickPath(SDL_Joystick *a) { return nullptr; }
__attribute__((weak)) int SDL_JoystickAttachVirtualEx(const SDL_VirtualJoystickDesc *a) { return 0; }
__attribute__((weak)) Uint16 SDL_GameControllerGetFirmwareVersion(SDL_GameController *a) { return 0; }
__attribute__((weak)) Uint16 SDL_JoystickGetFirmwareVersion(SDL_Joystick *a) { return 0; }
__attribute__((weak)) void SDL_GUIDToString(SDL_GUID a, char *b, int c) { return (void)0; }
__attribute__((weak)) SDL_GUID SDL_GUIDFromString(const char *a) { return {}; }
__attribute__((weak)) SDL_bool SDL_HasLSX(void) { return (SDL_bool)0; }
__attribute__((weak)) SDL_bool SDL_HasLASX(void) { return (SDL_bool)0; }
__attribute__((weak)) size_t SDL_utf8strnlen(const char *a, size_t b) { return 0; }
__attribute__((weak)) int SDL_GDKRunApp(SDL_main_func a, void *b) { return 0; }
__attribute__((weak)) void SDL_GetOriginalMemoryFunctions(SDL_malloc_func *a, SDL_calloc_func *b, SDL_realloc_func *c, SDL_free_func *d) { return (void)0; }
__attribute__((weak)) void SDL_ResetKeyboard(void) { return (void)0; }
__attribute__((weak)) int SDL_GetDefaultAudioInfo(char **a, SDL_AudioSpec *b, int c) { return 0; }
__attribute__((weak)) int SDL_GetPointDisplayIndex(const SDL_Point *a) { return 0; }
__attribute__((weak)) int SDL_GetRectDisplayIndex(const SDL_Rect *a) { return 0; }
__attribute__((weak)) SDL_bool SDL_ResetHint(const char *a) { return (SDL_bool)0; }
__attribute__((weak)) Uint16 SDL_crc16(Uint16 a, const void *b, size_t c) { return 0; }
__attribute__((weak)) void SDL_GetWindowSizeInPixels(SDL_Window *a, int *b, int *c) { return (void)0; }
__attribute__((weak)) void SDL_GetJoystickGUIDInfo(SDL_JoystickGUID a, Uint16 *b, Uint16 *c, Uint16 *d, Uint16 *e) { return (void)0; }
__attribute__((weak)) int SDL_SetPrimarySelectionText(const char *a) { return 0; }
__attribute__((weak)) char* SDL_GetPrimarySelectionText(void) { return nullptr; }
__attribute__((weak)) SDL_bool SDL_HasPrimarySelectionText(void) { return (SDL_bool)0; }
__attribute__((weak)) int SDL_GameControllerGetSensorDataWithTimestamp(SDL_GameController *a, SDL_SensorType b, Uint64 *c, float *d, int e) { return 0; }
__attribute__((weak)) int SDL_SensorGetDataWithTimestamp(SDL_Sensor *a, Uint64 *b, float *c, int d) { return 0; }
__attribute__((weak)) void SDL_ResetHints(void) { return (void)0; }
__attribute__((weak)) char* SDL_strcasestr(const char *a, const char *b) { return nullptr; }
__attribute__((weak)) void SDL_GDKSuspendComplete(void) { return (void)0; }

} /* namespace sdl2 */
} /* namespace libtas */

#endif /* __APPLE__ */