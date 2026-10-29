/*
    Weak symbol definitions for SDL3 functions that libTAS does not override.

    On macOS, these provide definitions (not just declarations) for SDL3 functions
    so that &sdl3::fn references in sdldynapi.cpp can resolve at compile time.
    The weak attribute ensures the game's SDL3 library symbols override these at link time.

    Auto-generated from SDL3_dynapi_procs.h.
*/

#ifdef __APPLE__

#include "sdl3_weak_imports.h"

namespace libtas {
namespace sdl3 {

__attribute__((weak)) size_t SDL_IOprintf(SDL_IOStream *a,  const char *b, ...) { return 0; }
__attribute__((weak)) void SDL_Log(const char *a, ...) { return (void)0; }
__attribute__((weak)) void SDL_LogCritical(int a,  const char *b, ...) { return (void)0; }
__attribute__((weak)) void SDL_LogDebug(int a,  const char *b, ...) { return (void)0; }
__attribute__((weak)) void SDL_LogError(int a,  const char *b, ...) { return (void)0; }
__attribute__((weak)) void SDL_LogInfo(int a,  const char *b, ...) { return (void)0; }
__attribute__((weak)) void SDL_LogMessage(int a, SDL_LogPriority b,  const char *c, ...) { return (void)0; }
__attribute__((weak)) void SDL_LogTrace(int a,  const char *b, ...) { return (void)0; }
__attribute__((weak)) void SDL_LogVerbose(int a,  const char *b, ...) { return (void)0; }
__attribute__((weak)) void SDL_LogWarn(int a,  const char *b, ...) { return (void)0; }
__attribute__((weak)) bool SDL_SetError(const char *a, ...) { return false; }
__attribute__((weak)) int SDL_asprintf(char **a,  const char *b, ...) { return 0; }
__attribute__((weak)) int SDL_snprintf(char *a, size_t b,  const char *c, ...) { return 0; }
__attribute__((weak)) int SDL_sscanf(const char *a,  const char *b, ...) { return 0; }
__attribute__((weak)) int SDL_swprintf(wchar_t *a, size_t b,  const wchar_t *c, ...) { return 0; }
__attribute__((weak)) SDL_Surface* SDL_AcquireCameraFrame(SDL_Camera *a, Uint64 *b) { return nullptr; }
__attribute__((weak)) SDL_GPUCommandBuffer* SDL_AcquireGPUCommandBuffer(SDL_GPUDevice *a) { return nullptr; }
__attribute__((weak)) bool SDL_AcquireGPUSwapchainTexture(SDL_GPUCommandBuffer *a, SDL_Window *b, SDL_GPUTexture **c, Uint32 *d, Uint32 *e) { return false; }
__attribute__((weak)) int SDL_AddAtomicInt(SDL_AtomicInt *a, int b) { return 0; }
__attribute__((weak)) bool SDL_AddEventWatch(SDL_EventFilter a, void *b) { return false; }
__attribute__((weak)) int SDL_AddGamepadMapping(const char *a) { return 0; }
__attribute__((weak)) int SDL_AddGamepadMappingsFromFile(const char *a) { return 0; }
__attribute__((weak)) int SDL_AddGamepadMappingsFromIO(SDL_IOStream *a, bool b) { return 0; }
__attribute__((weak)) bool SDL_AddHintCallback(const char *a, SDL_HintCallback b, void *c) { return false; }
__attribute__((weak)) bool SDL_AddSurfaceAlternateImage(SDL_Surface *a, SDL_Surface *b) { return false; }
__attribute__((weak)) SDL_TimerID SDL_AddTimer(Uint32 a, SDL_TimerCallback b, void *c) { return (SDL_TimerID)0; }
__attribute__((weak)) SDL_TimerID SDL_AddTimerNS(Uint64 a, SDL_NSTimerCallback b, void *c) { return (SDL_TimerID)0; }
__attribute__((weak)) bool SDL_AddVulkanRenderSemaphores(SDL_Renderer *a, Uint32 b, Sint64 c, Sint64 d) { return false; }
__attribute__((weak)) SDL_JoystickID SDL_AttachVirtualJoystick(const SDL_VirtualJoystickDesc *a) { return (SDL_JoystickID)0; }
__attribute__((weak)) bool SDL_AudioDevicePaused(SDL_AudioDeviceID a) { return false; }
__attribute__((weak)) SDL_GPUComputePass* SDL_BeginGPUComputePass(SDL_GPUCommandBuffer *a, const SDL_GPUStorageTextureReadWriteBinding *b, Uint32 c, const SDL_GPUStorageBufferReadWriteBinding *d, Uint32 e) { return nullptr; }
__attribute__((weak)) SDL_GPUCopyPass* SDL_BeginGPUCopyPass(SDL_GPUCommandBuffer *a) { return nullptr; }
__attribute__((weak)) SDL_GPURenderPass* SDL_BeginGPURenderPass(SDL_GPUCommandBuffer *a, const SDL_GPUColorTargetInfo *b, Uint32 c, const SDL_GPUDepthStencilTargetInfo *d) { return nullptr; }
__attribute__((weak)) bool SDL_BindAudioStream(SDL_AudioDeviceID a, SDL_AudioStream *b) { return false; }
__attribute__((weak)) bool SDL_BindAudioStreams(SDL_AudioDeviceID a, SDL_AudioStream * const *b, int c) { return false; }
__attribute__((weak)) void SDL_BindGPUComputePipeline(SDL_GPUComputePass *a, SDL_GPUComputePipeline *b) { return (void)0; }
__attribute__((weak)) void SDL_BindGPUComputeSamplers(SDL_GPUComputePass *a, Uint32 b, const SDL_GPUTextureSamplerBinding *c, Uint32 d) { return (void)0; }
__attribute__((weak)) void SDL_BindGPUComputeStorageBuffers(SDL_GPUComputePass *a, Uint32 b, SDL_GPUBuffer *const *c, Uint32 d) { return (void)0; }
__attribute__((weak)) void SDL_BindGPUComputeStorageTextures(SDL_GPUComputePass *a, Uint32 b, SDL_GPUTexture *const *c, Uint32 d) { return (void)0; }
__attribute__((weak)) void SDL_BindGPUFragmentSamplers(SDL_GPURenderPass *a, Uint32 b, const SDL_GPUTextureSamplerBinding *c, Uint32 d) { return (void)0; }
__attribute__((weak)) void SDL_BindGPUFragmentStorageBuffers(SDL_GPURenderPass *a, Uint32 b, SDL_GPUBuffer *const *c, Uint32 d) { return (void)0; }
__attribute__((weak)) void SDL_BindGPUFragmentStorageTextures(SDL_GPURenderPass *a, Uint32 b, SDL_GPUTexture *const *c, Uint32 d) { return (void)0; }
__attribute__((weak)) void SDL_BindGPUGraphicsPipeline(SDL_GPURenderPass *a, SDL_GPUGraphicsPipeline *b) { return (void)0; }
__attribute__((weak)) void SDL_BindGPUIndexBuffer(SDL_GPURenderPass *a, const SDL_GPUBufferBinding *b, SDL_GPUIndexElementSize c) { return (void)0; }
__attribute__((weak)) void SDL_BindGPUVertexBuffers(SDL_GPURenderPass *a, Uint32 b, const SDL_GPUBufferBinding *c, Uint32 d) { return (void)0; }
__attribute__((weak)) void SDL_BindGPUVertexSamplers(SDL_GPURenderPass *a, Uint32 b, const SDL_GPUTextureSamplerBinding *c, Uint32 d) { return (void)0; }
__attribute__((weak)) void SDL_BindGPUVertexStorageBuffers(SDL_GPURenderPass *a, Uint32 b, SDL_GPUBuffer *const *c, Uint32 d) { return (void)0; }
__attribute__((weak)) void SDL_BindGPUVertexStorageTextures(SDL_GPURenderPass *a, Uint32 b, SDL_GPUTexture *const *c, Uint32 d) { return (void)0; }
__attribute__((weak)) void SDL_BlitGPUTexture(SDL_GPUCommandBuffer *a, const SDL_GPUBlitInfo *b) { return (void)0; }
__attribute__((weak)) bool SDL_BlitSurface(SDL_Surface *a, const SDL_Rect *b, SDL_Surface *c, const SDL_Rect *d) { return false; }
__attribute__((weak)) bool SDL_BlitSurface9Grid(SDL_Surface *a, const SDL_Rect *b, int c, int d, int e, int f, float g, SDL_ScaleMode h, SDL_Surface *i, const SDL_Rect *j) { return false; }
__attribute__((weak)) bool SDL_BlitSurfaceScaled(SDL_Surface *a, const SDL_Rect *b, SDL_Surface *c, const SDL_Rect *d, SDL_ScaleMode e) { return false; }
__attribute__((weak)) bool SDL_BlitSurfaceTiled(SDL_Surface *a, const SDL_Rect *b, SDL_Surface *c, const SDL_Rect *d) { return false; }
__attribute__((weak)) bool SDL_BlitSurfaceTiledWithScale(SDL_Surface *a, const SDL_Rect *b, float c, SDL_ScaleMode d, SDL_Surface *e, const SDL_Rect *f) { return false; }
__attribute__((weak)) bool SDL_BlitSurfaceUnchecked(SDL_Surface *a, const SDL_Rect *b, SDL_Surface *c, const SDL_Rect *d) { return false; }
__attribute__((weak)) bool SDL_BlitSurfaceUncheckedScaled(SDL_Surface *a, const SDL_Rect *b, SDL_Surface *c, const SDL_Rect *d, SDL_ScaleMode e) { return false; }
__attribute__((weak)) void SDL_BroadcastCondition(SDL_Condition *a) { return (void)0; }
__attribute__((weak)) bool SDL_CaptureMouse(bool a) { return false; }
__attribute__((weak)) bool SDL_ClaimWindowForGPUDevice(SDL_GPUDevice *a, SDL_Window *b) { return false; }
__attribute__((weak)) void SDL_CleanupTLS(void) { return (void)0; }
__attribute__((weak)) bool SDL_ClearAudioStream(SDL_AudioStream *a) { return false; }
__attribute__((weak)) bool SDL_ClearClipboardData(void) { return false; }
__attribute__((weak)) bool SDL_ClearComposition(SDL_Window *a) { return false; }
__attribute__((weak)) bool SDL_ClearError(void) { return false; }
__attribute__((weak)) bool SDL_ClearProperty(SDL_PropertiesID a, const char *b) { return false; }
__attribute__((weak)) bool SDL_ClearSurface(SDL_Surface *a, float b, float c, float d, float e) { return false; }
__attribute__((weak)) void SDL_CloseAudioDevice(SDL_AudioDeviceID a) { return (void)0; }
__attribute__((weak)) void SDL_CloseCamera(SDL_Camera *a) { return (void)0; }
__attribute__((weak)) void SDL_CloseGamepad(SDL_Gamepad *a) { return (void)0; }
__attribute__((weak)) void SDL_CloseHaptic(SDL_Haptic *a) { return (void)0; }
__attribute__((weak)) bool SDL_CloseIO(SDL_IOStream *a) { return false; }
__attribute__((weak)) void SDL_CloseJoystick(SDL_Joystick *a) { return (void)0; }
__attribute__((weak)) void SDL_CloseSensor(SDL_Sensor *a) { return (void)0; }
__attribute__((weak)) bool SDL_CloseStorage(SDL_Storage *a) { return false; }
__attribute__((weak)) bool SDL_CompareAndSwapAtomicInt(SDL_AtomicInt *a, int b, int c) { return false; }
__attribute__((weak)) bool SDL_CompareAndSwapAtomicPointer(void **a, void *b, void *c) { return false; }
__attribute__((weak)) bool SDL_CompareAndSwapAtomicU32(SDL_AtomicU32 *a, Uint32 b, Uint32 c) { return false; }
__attribute__((weak)) SDL_BlendMode SDL_ComposeCustomBlendMode(SDL_BlendFactor a, SDL_BlendFactor b, SDL_BlendOperation c, SDL_BlendFactor d, SDL_BlendFactor e, SDL_BlendOperation f) { return (SDL_BlendMode)0; }
__attribute__((weak)) bool SDL_ConvertAudioSamples(const SDL_AudioSpec *a, const Uint8 *b, int c, const SDL_AudioSpec *d, Uint8 **e, int *f) { return false; }
__attribute__((weak)) bool SDL_ConvertEventToRenderCoordinates(SDL_Renderer *a, SDL_Event *b) { return false; }
__attribute__((weak)) bool SDL_ConvertPixels(int a, int b, SDL_PixelFormat c, const void *d, int e, SDL_PixelFormat f, void *g, int h) { return false; }
__attribute__((weak)) bool SDL_ConvertPixelsAndColorspace(int a, int b, SDL_PixelFormat c, SDL_Colorspace d, SDL_PropertiesID e, const void *f, int g, SDL_PixelFormat h, SDL_Colorspace i, SDL_PropertiesID j, void *k, int l) { return false; }
__attribute__((weak)) SDL_Surface* SDL_ConvertSurface(SDL_Surface *a, SDL_PixelFormat b) { return nullptr; }
__attribute__((weak)) SDL_Surface* SDL_ConvertSurfaceAndColorspace(SDL_Surface *a, SDL_PixelFormat b, SDL_Palette *c, SDL_Colorspace d, SDL_PropertiesID e) { return nullptr; }
__attribute__((weak)) bool SDL_CopyFile(const char *a, const char *b) { return false; }
__attribute__((weak)) void SDL_CopyGPUBufferToBuffer(SDL_GPUCopyPass *a, const SDL_GPUBufferLocation *b, const SDL_GPUBufferLocation *c, Uint32 d, bool e) { return (void)0; }
__attribute__((weak)) void SDL_CopyGPUTextureToTexture(SDL_GPUCopyPass *a, const SDL_GPUTextureLocation *b, const SDL_GPUTextureLocation *c, Uint32 d, Uint32 e, Uint32 f, bool g) { return (void)0; }
__attribute__((weak)) bool SDL_CopyProperties(SDL_PropertiesID a, SDL_PropertiesID b) { return false; }
__attribute__((weak)) bool SDL_CopyStorageFile(SDL_Storage *a, const char *b, const char *c) { return false; }
__attribute__((weak)) SDL_AudioStream* SDL_CreateAudioStream(const SDL_AudioSpec *a, const SDL_AudioSpec *b) { return nullptr; }
__attribute__((weak)) SDL_Cursor* SDL_CreateColorCursor(SDL_Surface *a, int b, int c) { return nullptr; }
__attribute__((weak)) SDL_Condition* SDL_CreateCondition(void) { return nullptr; }
__attribute__((weak)) SDL_Cursor* SDL_CreateCursor(const Uint8 *a, const Uint8 *b, int c, int d, int e, int f) { return nullptr; }
__attribute__((weak)) bool SDL_CreateDirectory(const char *a) { return false; }
__attribute__((weak)) SDL_Environment* SDL_CreateEnvironment(bool a) { return nullptr; }
__attribute__((weak)) SDL_GPUBuffer* SDL_CreateGPUBuffer(SDL_GPUDevice *a, const SDL_GPUBufferCreateInfo* b) { return nullptr; }
__attribute__((weak)) SDL_GPUComputePipeline* SDL_CreateGPUComputePipeline(SDL_GPUDevice *a, const SDL_GPUComputePipelineCreateInfo *b) { return nullptr; }
__attribute__((weak)) SDL_GPUDevice* SDL_CreateGPUDevice(SDL_GPUShaderFormat a, bool b, const char *c) { return nullptr; }
__attribute__((weak)) SDL_GPUDevice* SDL_CreateGPUDeviceWithProperties(SDL_PropertiesID a) { return nullptr; }
__attribute__((weak)) SDL_GPUGraphicsPipeline* SDL_CreateGPUGraphicsPipeline(SDL_GPUDevice *a, const SDL_GPUGraphicsPipelineCreateInfo *b) { return nullptr; }
__attribute__((weak)) SDL_GPUSampler* SDL_CreateGPUSampler(SDL_GPUDevice *a, const SDL_GPUSamplerCreateInfo *b) { return nullptr; }
__attribute__((weak)) SDL_GPUShader* SDL_CreateGPUShader(SDL_GPUDevice *a, const SDL_GPUShaderCreateInfo *b) { return nullptr; }
__attribute__((weak)) SDL_GPUTexture* SDL_CreateGPUTexture(SDL_GPUDevice *a, const SDL_GPUTextureCreateInfo *b) { return nullptr; }
__attribute__((weak)) SDL_GPUTransferBuffer* SDL_CreateGPUTransferBuffer(SDL_GPUDevice *a, const SDL_GPUTransferBufferCreateInfo *b) { return nullptr; }
__attribute__((weak)) SDL_HapticEffectID SDL_CreateHapticEffect(SDL_Haptic *a, const SDL_HapticEffect *b) { return (SDL_HapticEffectID)0; }
__attribute__((weak)) SDL_Mutex* SDL_CreateMutex(void) { return nullptr; }
__attribute__((weak)) SDL_Palette* SDL_CreatePalette(int a) { return nullptr; }
__attribute__((weak)) SDL_Window* SDL_CreatePopupWindow(SDL_Window *a, int b, int c, int d, int e, SDL_WindowFlags f) { return nullptr; }
__attribute__((weak)) SDL_Process* SDL_CreateProcess(const char * const *a, bool b) { return nullptr; }
__attribute__((weak)) SDL_Process* SDL_CreateProcessWithProperties(SDL_PropertiesID a) { return nullptr; }
__attribute__((weak)) SDL_PropertiesID SDL_CreateProperties(void) { return (SDL_PropertiesID)0; }
__attribute__((weak)) SDL_RWLock* SDL_CreateRWLock(void) { return nullptr; }
__attribute__((weak)) SDL_Renderer* SDL_CreateRenderer(SDL_Window *a, const char *b) { return nullptr; }
__attribute__((weak)) SDL_Renderer* SDL_CreateRendererWithProperties(SDL_PropertiesID a) { return nullptr; }
__attribute__((weak)) SDL_Semaphore* SDL_CreateSemaphore(Uint32 a) { return nullptr; }
__attribute__((weak)) SDL_Renderer* SDL_CreateSoftwareRenderer(SDL_Surface *a) { return nullptr; }
__attribute__((weak)) bool SDL_CreateStorageDirectory(SDL_Storage *a, const char *b) { return false; }
__attribute__((weak)) SDL_Surface* SDL_CreateSurface(int a, int b, SDL_PixelFormat c) { return nullptr; }
__attribute__((weak)) SDL_Surface* SDL_CreateSurfaceFrom(int a, int b, SDL_PixelFormat c, void *d, int e) { return nullptr; }
__attribute__((weak)) SDL_Palette* SDL_CreateSurfacePalette(SDL_Surface *a) { return nullptr; }
__attribute__((weak)) SDL_Cursor* SDL_CreateSystemCursor(SDL_SystemCursor a) { return nullptr; }
__attribute__((weak)) SDL_Texture* SDL_CreateTexture(SDL_Renderer *a, SDL_PixelFormat b, SDL_TextureAccess c, int d, int e) { return nullptr; }
__attribute__((weak)) SDL_Texture* SDL_CreateTextureFromSurface(SDL_Renderer *a, SDL_Surface *b) { return nullptr; }
__attribute__((weak)) SDL_Texture* SDL_CreateTextureWithProperties(SDL_Renderer *a, SDL_PropertiesID b) { return nullptr; }
__attribute__((weak)) SDL_Thread* SDL_CreateThreadRuntime(SDL_ThreadFunction a, const char *b, void *c, SDL_FunctionPointer d, SDL_FunctionPointer e) { return nullptr; }
__attribute__((weak)) SDL_Thread* SDL_CreateThreadWithPropertiesRuntime(SDL_PropertiesID a, SDL_FunctionPointer b, SDL_FunctionPointer c) { return nullptr; }
__attribute__((weak)) SDL_Window* SDL_CreateWindow(const char *a, int b, int c, SDL_WindowFlags d) { return nullptr; }
__attribute__((weak)) bool SDL_CreateWindowAndRenderer(const char *a, int b, int c, SDL_WindowFlags d, SDL_Window **e, SDL_Renderer **f) { return false; }
__attribute__((weak)) SDL_Window* SDL_CreateWindowWithProperties(SDL_PropertiesID a) { return nullptr; }
__attribute__((weak)) bool SDL_CursorVisible(void) { return false; }
__attribute__((weak)) bool SDL_DateTimeToTime(const SDL_DateTime *a, SDL_Time *b) { return false; }
__attribute__((weak)) void SDL_Delay(Uint32 a) { return (void)0; }
__attribute__((weak)) void SDL_DelayNS(Uint64 a) { return (void)0; }
__attribute__((weak)) void SDL_DestroyAudioStream(SDL_AudioStream *a) { return (void)0; }
__attribute__((weak)) void SDL_DestroyCondition(SDL_Condition *a) { return (void)0; }
__attribute__((weak)) void SDL_DestroyCursor(SDL_Cursor *a) { return (void)0; }
__attribute__((weak)) void SDL_DestroyEnvironment(SDL_Environment *a) { return (void)0; }
__attribute__((weak)) void SDL_DestroyGPUDevice(SDL_GPUDevice *a) { return (void)0; }
__attribute__((weak)) void SDL_DestroyHapticEffect(SDL_Haptic *a, SDL_HapticEffectID b) { return (void)0; }
__attribute__((weak)) void SDL_DestroyMutex(SDL_Mutex *a) { return (void)0; }
__attribute__((weak)) void SDL_DestroyPalette(SDL_Palette *a) { return (void)0; }
__attribute__((weak)) void SDL_DestroyProcess(SDL_Process *a) { return (void)0; }
__attribute__((weak)) void SDL_DestroyProperties(SDL_PropertiesID a) { return (void)0; }
__attribute__((weak)) void SDL_DestroyRWLock(SDL_RWLock *a) { return (void)0; }
__attribute__((weak)) void SDL_DestroyRenderer(SDL_Renderer *a) { return (void)0; }
__attribute__((weak)) void SDL_DestroySemaphore(SDL_Semaphore *a) { return (void)0; }
__attribute__((weak)) void SDL_DestroySurface(SDL_Surface *a) { return (void)0; }
__attribute__((weak)) void SDL_DestroyTexture(SDL_Texture *a) { return (void)0; }
__attribute__((weak)) void SDL_DestroyWindow(SDL_Window *a) { return (void)0; }
__attribute__((weak)) bool SDL_DestroyWindowSurface(SDL_Window *a) { return false; }
__attribute__((weak)) void SDL_DetachThread(SDL_Thread *a) { return (void)0; }
__attribute__((weak)) bool SDL_DetachVirtualJoystick(SDL_JoystickID a) { return false; }
__attribute__((weak)) bool SDL_DisableScreenSaver(void) { return false; }
__attribute__((weak)) void SDL_DispatchGPUCompute(SDL_GPUComputePass *a, Uint32 b, Uint32 c, Uint32 d) { return (void)0; }
__attribute__((weak)) void SDL_DispatchGPUComputeIndirect(SDL_GPUComputePass *a, SDL_GPUBuffer *b, Uint32 c) { return (void)0; }
__attribute__((weak)) void SDL_DownloadFromGPUBuffer(SDL_GPUCopyPass *a, const SDL_GPUBufferRegion *b, const SDL_GPUTransferBufferLocation *c) { return (void)0; }
__attribute__((weak)) void SDL_DownloadFromGPUTexture(SDL_GPUCopyPass *a, const SDL_GPUTextureRegion *b, const SDL_GPUTextureTransferInfo *c) { return (void)0; }
__attribute__((weak)) void SDL_DrawGPUIndexedPrimitives(SDL_GPURenderPass *a, Uint32 b, Uint32 c, Uint32 d, Sint32 e, Uint32 f) { return (void)0; }
__attribute__((weak)) void SDL_DrawGPUIndexedPrimitivesIndirect(SDL_GPURenderPass *a, SDL_GPUBuffer *b, Uint32 c, Uint32 d) { return (void)0; }
__attribute__((weak)) void SDL_DrawGPUPrimitives(SDL_GPURenderPass *a, Uint32 b, Uint32 c, Uint32 d, Uint32 e) { return (void)0; }
__attribute__((weak)) void SDL_DrawGPUPrimitivesIndirect(SDL_GPURenderPass *a, SDL_GPUBuffer *b, Uint32 c, Uint32 d) { return (void)0; }
__attribute__((weak)) SDL_Surface* SDL_DuplicateSurface(SDL_Surface *a) { return nullptr; }
__attribute__((weak)) SDL_EGLConfig SDL_EGL_GetCurrentConfig(void) { return (SDL_EGLConfig)0; }
__attribute__((weak)) SDL_EGLDisplay SDL_EGL_GetCurrentDisplay(void) { return (SDL_EGLDisplay)0; }
__attribute__((weak)) SDL_FunctionPointer SDL_EGL_GetProcAddress(const char *a) { return (SDL_FunctionPointer)0; }
__attribute__((weak)) SDL_EGLSurface SDL_EGL_GetWindowSurface(SDL_Window *a) { return (SDL_EGLSurface)0; }
__attribute__((weak)) void SDL_EGL_SetAttributeCallbacks(SDL_EGLAttribArrayCallback a, SDL_EGLIntArrayCallback b, SDL_EGLIntArrayCallback c, void *d) { return (void)0; }
__attribute__((weak)) bool SDL_EnableScreenSaver(void) { return false; }
__attribute__((weak)) void SDL_EndGPUComputePass(SDL_GPUComputePass *a) { return (void)0; }
__attribute__((weak)) void SDL_EndGPUCopyPass(SDL_GPUCopyPass *a) { return (void)0; }
__attribute__((weak)) void SDL_EndGPURenderPass(SDL_GPURenderPass *a) { return (void)0; }
__attribute__((weak)) int SDL_EnterAppMainCallbacks(int a, char *b[], SDL_AppInit_func c, SDL_AppIterate_func d, SDL_AppEvent_func e, SDL_AppQuit_func f) { return 0; }
__attribute__((weak)) bool SDL_EnumerateDirectory(const char *a, SDL_EnumerateDirectoryCallback b, void *c) { return false; }
__attribute__((weak)) bool SDL_EnumerateProperties(SDL_PropertiesID a, SDL_EnumeratePropertiesCallback b, void *c) { return false; }
__attribute__((weak)) bool SDL_EnumerateStorageDirectory(SDL_Storage *a, const char *b, SDL_EnumerateDirectoryCallback c, void *d) { return false; }
__attribute__((weak)) bool SDL_EventEnabled(Uint32 a) { return false; }
__attribute__((weak)) bool SDL_FillSurfaceRect(SDL_Surface *a, const SDL_Rect *b, Uint32 c) { return false; }
__attribute__((weak)) bool SDL_FillSurfaceRects(SDL_Surface *a, const SDL_Rect *b, int c, Uint32 d) { return false; }
__attribute__((weak)) void SDL_FilterEvents(SDL_EventFilter a, void *b) { return (void)0; }
__attribute__((weak)) bool SDL_FlashWindow(SDL_Window *a, SDL_FlashOperation b) { return false; }
__attribute__((weak)) bool SDL_FlipSurface(SDL_Surface *a, SDL_FlipMode b) { return false; }
__attribute__((weak)) bool SDL_FlushAudioStream(SDL_AudioStream *a) { return false; }
__attribute__((weak)) void SDL_FlushEvent(Uint32 a) { return (void)0; }
__attribute__((weak)) void SDL_FlushEvents(Uint32 a, Uint32 b) { return (void)0; }
__attribute__((weak)) bool SDL_FlushIO(SDL_IOStream *a) { return false; }
__attribute__((weak)) bool SDL_FlushRenderer(SDL_Renderer *a) { return false; }
__attribute__((weak)) void SDL_GDKResumeGPU(SDL_GPUDevice *a) { return (void)0; }
__attribute__((weak)) void SDL_GDKSuspendComplete(void) { return (void)0; }
__attribute__((weak)) void SDL_GDKSuspendGPU(SDL_GPUDevice *a) { return (void)0; }
__attribute__((weak)) SDL_GLContext SDL_GL_CreateContext(SDL_Window *a) { return (SDL_GLContext)0; }
__attribute__((weak)) bool SDL_GL_DestroyContext(SDL_GLContext a) { return false; }
__attribute__((weak)) bool SDL_GL_ExtensionSupported(const char *a) { return false; }
__attribute__((weak)) bool SDL_GL_GetAttribute(SDL_GLAttr a, int *b) { return false; }
__attribute__((weak)) SDL_GLContext SDL_GL_GetCurrentContext(void) { return (SDL_GLContext)0; }
__attribute__((weak)) SDL_Window* SDL_GL_GetCurrentWindow(void) { return nullptr; }
__attribute__((weak)) SDL_FunctionPointer SDL_GL_GetProcAddress(const char *a) { return (SDL_FunctionPointer)0; }
__attribute__((weak)) bool SDL_GL_GetSwapInterval(int *a) { return false; }
__attribute__((weak)) bool SDL_GL_LoadLibrary(const char *a) { return false; }
__attribute__((weak)) bool SDL_GL_MakeCurrent(SDL_Window *a, SDL_GLContext b) { return false; }
__attribute__((weak)) void SDL_GL_ResetAttributes(void) { return (void)0; }
__attribute__((weak)) bool SDL_GL_SetAttribute(SDL_GLAttr a, int b) { return false; }
__attribute__((weak)) bool SDL_GL_SetSwapInterval(int a) { return false; }
__attribute__((weak)) bool SDL_GL_SwapWindow(SDL_Window *a) { return false; }
__attribute__((weak)) void SDL_GL_UnloadLibrary(void) { return (void)0; }
__attribute__((weak)) bool SDL_GPUSupportsProperties(SDL_PropertiesID a) { return false; }
__attribute__((weak)) bool SDL_GPUSupportsShaderFormats(SDL_GPUShaderFormat a, const char *b) { return false; }
__attribute__((weak)) Uint32 SDL_GPUTextureFormatTexelBlockSize(SDL_GPUTextureFormat a) { return 0; }
__attribute__((weak)) bool SDL_GPUTextureSupportsFormat(SDL_GPUDevice *a, SDL_GPUTextureFormat b, SDL_GPUTextureType c, SDL_GPUTextureUsageFlags d) { return false; }
__attribute__((weak)) bool SDL_GPUTextureSupportsSampleCount(SDL_GPUDevice *a, SDL_GPUTextureFormat b, SDL_GPUSampleCount c) { return false; }
__attribute__((weak)) void SDL_GUIDToString(SDL_GUID a, char *b, int c) { return (void)0; }
__attribute__((weak)) bool SDL_GamepadConnected(SDL_Gamepad *a) { return false; }
__attribute__((weak)) bool SDL_GamepadEventsEnabled(void) { return false; }
__attribute__((weak)) bool SDL_GamepadHasAxis(SDL_Gamepad *a, SDL_GamepadAxis b) { return false; }
__attribute__((weak)) bool SDL_GamepadHasButton(SDL_Gamepad *a, SDL_GamepadButton b) { return false; }
__attribute__((weak)) bool SDL_GamepadHasSensor(SDL_Gamepad *a, SDL_SensorType b) { return false; }
__attribute__((weak)) bool SDL_GamepadSensorEnabled(SDL_Gamepad *a, SDL_SensorType b) { return false; }
__attribute__((weak)) void SDL_GenerateMipmapsForGPUTexture(SDL_GPUCommandBuffer *a, SDL_GPUTexture *b) { return (void)0; }
__attribute__((weak)) void* SDL_GetAndroidActivity(void) { return nullptr; }
__attribute__((weak)) const char* SDL_GetAndroidCachePath(void) { return nullptr; }
__attribute__((weak)) const char* SDL_GetAndroidExternalStoragePath(void) { return nullptr; }
__attribute__((weak)) Uint32 SDL_GetAndroidExternalStorageState(void) { return 0; }
__attribute__((weak)) const char* SDL_GetAndroidInternalStoragePath(void) { return nullptr; }
__attribute__((weak)) void* SDL_GetAndroidJNIEnv(void) { return nullptr; }
__attribute__((weak)) int SDL_GetAndroidSDKVersion(void) { return 0; }
__attribute__((weak)) const char* SDL_GetAppMetadataProperty(const char *a) { return nullptr; }
__attribute__((weak)) SDL_AssertionHandler SDL_GetAssertionHandler(void **a) { return (SDL_AssertionHandler)0; }
__attribute__((weak)) const SDL_AssertData* SDL_GetAssertionReport(void) { return nullptr; }
__attribute__((weak)) int SDL_GetAtomicInt(SDL_AtomicInt *a) { return 0; }
__attribute__((weak)) void* SDL_GetAtomicPointer(void **a) { return nullptr; }
__attribute__((weak)) Uint32 SDL_GetAtomicU32(SDL_AtomicU32 *a) { return 0; }
__attribute__((weak)) int* SDL_GetAudioDeviceChannelMap(SDL_AudioDeviceID a, int *b) { return nullptr; }
__attribute__((weak)) bool SDL_GetAudioDeviceFormat(SDL_AudioDeviceID a, SDL_AudioSpec *b, int *c) { return false; }
__attribute__((weak)) float SDL_GetAudioDeviceGain(SDL_AudioDeviceID a) { return 0.0f; }
__attribute__((weak)) const char* SDL_GetAudioDeviceName(SDL_AudioDeviceID a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetAudioDriver(int a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetAudioFormatName(SDL_AudioFormat a) { return nullptr; }
__attribute__((weak)) SDL_AudioDeviceID* SDL_GetAudioPlaybackDevices(int *a) { return nullptr; }
__attribute__((weak)) SDL_AudioDeviceID* SDL_GetAudioRecordingDevices(int *a) { return nullptr; }
__attribute__((weak)) int SDL_GetAudioStreamAvailable(SDL_AudioStream *a) { return 0; }
__attribute__((weak)) int SDL_GetAudioStreamData(SDL_AudioStream *a, void *b, int c) { return 0; }
__attribute__((weak)) SDL_AudioDeviceID SDL_GetAudioStreamDevice(SDL_AudioStream *a) { return (SDL_AudioDeviceID)0; }
__attribute__((weak)) bool SDL_GetAudioStreamFormat(SDL_AudioStream *a, SDL_AudioSpec *b, SDL_AudioSpec *c) { return false; }
__attribute__((weak)) float SDL_GetAudioStreamFrequencyRatio(SDL_AudioStream *a) { return 0.0f; }
__attribute__((weak)) float SDL_GetAudioStreamGain(SDL_AudioStream *a) { return 0.0f; }
__attribute__((weak)) int* SDL_GetAudioStreamInputChannelMap(SDL_AudioStream *a, int *b) { return nullptr; }
__attribute__((weak)) int* SDL_GetAudioStreamOutputChannelMap(SDL_AudioStream *a, int *b) { return nullptr; }
__attribute__((weak)) SDL_PropertiesID SDL_GetAudioStreamProperties(SDL_AudioStream *a) { return (SDL_PropertiesID)0; }
__attribute__((weak)) int SDL_GetAudioStreamQueued(SDL_AudioStream *a) { return 0; }
__attribute__((weak)) const char* SDL_GetBasePath(void) { return nullptr; }
__attribute__((weak)) bool SDL_GetBooleanProperty(SDL_PropertiesID a, const char *b, bool c) { return false; }
__attribute__((weak)) int SDL_GetCPUCacheLineSize(void) { return 0; }
__attribute__((weak)) const char* SDL_GetCameraDriver(int a) { return nullptr; }
__attribute__((weak)) bool SDL_GetCameraFormat(SDL_Camera *a, SDL_CameraSpec *b) { return false; }
__attribute__((weak)) SDL_CameraID SDL_GetCameraID(SDL_Camera *a) { return (SDL_CameraID)0; }
__attribute__((weak)) const char* SDL_GetCameraName(SDL_CameraID a) { return nullptr; }
__attribute__((weak)) SDL_CameraPermissionState SDL_GetCameraPermissionState(SDL_Camera *a) { return (SDL_CameraPermissionState)0; }
__attribute__((weak)) SDL_CameraPosition SDL_GetCameraPosition(SDL_CameraID a) { return (SDL_CameraPosition)0; }
__attribute__((weak)) SDL_PropertiesID SDL_GetCameraProperties(SDL_Camera *a) { return (SDL_PropertiesID)0; }
__attribute__((weak)) SDL_CameraSpec** SDL_GetCameraSupportedFormats(SDL_CameraID a, int *b) { return nullptr; }
__attribute__((weak)) SDL_CameraID* SDL_GetCameras(int *a) { return nullptr; }
__attribute__((weak)) void* SDL_GetClipboardData(const char *a, size_t *b) { return nullptr; }
__attribute__((weak)) char ** SDL_GetClipboardMimeTypes(size_t *a) { return nullptr; }
__attribute__((weak)) char* SDL_GetClipboardText(void) { return nullptr; }
__attribute__((weak)) bool SDL_GetClosestFullscreenDisplayMode(SDL_DisplayID a, int b, int c, float d, bool e, SDL_DisplayMode *f) { return false; }
__attribute__((weak)) const char* SDL_GetCurrentAudioDriver(void) { return nullptr; }
__attribute__((weak)) const char* SDL_GetCurrentCameraDriver(void) { return nullptr; }
__attribute__((weak)) const SDL_DisplayMode* SDL_GetCurrentDisplayMode(SDL_DisplayID a) { return nullptr; }
__attribute__((weak)) SDL_DisplayOrientation SDL_GetCurrentDisplayOrientation(SDL_DisplayID a) { return (SDL_DisplayOrientation)0; }
__attribute__((weak)) bool SDL_GetCurrentRenderOutputSize(SDL_Renderer *a, int *b, int *c) { return false; }
__attribute__((weak)) SDL_ThreadID SDL_GetCurrentThreadID(void) { return (SDL_ThreadID)0; }
__attribute__((weak)) bool SDL_GetCurrentTime(SDL_Time *a) { return false; }
__attribute__((weak)) const char* SDL_GetCurrentVideoDriver(void) { return nullptr; }
__attribute__((weak)) SDL_Cursor* SDL_GetCursor(void) { return nullptr; }
__attribute__((weak)) bool SDL_GetDXGIOutputInfo(SDL_DisplayID a, int *b, int *c) { return false; }
__attribute__((weak)) bool SDL_GetDateTimeLocalePreferences(SDL_DateFormat *a, SDL_TimeFormat *b) { return false; }
__attribute__((weak)) int SDL_GetDayOfWeek(int a, int b, int c) { return 0; }
__attribute__((weak)) int SDL_GetDayOfYear(int a, int b, int c) { return 0; }
__attribute__((weak)) int SDL_GetDaysInMonth(int a, int b) { return 0; }
__attribute__((weak)) SDL_AssertionHandler SDL_GetDefaultAssertionHandler(void) { return (SDL_AssertionHandler)0; }
__attribute__((weak)) SDL_Cursor* SDL_GetDefaultCursor(void) { return nullptr; }
__attribute__((weak)) const SDL_DisplayMode* SDL_GetDesktopDisplayMode(SDL_DisplayID a) { return nullptr; }
__attribute__((weak)) int SDL_GetDirect3D9AdapterIndex(SDL_DisplayID a) { return 0; }
__attribute__((weak)) bool SDL_GetDisplayBounds(SDL_DisplayID a, SDL_Rect *b) { return false; }
__attribute__((weak)) float SDL_GetDisplayContentScale(SDL_DisplayID a) { return 0.0f; }
__attribute__((weak)) SDL_DisplayID SDL_GetDisplayForPoint(const SDL_Point *a) { return (SDL_DisplayID)0; }
__attribute__((weak)) SDL_DisplayID SDL_GetDisplayForRect(const SDL_Rect *a) { return (SDL_DisplayID)0; }
__attribute__((weak)) SDL_DisplayID SDL_GetDisplayForWindow(SDL_Window *a) { return (SDL_DisplayID)0; }
__attribute__((weak)) const char* SDL_GetDisplayName(SDL_DisplayID a) { return nullptr; }
__attribute__((weak)) SDL_PropertiesID SDL_GetDisplayProperties(SDL_DisplayID a) { return (SDL_PropertiesID)0; }
__attribute__((weak)) bool SDL_GetDisplayUsableBounds(SDL_DisplayID a, SDL_Rect *b) { return false; }
__attribute__((weak)) SDL_DisplayID* SDL_GetDisplays(int *a) { return nullptr; }
__attribute__((weak)) SDL_Environment* SDL_GetEnvironment(void) { return nullptr; }
__attribute__((weak)) const char* SDL_GetEnvironmentVariable(SDL_Environment *a, const char *b) { return nullptr; }
__attribute__((weak)) char** SDL_GetEnvironmentVariables(SDL_Environment *a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetError(void) { return nullptr; }
__attribute__((weak)) bool SDL_GetEventFilter(SDL_EventFilter *a, void **b) { return false; }
__attribute__((weak)) float SDL_GetFloatProperty(SDL_PropertiesID a, const char *b, float c) { return 0.0f; }
__attribute__((weak)) SDL_DisplayMode** SDL_GetFullscreenDisplayModes(SDL_DisplayID a, int *b) { return nullptr; }
__attribute__((weak)) bool SDL_GetGDKDefaultUser(XUserHandle *a) { return false; }
__attribute__((weak)) bool SDL_GetGDKTaskQueue(XTaskQueueHandle *a) { return false; }
__attribute__((weak)) const char* SDL_GetGPUDeviceDriver(SDL_GPUDevice *a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetGPUDriver(int a) { return nullptr; }
__attribute__((weak)) SDL_GPUShaderFormat SDL_GetGPUShaderFormats(SDL_GPUDevice *a) { return (SDL_GPUShaderFormat)0; }
__attribute__((weak)) SDL_GPUTextureFormat SDL_GetGPUSwapchainTextureFormat(SDL_GPUDevice *a, SDL_Window *b) { return (SDL_GPUTextureFormat)0; }
__attribute__((weak)) const char* SDL_GetGamepadAppleSFSymbolsNameForAxis(SDL_Gamepad *a, SDL_GamepadAxis b) { return nullptr; }
__attribute__((weak)) const char* SDL_GetGamepadAppleSFSymbolsNameForButton(SDL_Gamepad *a, SDL_GamepadButton b) { return nullptr; }
__attribute__((weak)) Sint16 SDL_GetGamepadAxis(SDL_Gamepad *a, SDL_GamepadAxis b) { return 0; }
__attribute__((weak)) SDL_GamepadAxis SDL_GetGamepadAxisFromString(const char *a) { return (SDL_GamepadAxis)0; }
__attribute__((weak)) SDL_GamepadBinding** SDL_GetGamepadBindings(SDL_Gamepad *a, int *b) { return nullptr; }
__attribute__((weak)) bool SDL_GetGamepadButton(SDL_Gamepad *a, SDL_GamepadButton b) { return false; }
__attribute__((weak)) SDL_GamepadButton SDL_GetGamepadButtonFromString(const char *a) { return (SDL_GamepadButton)0; }
__attribute__((weak)) SDL_GamepadButtonLabel SDL_GetGamepadButtonLabel(SDL_Gamepad *a, SDL_GamepadButton b) { return (SDL_GamepadButtonLabel)0; }
__attribute__((weak)) SDL_GamepadButtonLabel SDL_GetGamepadButtonLabelForType(SDL_GamepadType a, SDL_GamepadButton b) { return (SDL_GamepadButtonLabel)0; }
__attribute__((weak)) SDL_JoystickConnectionState SDL_GetGamepadConnectionState(SDL_Gamepad *a) { return (SDL_JoystickConnectionState)0; }
__attribute__((weak)) Uint16 SDL_GetGamepadFirmwareVersion(SDL_Gamepad *a) { return 0; }
__attribute__((weak)) SDL_Gamepad* SDL_GetGamepadFromID(SDL_JoystickID a) { return nullptr; }
__attribute__((weak)) SDL_Gamepad* SDL_GetGamepadFromPlayerIndex(int a) { return nullptr; }
__attribute__((weak)) SDL_GUID SDL_GetGamepadGUIDForID(SDL_JoystickID a) { return {0}; }
__attribute__((weak)) SDL_JoystickID SDL_GetGamepadID(SDL_Gamepad *a) { return (SDL_JoystickID)0; }
__attribute__((weak)) SDL_Joystick* SDL_GetGamepadJoystick(SDL_Gamepad *a) { return nullptr; }
__attribute__((weak)) char* SDL_GetGamepadMapping(SDL_Gamepad *a) { return nullptr; }
__attribute__((weak)) char* SDL_GetGamepadMappingForGUID(SDL_GUID a) { return nullptr; }
__attribute__((weak)) char* SDL_GetGamepadMappingForID(SDL_JoystickID a) { return nullptr; }
__attribute__((weak)) char ** SDL_GetGamepadMappings(int *a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetGamepadName(SDL_Gamepad *a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetGamepadNameForID(SDL_JoystickID a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetGamepadPath(SDL_Gamepad *a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetGamepadPathForID(SDL_JoystickID a) { return nullptr; }
__attribute__((weak)) int SDL_GetGamepadPlayerIndex(SDL_Gamepad *a) { return 0; }
__attribute__((weak)) int SDL_GetGamepadPlayerIndexForID(SDL_JoystickID a) { return 0; }
__attribute__((weak)) SDL_PowerState SDL_GetGamepadPowerInfo(SDL_Gamepad *a, int *b) { return (SDL_PowerState)0; }
__attribute__((weak)) Uint16 SDL_GetGamepadProduct(SDL_Gamepad *a) { return 0; }
__attribute__((weak)) Uint16 SDL_GetGamepadProductForID(SDL_JoystickID a) { return 0; }
__attribute__((weak)) Uint16 SDL_GetGamepadProductVersion(SDL_Gamepad *a) { return 0; }
__attribute__((weak)) Uint16 SDL_GetGamepadProductVersionForID(SDL_JoystickID a) { return 0; }
__attribute__((weak)) SDL_PropertiesID SDL_GetGamepadProperties(SDL_Gamepad *a) { return (SDL_PropertiesID)0; }
__attribute__((weak)) bool SDL_GetGamepadSensorData(SDL_Gamepad *a, SDL_SensorType b, float *c, int d) { return false; }
__attribute__((weak)) float SDL_GetGamepadSensorDataRate(SDL_Gamepad *a, SDL_SensorType b) { return 0.0f; }
__attribute__((weak)) const char* SDL_GetGamepadSerial(SDL_Gamepad *a) { return nullptr; }
__attribute__((weak)) Uint64 SDL_GetGamepadSteamHandle(SDL_Gamepad *a) { return 0; }
__attribute__((weak)) const char* SDL_GetGamepadStringForAxis(SDL_GamepadAxis a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetGamepadStringForButton(SDL_GamepadButton a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetGamepadStringForType(SDL_GamepadType a) { return nullptr; }
__attribute__((weak)) bool SDL_GetGamepadTouchpadFinger(SDL_Gamepad *a, int b, int c, bool *d, float *e, float *f, float *g) { return false; }
__attribute__((weak)) SDL_GamepadType SDL_GetGamepadType(SDL_Gamepad *a) { return (SDL_GamepadType)0; }
__attribute__((weak)) SDL_GamepadType SDL_GetGamepadTypeForID(SDL_JoystickID a) { return (SDL_GamepadType)0; }
__attribute__((weak)) SDL_GamepadType SDL_GetGamepadTypeFromString(const char *a) { return (SDL_GamepadType)0; }
__attribute__((weak)) Uint16 SDL_GetGamepadVendor(SDL_Gamepad *a) { return 0; }
__attribute__((weak)) Uint16 SDL_GetGamepadVendorForID(SDL_JoystickID a) { return 0; }
__attribute__((weak)) SDL_JoystickID* SDL_GetGamepads(int *a) { return nullptr; }
__attribute__((weak)) SDL_MouseButtonFlags SDL_GetGlobalMouseState(float *a, float *b) { return (SDL_MouseButtonFlags)0; }
__attribute__((weak)) SDL_PropertiesID SDL_GetGlobalProperties(void) { return (SDL_PropertiesID)0; }
__attribute__((weak)) SDL_Window* SDL_GetGrabbedWindow(void) { return nullptr; }
__attribute__((weak)) bool SDL_GetHapticEffectStatus(SDL_Haptic *a, SDL_HapticEffectID b) { return false; }
__attribute__((weak)) Uint32 SDL_GetHapticFeatures(SDL_Haptic *a) { return 0; }
__attribute__((weak)) SDL_Haptic* SDL_GetHapticFromID(SDL_HapticID a) { return nullptr; }
__attribute__((weak)) SDL_HapticID SDL_GetHapticID(SDL_Haptic *a) { return (SDL_HapticID)0; }
__attribute__((weak)) const char* SDL_GetHapticName(SDL_Haptic *a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetHapticNameForID(SDL_HapticID a) { return nullptr; }
__attribute__((weak)) SDL_HapticID* SDL_GetHaptics(int *a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetHint(const char *a) { return nullptr; }
__attribute__((weak)) bool SDL_GetHintBoolean(const char *a, bool b) { return false; }
__attribute__((weak)) SDL_PropertiesID SDL_GetIOProperties(SDL_IOStream *a) { return (SDL_PropertiesID)0; }
__attribute__((weak)) Sint64 SDL_GetIOSize(SDL_IOStream *a) { return 0; }
__attribute__((weak)) SDL_IOStatus SDL_GetIOStatus(SDL_IOStream *a) { return (SDL_IOStatus)0; }
__attribute__((weak)) Sint16 SDL_GetJoystickAxis(SDL_Joystick *a, int b) { return 0; }
__attribute__((weak)) bool SDL_GetJoystickAxisInitialState(SDL_Joystick *a, int b, Sint16 *c) { return false; }
__attribute__((weak)) bool SDL_GetJoystickBall(SDL_Joystick *a, int b, int *c, int *d) { return false; }
__attribute__((weak)) bool SDL_GetJoystickButton(SDL_Joystick *a, int b) { return false; }
__attribute__((weak)) SDL_JoystickConnectionState SDL_GetJoystickConnectionState(SDL_Joystick *a) { return (SDL_JoystickConnectionState)0; }
__attribute__((weak)) Uint16 SDL_GetJoystickFirmwareVersion(SDL_Joystick *a) { return 0; }
__attribute__((weak)) SDL_Joystick* SDL_GetJoystickFromID(SDL_JoystickID a) { return nullptr; }
__attribute__((weak)) SDL_Joystick* SDL_GetJoystickFromPlayerIndex(int a) { return nullptr; }
__attribute__((weak)) SDL_GUID SDL_GetJoystickGUID(SDL_Joystick *a) { return {0}; }
__attribute__((weak)) SDL_GUID SDL_GetJoystickGUIDForID(SDL_JoystickID a) { return {0}; }
__attribute__((weak)) void SDL_GetJoystickGUIDInfo(SDL_GUID a, Uint16 *b, Uint16 *c, Uint16 *d, Uint16 *e) { return (void)0; }
__attribute__((weak)) Uint8 SDL_GetJoystickHat(SDL_Joystick *a, int b) { return 0; }
__attribute__((weak)) SDL_JoystickID SDL_GetJoystickID(SDL_Joystick *a) { return (SDL_JoystickID)0; }
__attribute__((weak)) const char* SDL_GetJoystickName(SDL_Joystick *a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetJoystickNameForID(SDL_JoystickID a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetJoystickPath(SDL_Joystick *a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetJoystickPathForID(SDL_JoystickID a) { return nullptr; }
__attribute__((weak)) int SDL_GetJoystickPlayerIndex(SDL_Joystick *a) { return 0; }
__attribute__((weak)) int SDL_GetJoystickPlayerIndexForID(SDL_JoystickID a) { return 0; }
__attribute__((weak)) SDL_PowerState SDL_GetJoystickPowerInfo(SDL_Joystick *a, int *b) { return (SDL_PowerState)0; }
__attribute__((weak)) Uint16 SDL_GetJoystickProduct(SDL_Joystick *a) { return 0; }
__attribute__((weak)) Uint16 SDL_GetJoystickProductForID(SDL_JoystickID a) { return 0; }
__attribute__((weak)) Uint16 SDL_GetJoystickProductVersion(SDL_Joystick *a) { return 0; }
__attribute__((weak)) Uint16 SDL_GetJoystickProductVersionForID(SDL_JoystickID a) { return 0; }
__attribute__((weak)) SDL_PropertiesID SDL_GetJoystickProperties(SDL_Joystick *a) { return (SDL_PropertiesID)0; }
__attribute__((weak)) const char* SDL_GetJoystickSerial(SDL_Joystick *a) { return nullptr; }
__attribute__((weak)) SDL_JoystickType SDL_GetJoystickType(SDL_Joystick *a) { return (SDL_JoystickType)0; }
__attribute__((weak)) SDL_JoystickType SDL_GetJoystickTypeForID(SDL_JoystickID a) { return (SDL_JoystickType)0; }
__attribute__((weak)) Uint16 SDL_GetJoystickVendor(SDL_Joystick *a) { return 0; }
__attribute__((weak)) Uint16 SDL_GetJoystickVendorForID(SDL_JoystickID a) { return 0; }
__attribute__((weak)) SDL_JoystickID* SDL_GetJoysticks(int *a) { return nullptr; }
__attribute__((weak)) SDL_Keycode SDL_GetKeyFromName(const char *a) { return (SDL_Keycode)0; }
__attribute__((weak)) SDL_Keycode SDL_GetKeyFromScancode(SDL_Scancode a, SDL_Keymod b, bool c) { return (SDL_Keycode)0; }
__attribute__((weak)) const char* SDL_GetKeyName(SDL_Keycode a) { return nullptr; }
__attribute__((weak)) SDL_Window* SDL_GetKeyboardFocus(void) { return nullptr; }
__attribute__((weak)) const char* SDL_GetKeyboardNameForID(SDL_KeyboardID a) { return nullptr; }
__attribute__((weak)) const bool* SDL_GetKeyboardState(int *a) { return nullptr; }
__attribute__((weak)) SDL_KeyboardID* SDL_GetKeyboards(int *a) { return nullptr; }
__attribute__((weak)) void SDL_GetLogOutputFunction(SDL_LogOutputFunction *a, void **b) { return (void)0; }
__attribute__((weak)) SDL_LogPriority SDL_GetLogPriority(int a) { return (SDL_LogPriority)0; }
__attribute__((weak)) bool SDL_GetMasksForPixelFormat(SDL_PixelFormat a, int *b, Uint32 *c, Uint32 *d, Uint32 *e, Uint32 *f) { return false; }
__attribute__((weak)) int SDL_GetMaxHapticEffects(SDL_Haptic *a) { return 0; }
__attribute__((weak)) int SDL_GetMaxHapticEffectsPlaying(SDL_Haptic *a) { return 0; }
__attribute__((weak)) void SDL_GetMemoryFunctions(SDL_malloc_func *a, SDL_calloc_func *b, SDL_realloc_func *c, SDL_free_func *d) { return (void)0; }
__attribute__((weak)) SDL_MouseID* SDL_GetMice(int *a) { return nullptr; }
__attribute__((weak)) SDL_Keymod SDL_GetModState(void) { return (SDL_Keymod)0; }
__attribute__((weak)) SDL_Window* SDL_GetMouseFocus(void) { return nullptr; }
__attribute__((weak)) const char* SDL_GetMouseNameForID(SDL_MouseID a) { return nullptr; }
__attribute__((weak)) SDL_MouseButtonFlags SDL_GetMouseState(float *a, float *b) { return (SDL_MouseButtonFlags)0; }
__attribute__((weak)) SDL_DisplayOrientation SDL_GetNaturalDisplayOrientation(SDL_DisplayID a) { return (SDL_DisplayOrientation)0; }
__attribute__((weak)) int SDL_GetNumAllocations(void) { return 0; }
__attribute__((weak)) int SDL_GetNumAudioDrivers(void) { return 0; }
__attribute__((weak)) int SDL_GetNumCameraDrivers(void) { return 0; }
__attribute__((weak)) int SDL_GetNumGPUDrivers(void) { return 0; }
__attribute__((weak)) int SDL_GetNumGamepadTouchpadFingers(SDL_Gamepad *a, int b) { return 0; }
__attribute__((weak)) int SDL_GetNumGamepadTouchpads(SDL_Gamepad *a) { return 0; }
__attribute__((weak)) int SDL_GetNumHapticAxes(SDL_Haptic *a) { return 0; }
__attribute__((weak)) int SDL_GetNumJoystickAxes(SDL_Joystick *a) { return 0; }
__attribute__((weak)) int SDL_GetNumJoystickBalls(SDL_Joystick *a) { return 0; }
__attribute__((weak)) int SDL_GetNumJoystickButtons(SDL_Joystick *a) { return 0; }
__attribute__((weak)) int SDL_GetNumJoystickHats(SDL_Joystick *a) { return 0; }
__attribute__((weak)) int SDL_GetNumLogicalCPUCores(void) { return 0; }
__attribute__((weak)) int SDL_GetNumRenderDrivers(void) { return 0; }
__attribute__((weak)) int SDL_GetNumVideoDrivers(void) { return 0; }
__attribute__((weak)) Sint64 SDL_GetNumberProperty(SDL_PropertiesID a, const char *b, Sint64 c) { return 0; }
__attribute__((weak)) void SDL_GetOriginalMemoryFunctions(SDL_malloc_func *a, SDL_calloc_func *b, SDL_realloc_func *c, SDL_free_func *d) { return (void)0; }
__attribute__((weak)) bool SDL_GetPathInfo(const char *a, SDL_PathInfo *b) { return false; }
__attribute__((weak)) Uint64 SDL_GetPerformanceCounter(void) { return 0; }
__attribute__((weak)) Uint64 SDL_GetPerformanceFrequency(void) { return 0; }
__attribute__((weak)) const SDL_PixelFormatDetails* SDL_GetPixelFormatDetails(SDL_PixelFormat a) { return nullptr; }
__attribute__((weak)) SDL_PixelFormat SDL_GetPixelFormatForMasks(int a, Uint32 b, Uint32 c, Uint32 d, Uint32 e) { return (SDL_PixelFormat)0; }
__attribute__((weak)) const char* SDL_GetPixelFormatName(SDL_PixelFormat a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetPlatform(void) { return nullptr; }
__attribute__((weak)) void* SDL_GetPointerProperty(SDL_PropertiesID a, const char *b, void *c) { return nullptr; }
__attribute__((weak)) SDL_PowerState SDL_GetPowerInfo(int *a, int *b) { return (SDL_PowerState)0; }
__attribute__((weak)) char* SDL_GetPrefPath(const char *a, const char *b) { return nullptr; }
__attribute__((weak)) SDL_Locale** SDL_GetPreferredLocales(int *a) { return nullptr; }
__attribute__((weak)) SDL_DisplayID SDL_GetPrimaryDisplay(void) { return (SDL_DisplayID)0; }
__attribute__((weak)) char* SDL_GetPrimarySelectionText(void) { return nullptr; }
__attribute__((weak)) SDL_IOStream* SDL_GetProcessInput(SDL_Process *a) { return nullptr; }
__attribute__((weak)) SDL_IOStream* SDL_GetProcessOutput(SDL_Process *a) { return nullptr; }
__attribute__((weak)) SDL_PropertiesID SDL_GetProcessProperties(SDL_Process *a) { return (SDL_PropertiesID)0; }
__attribute__((weak)) SDL_PropertyType SDL_GetPropertyType(SDL_PropertiesID a, const char *b) { return (SDL_PropertyType)0; }
__attribute__((weak)) void SDL_GetRGB(Uint32 a, const SDL_PixelFormatDetails *b, const SDL_Palette *c, Uint8 *d, Uint8 *e, Uint8 *f) { return (void)0; }
__attribute__((weak)) void SDL_GetRGBA(Uint32 a, const SDL_PixelFormatDetails *b, const SDL_Palette *c, Uint8 *d, Uint8 *e, Uint8 *f, Uint8 *g) { return (void)0; }
__attribute__((weak)) SDL_GamepadType SDL_GetRealGamepadType(SDL_Gamepad *a) { return (SDL_GamepadType)0; }
__attribute__((weak)) SDL_GamepadType SDL_GetRealGamepadTypeForID(SDL_JoystickID a) { return (SDL_GamepadType)0; }
__attribute__((weak)) bool SDL_GetRectAndLineIntersection(const SDL_Rect *a, int *b, int *c, int *d, int *e) { return false; }
__attribute__((weak)) bool SDL_GetRectAndLineIntersectionFloat(const SDL_FRect *a, float *b, float *c, float *d, float *e) { return false; }
__attribute__((weak)) bool SDL_GetRectEnclosingPoints(const SDL_Point *a, int b, const SDL_Rect *c, SDL_Rect *d) { return false; }
__attribute__((weak)) bool SDL_GetRectEnclosingPointsFloat(const SDL_FPoint *a, int b, const SDL_FRect *c, SDL_FRect *d) { return false; }
__attribute__((weak)) bool SDL_GetRectIntersection(const SDL_Rect *a, const SDL_Rect *b, SDL_Rect *c) { return false; }
__attribute__((weak)) bool SDL_GetRectIntersectionFloat(const SDL_FRect *a, const SDL_FRect *b, SDL_FRect *c) { return false; }
__attribute__((weak)) bool SDL_GetRectUnion(const SDL_Rect *a, const SDL_Rect *b, SDL_Rect *c) { return false; }
__attribute__((weak)) bool SDL_GetRectUnionFloat(const SDL_FRect *a, const SDL_FRect *b, SDL_FRect *c) { return false; }
__attribute__((weak)) SDL_MouseButtonFlags SDL_GetRelativeMouseState(float *a, float *b) { return (SDL_MouseButtonFlags)0; }
__attribute__((weak)) bool SDL_GetRenderClipRect(SDL_Renderer *a, SDL_Rect *b) { return false; }
__attribute__((weak)) bool SDL_GetRenderColorScale(SDL_Renderer *a, float *b) { return false; }
__attribute__((weak)) bool SDL_GetRenderDrawBlendMode(SDL_Renderer *a, SDL_BlendMode *b) { return false; }
__attribute__((weak)) bool SDL_GetRenderDrawColor(SDL_Renderer *a, Uint8 *b, Uint8 *c, Uint8 *d, Uint8 *e) { return false; }
__attribute__((weak)) bool SDL_GetRenderDrawColorFloat(SDL_Renderer *a, float *b, float *c, float *d, float *e) { return false; }
__attribute__((weak)) const char* SDL_GetRenderDriver(int a) { return nullptr; }
__attribute__((weak)) bool SDL_GetRenderLogicalPresentation(SDL_Renderer *a, int *b, int *c, SDL_RendererLogicalPresentation *d) { return false; }
__attribute__((weak)) bool SDL_GetRenderLogicalPresentationRect(SDL_Renderer *a, SDL_FRect *b) { return false; }
__attribute__((weak)) void* SDL_GetRenderMetalCommandEncoder(SDL_Renderer *a) { return nullptr; }
__attribute__((weak)) void* SDL_GetRenderMetalLayer(SDL_Renderer *a) { return nullptr; }
__attribute__((weak)) bool SDL_GetRenderOutputSize(SDL_Renderer *a, int *b, int *c) { return false; }
__attribute__((weak)) bool SDL_GetRenderSafeArea(SDL_Renderer *a, SDL_Rect *b) { return false; }
__attribute__((weak)) bool SDL_GetRenderScale(SDL_Renderer *a, float *b, float *c) { return false; }
__attribute__((weak)) SDL_Texture* SDL_GetRenderTarget(SDL_Renderer *a) { return nullptr; }
__attribute__((weak)) bool SDL_GetRenderVSync(SDL_Renderer *a, int *b) { return false; }
__attribute__((weak)) bool SDL_GetRenderViewport(SDL_Renderer *a, SDL_Rect *b) { return false; }
__attribute__((weak)) SDL_Window* SDL_GetRenderWindow(SDL_Renderer *a) { return nullptr; }
__attribute__((weak)) SDL_Renderer* SDL_GetRenderer(SDL_Window *a) { return nullptr; }
__attribute__((weak)) SDL_Renderer* SDL_GetRendererFromTexture(SDL_Texture *a) { return nullptr; }
__attribute__((weak)) const char * SDL_GetRendererName(SDL_Renderer *a) { return nullptr; }
__attribute__((weak)) SDL_PropertiesID SDL_GetRendererProperties(SDL_Renderer *a) { return (SDL_PropertiesID)0; }
__attribute__((weak)) const char* SDL_GetRevision(void) { return nullptr; }
__attribute__((weak)) size_t SDL_GetSIMDAlignment(void) { return 0; }
__attribute__((weak)) SDL_Scancode SDL_GetScancodeFromKey(SDL_Keycode a, SDL_Keymod *b) { return (SDL_Scancode)0; }
__attribute__((weak)) SDL_Scancode SDL_GetScancodeFromName(const char *a) { return (SDL_Scancode)0; }
__attribute__((weak)) const char* SDL_GetScancodeName(SDL_Scancode a) { return nullptr; }
__attribute__((weak)) Uint32 SDL_GetSemaphoreValue(SDL_Semaphore *a) { return 0; }
__attribute__((weak)) bool SDL_GetSensorData(SDL_Sensor *a, float *b, int c) { return false; }
__attribute__((weak)) SDL_Sensor* SDL_GetSensorFromID(SDL_SensorID a) { return nullptr; }
__attribute__((weak)) SDL_SensorID SDL_GetSensorID(SDL_Sensor *a) { return (SDL_SensorID)0; }
__attribute__((weak)) const char* SDL_GetSensorName(SDL_Sensor *a) { return nullptr; }
__attribute__((weak)) const char* SDL_GetSensorNameForID(SDL_SensorID a) { return nullptr; }
__attribute__((weak)) int SDL_GetSensorNonPortableType(SDL_Sensor *a) { return 0; }
__attribute__((weak)) int SDL_GetSensorNonPortableTypeForID(SDL_SensorID a) { return 0; }
__attribute__((weak)) SDL_PropertiesID SDL_GetSensorProperties(SDL_Sensor *a) { return (SDL_PropertiesID)0; }
__attribute__((weak)) SDL_SensorType SDL_GetSensorType(SDL_Sensor *a) { return (SDL_SensorType)0; }
__attribute__((weak)) SDL_SensorType SDL_GetSensorTypeForID(SDL_SensorID a) { return (SDL_SensorType)0; }
__attribute__((weak)) SDL_SensorID* SDL_GetSensors(int *a) { return nullptr; }
__attribute__((weak)) int SDL_GetSilenceValueForFormat(SDL_AudioFormat a) { return 0; }
__attribute__((weak)) bool SDL_GetStorageFileSize(SDL_Storage *a, const char *b, Uint64 *c) { return false; }
__attribute__((weak)) bool SDL_GetStoragePathInfo(SDL_Storage *a, const char *b, SDL_PathInfo *c) { return false; }
__attribute__((weak)) Uint64 SDL_GetStorageSpaceRemaining(SDL_Storage *a) { return 0; }
__attribute__((weak)) const char* SDL_GetStringProperty(SDL_PropertiesID a, const char *b, const char *c) { return nullptr; }
__attribute__((weak)) bool SDL_GetSurfaceAlphaMod(SDL_Surface *a, Uint8 *b) { return false; }
__attribute__((weak)) bool SDL_GetSurfaceBlendMode(SDL_Surface *a, SDL_BlendMode *b) { return false; }
__attribute__((weak)) bool SDL_GetSurfaceClipRect(SDL_Surface *a, SDL_Rect *b) { return false; }
__attribute__((weak)) bool SDL_GetSurfaceColorKey(SDL_Surface *a, Uint32 *b) { return false; }
__attribute__((weak)) bool SDL_GetSurfaceColorMod(SDL_Surface *a, Uint8 *b, Uint8 *c, Uint8 *d) { return false; }
__attribute__((weak)) SDL_Colorspace SDL_GetSurfaceColorspace(SDL_Surface *a) { return (SDL_Colorspace)0; }
__attribute__((weak)) SDL_Surface** SDL_GetSurfaceImages(SDL_Surface *a, int *b) { return nullptr; }
__attribute__((weak)) SDL_Palette* SDL_GetSurfacePalette(SDL_Surface *a) { return nullptr; }
__attribute__((weak)) SDL_PropertiesID SDL_GetSurfaceProperties(SDL_Surface *a) { return (SDL_PropertiesID)0; }
__attribute__((weak)) int SDL_GetSystemRAM(void) { return 0; }
__attribute__((weak)) SDL_SystemTheme SDL_GetSystemTheme(void) { return (SDL_SystemTheme)0; }
__attribute__((weak)) void* SDL_GetTLS(SDL_TLSID *a) { return nullptr; }
__attribute__((weak)) bool SDL_GetTextInputArea(SDL_Window *a, SDL_Rect *b, int *c) { return false; }
__attribute__((weak)) bool SDL_GetTextureAlphaMod(SDL_Texture *a, Uint8 *b) { return false; }
__attribute__((weak)) bool SDL_GetTextureAlphaModFloat(SDL_Texture *a, float *b) { return false; }
__attribute__((weak)) bool SDL_GetTextureBlendMode(SDL_Texture *a, SDL_BlendMode *b) { return false; }
__attribute__((weak)) bool SDL_GetTextureColorMod(SDL_Texture *a, Uint8 *b, Uint8 *c, Uint8 *d) { return false; }
__attribute__((weak)) bool SDL_GetTextureColorModFloat(SDL_Texture *a, float *b, float *c, float *d) { return false; }
__attribute__((weak)) SDL_PropertiesID SDL_GetTextureProperties(SDL_Texture *a) { return (SDL_PropertiesID)0; }
__attribute__((weak)) bool SDL_GetTextureScaleMode(SDL_Texture *a, SDL_ScaleMode *b) { return false; }
__attribute__((weak)) bool SDL_GetTextureSize(SDL_Texture *a, float *b, float *c) { return false; }
__attribute__((weak)) SDL_ThreadID SDL_GetThreadID(SDL_Thread *a) { return (SDL_ThreadID)0; }
__attribute__((weak)) const char* SDL_GetThreadName(SDL_Thread *a) { return nullptr; }
__attribute__((weak)) Uint64 SDL_GetTicks(void) { return 0; }
__attribute__((weak)) Uint64 SDL_GetTicksNS(void) { return 0; }
__attribute__((weak)) const char* SDL_GetTouchDeviceName(SDL_TouchID a) { return nullptr; }
__attribute__((weak)) SDL_TouchDeviceType SDL_GetTouchDeviceType(SDL_TouchID a) { return (SDL_TouchDeviceType)0; }
__attribute__((weak)) SDL_TouchID* SDL_GetTouchDevices(int *a) { return nullptr; }
__attribute__((weak)) SDL_Finger** SDL_GetTouchFingers(SDL_TouchID a, int *b) { return nullptr; }
__attribute__((weak)) const char* SDL_GetUserFolder(SDL_Folder a) { return nullptr; }
__attribute__((weak)) int SDL_GetVersion(void) { return 0; }
__attribute__((weak)) const char* SDL_GetVideoDriver(int a) { return nullptr; }
__attribute__((weak)) bool SDL_GetWindowAspectRatio(SDL_Window *a, float *b, float *c) { return false; }
__attribute__((weak)) bool SDL_GetWindowBordersSize(SDL_Window *a, int *b, int *c, int *d, int *e) { return false; }
__attribute__((weak)) float SDL_GetWindowDisplayScale(SDL_Window *a) { return 0.0f; }
__attribute__((weak)) SDL_WindowFlags SDL_GetWindowFlags(SDL_Window *a) { return (SDL_WindowFlags)0; }
__attribute__((weak)) SDL_Window* SDL_GetWindowFromEvent(const SDL_Event *a) { return nullptr; }
__attribute__((weak)) SDL_Window* SDL_GetWindowFromID(SDL_WindowID a) { return nullptr; }
__attribute__((weak)) const SDL_DisplayMode* SDL_GetWindowFullscreenMode(SDL_Window *a) { return nullptr; }
__attribute__((weak)) void* SDL_GetWindowICCProfile(SDL_Window *a, size_t *b) { return nullptr; }
__attribute__((weak)) SDL_WindowID SDL_GetWindowID(SDL_Window *a) { return (SDL_WindowID)0; }
__attribute__((weak)) bool SDL_GetWindowKeyboardGrab(SDL_Window *a) { return false; }
__attribute__((weak)) bool SDL_GetWindowMaximumSize(SDL_Window *a, int *b, int *c) { return false; }
__attribute__((weak)) bool SDL_GetWindowMinimumSize(SDL_Window *a, int *b, int *c) { return false; }
__attribute__((weak)) bool SDL_GetWindowMouseGrab(SDL_Window *a) { return false; }
__attribute__((weak)) const SDL_Rect* SDL_GetWindowMouseRect(SDL_Window *a) { return nullptr; }
__attribute__((weak)) float SDL_GetWindowOpacity(SDL_Window *a) { return 0.0f; }
__attribute__((weak)) SDL_Window* SDL_GetWindowParent(SDL_Window *a) { return nullptr; }
__attribute__((weak)) float SDL_GetWindowPixelDensity(SDL_Window *a) { return 0.0f; }
__attribute__((weak)) SDL_PixelFormat SDL_GetWindowPixelFormat(SDL_Window *a) { return (SDL_PixelFormat)0; }
__attribute__((weak)) bool SDL_GetWindowPosition(SDL_Window *a, int *b, int *c) { return false; }
__attribute__((weak)) SDL_PropertiesID SDL_GetWindowProperties(SDL_Window *a) { return (SDL_PropertiesID)0; }
__attribute__((weak)) bool SDL_GetWindowRelativeMouseMode(SDL_Window *a) { return false; }
__attribute__((weak)) bool SDL_GetWindowSafeArea(SDL_Window *a, SDL_Rect *b) { return false; }
__attribute__((weak)) bool SDL_GetWindowSize(SDL_Window *a, int *b, int *c) { return false; }
__attribute__((weak)) bool SDL_GetWindowSizeInPixels(SDL_Window *a, int *b, int *c) { return false; }
__attribute__((weak)) SDL_Surface* SDL_GetWindowSurface(SDL_Window *a) { return nullptr; }
__attribute__((weak)) bool SDL_GetWindowSurfaceVSync(SDL_Window *a, int *b) { return false; }
__attribute__((weak)) const char* SDL_GetWindowTitle(SDL_Window *a) { return nullptr; }
__attribute__((weak)) SDL_Window** SDL_GetWindows(int *a) { return nullptr; }
__attribute__((weak)) char ** SDL_GlobDirectory(const char *a, const char *b, SDL_GlobFlags c, int *d) { return nullptr; }
__attribute__((weak)) char ** SDL_GlobStorageDirectory(SDL_Storage *a, const char *b, const char *c, SDL_GlobFlags d, int *e) { return nullptr; }
__attribute__((weak)) bool SDL_HapticEffectSupported(SDL_Haptic *a, const SDL_HapticEffect *b) { return false; }
__attribute__((weak)) bool SDL_HapticRumbleSupported(SDL_Haptic *a) { return false; }
__attribute__((weak)) bool SDL_HasARMSIMD(void) { return false; }
__attribute__((weak)) bool SDL_HasAVX(void) { return false; }
__attribute__((weak)) bool SDL_HasAVX2(void) { return false; }
__attribute__((weak)) bool SDL_HasAVX512F(void) { return false; }
__attribute__((weak)) bool SDL_HasAltiVec(void) { return false; }
__attribute__((weak)) bool SDL_HasClipboardData(const char *a) { return false; }
__attribute__((weak)) bool SDL_HasClipboardText(void) { return false; }
__attribute__((weak)) bool SDL_HasEvent(Uint32 a) { return false; }
__attribute__((weak)) bool SDL_HasEvents(Uint32 a, Uint32 b) { return false; }
__attribute__((weak)) bool SDL_HasGamepad(void) { return false; }
__attribute__((weak)) bool SDL_HasJoystick(void) { return false; }
__attribute__((weak)) bool SDL_HasKeyboard(void) { return false; }
__attribute__((weak)) bool SDL_HasLASX(void) { return false; }
__attribute__((weak)) bool SDL_HasLSX(void) { return false; }
__attribute__((weak)) bool SDL_HasMMX(void) { return false; }
__attribute__((weak)) bool SDL_HasMouse(void) { return false; }
__attribute__((weak)) bool SDL_HasNEON(void) { return false; }
__attribute__((weak)) bool SDL_HasPrimarySelectionText(void) { return false; }
__attribute__((weak)) bool SDL_HasProperty(SDL_PropertiesID a, const char *b) { return false; }
__attribute__((weak)) bool SDL_HasRectIntersection(const SDL_Rect *a, const SDL_Rect *b) { return false; }
__attribute__((weak)) bool SDL_HasRectIntersectionFloat(const SDL_FRect *a, const SDL_FRect *b) { return false; }
__attribute__((weak)) bool SDL_HasSSE(void) { return false; }
__attribute__((weak)) bool SDL_HasSSE2(void) { return false; }
__attribute__((weak)) bool SDL_HasSSE3(void) { return false; }
__attribute__((weak)) bool SDL_HasSSE41(void) { return false; }
__attribute__((weak)) bool SDL_HasSSE42(void) { return false; }
__attribute__((weak)) bool SDL_HasScreenKeyboardSupport(void) { return false; }
__attribute__((weak)) bool SDL_HideCursor(void) { return false; }
__attribute__((weak)) bool SDL_HideWindow(SDL_Window *a) { return false; }
__attribute__((weak)) SDL_IOStream* SDL_IOFromConstMem(const void *a, size_t b) { return nullptr; }
__attribute__((weak)) SDL_IOStream* SDL_IOFromDynamicMem(void) { return nullptr; }
__attribute__((weak)) SDL_IOStream* SDL_IOFromFile(const char *a, const char *b) { return nullptr; }
__attribute__((weak)) SDL_IOStream* SDL_IOFromMem(void *a, size_t b) { return nullptr; }
__attribute__((weak)) size_t SDL_IOvprintf(SDL_IOStream *a,  const char *b, va_list c) { return 0; }
__attribute__((weak)) bool SDL_Init(SDL_InitFlags a) { return false; }
__attribute__((weak)) bool SDL_InitHapticRumble(SDL_Haptic *a) { return false; }
__attribute__((weak)) bool SDL_InitSubSystem(SDL_InitFlags a) { return false; }
__attribute__((weak)) void SDL_InsertGPUDebugLabel(SDL_GPUCommandBuffer *a, const char *b) { return (void)0; }
__attribute__((weak)) bool SDL_IsChromebook(void) { return false; }
__attribute__((weak)) bool SDL_IsDeXMode(void) { return false; }
__attribute__((weak)) bool SDL_IsGamepad(SDL_JoystickID a) { return false; }
__attribute__((weak)) bool SDL_IsJoystickHaptic(SDL_Joystick *a) { return false; }
__attribute__((weak)) bool SDL_IsJoystickVirtual(SDL_JoystickID a) { return false; }
__attribute__((weak)) bool SDL_IsMouseHaptic(void) { return false; }
__attribute__((weak)) bool SDL_IsTV(void) { return false; }
__attribute__((weak)) bool SDL_IsTablet(void) { return false; }
__attribute__((weak)) bool SDL_JoystickConnected(SDL_Joystick *a) { return false; }
__attribute__((weak)) bool SDL_JoystickEventsEnabled(void) { return false; }
__attribute__((weak)) bool SDL_KillProcess(SDL_Process *a, bool b) { return false; }
__attribute__((weak)) SDL_Surface* SDL_LoadBMP(const char *a) { return nullptr; }
__attribute__((weak)) SDL_Surface* SDL_LoadBMP_IO(SDL_IOStream *a, bool b) { return nullptr; }
__attribute__((weak)) void* SDL_LoadFile(const char *a, size_t *b) { return nullptr; }
__attribute__((weak)) void* SDL_LoadFile_IO(SDL_IOStream *a, size_t *b, bool c) { return nullptr; }
__attribute__((weak)) SDL_FunctionPointer SDL_LoadFunction(SDL_SharedObject *a, const char *b) { return (SDL_FunctionPointer)0; }
__attribute__((weak)) SDL_SharedObject* SDL_LoadObject(const char *a) { return nullptr; }
__attribute__((weak)) bool SDL_LoadWAV(const char *a, SDL_AudioSpec *b, Uint8 **c, Uint32 *d) { return false; }
__attribute__((weak)) bool SDL_LoadWAV_IO(SDL_IOStream *a, bool b, SDL_AudioSpec *c, Uint8 **d, Uint32 *e) { return false; }
__attribute__((weak)) bool SDL_LockAudioStream(SDL_AudioStream *a) { return false; }
__attribute__((weak)) void SDL_LockJoysticks(void) { return (void)0; }
__attribute__((weak)) void SDL_LockMutex(SDL_Mutex *a) { return (void)0; }
__attribute__((weak)) bool SDL_LockProperties(SDL_PropertiesID a) { return false; }
__attribute__((weak)) void SDL_LockRWLockForReading(SDL_RWLock *a) { return (void)0; }
__attribute__((weak)) void SDL_LockRWLockForWriting(SDL_RWLock *a) { return (void)0; }
__attribute__((weak)) void SDL_LockSpinlock(SDL_SpinLock *a) { return (void)0; }
__attribute__((weak)) bool SDL_LockSurface(SDL_Surface *a) { return false; }
__attribute__((weak)) bool SDL_LockTexture(SDL_Texture *a, const SDL_Rect *b, void **c, int *d) { return false; }
__attribute__((weak)) bool SDL_LockTextureToSurface(SDL_Texture *a, const SDL_Rect *b, SDL_Surface **c) { return false; }
__attribute__((weak)) void SDL_LogMessageV(int a, SDL_LogPriority b,  const char *c, va_list d) { return (void)0; }
__attribute__((weak)) void* SDL_MapGPUTransferBuffer(SDL_GPUDevice *a, SDL_GPUTransferBuffer *b, bool c) { return nullptr; }
__attribute__((weak)) Uint32 SDL_MapRGB(const SDL_PixelFormatDetails *a, const SDL_Palette *b, Uint8 c, Uint8 d, Uint8 e) { return 0; }
__attribute__((weak)) Uint32 SDL_MapRGBA(const SDL_PixelFormatDetails *a, const SDL_Palette *b, Uint8 c, Uint8 d, Uint8 e, Uint8 f) { return 0; }
__attribute__((weak)) Uint32 SDL_MapSurfaceRGB(SDL_Surface *a, Uint8 b, Uint8 c, Uint8 d) { return 0; }
__attribute__((weak)) Uint32 SDL_MapSurfaceRGBA(SDL_Surface *a, Uint8 b, Uint8 c, Uint8 d, Uint8 e) { return 0; }
__attribute__((weak)) bool SDL_MaximizeWindow(SDL_Window *a) { return false; }
__attribute__((weak)) void SDL_MemoryBarrierAcquireFunction(void) { return (void)0; }
__attribute__((weak)) void SDL_MemoryBarrierReleaseFunction(void) { return (void)0; }
__attribute__((weak)) SDL_MetalView SDL_Metal_CreateView(SDL_Window *a) { return (SDL_MetalView)0; }
__attribute__((weak)) void SDL_Metal_DestroyView(SDL_MetalView a) { return (void)0; }
__attribute__((weak)) void* SDL_Metal_GetLayer(SDL_MetalView a) { return nullptr; }
__attribute__((weak)) bool SDL_MinimizeWindow(SDL_Window *a) { return false; }
__attribute__((weak)) bool SDL_MixAudio(Uint8 *a, const Uint8 *b, SDL_AudioFormat c, Uint32 d, float e) { return false; }
__attribute__((weak)) void SDL_OnApplicationDidChangeStatusBarOrientation(void) { return (void)0; }
__attribute__((weak)) void SDL_OnApplicationDidEnterBackground(void) { return (void)0; }
__attribute__((weak)) void SDL_OnApplicationDidEnterForeground(void) { return (void)0; }
__attribute__((weak)) void SDL_OnApplicationDidReceiveMemoryWarning(void) { return (void)0; }
__attribute__((weak)) void SDL_OnApplicationWillEnterBackground(void) { return (void)0; }
__attribute__((weak)) void SDL_OnApplicationWillEnterForeground(void) { return (void)0; }
__attribute__((weak)) void SDL_OnApplicationWillTerminate(void) { return (void)0; }
__attribute__((weak)) SDL_AudioDeviceID SDL_OpenAudioDevice(SDL_AudioDeviceID a, const SDL_AudioSpec *b) { return (SDL_AudioDeviceID)0; }
__attribute__((weak)) SDL_AudioStream* SDL_OpenAudioDeviceStream(SDL_AudioDeviceID a, const SDL_AudioSpec *b, SDL_AudioStreamCallback c, void *d) { return nullptr; }
__attribute__((weak)) SDL_Camera* SDL_OpenCamera(SDL_CameraID a, const SDL_CameraSpec *b) { return nullptr; }
__attribute__((weak)) SDL_Storage* SDL_OpenFileStorage(const char *a) { return nullptr; }
__attribute__((weak)) SDL_Gamepad* SDL_OpenGamepad(SDL_JoystickID a) { return nullptr; }
__attribute__((weak)) SDL_Haptic* SDL_OpenHaptic(SDL_HapticID a) { return nullptr; }
__attribute__((weak)) SDL_Haptic* SDL_OpenHapticFromJoystick(SDL_Joystick *a) { return nullptr; }
__attribute__((weak)) SDL_Haptic* SDL_OpenHapticFromMouse(void) { return nullptr; }
__attribute__((weak)) SDL_IOStream* SDL_OpenIO(const SDL_IOStreamInterface *a, void *b) { return nullptr; }
__attribute__((weak)) SDL_Joystick* SDL_OpenJoystick(SDL_JoystickID a) { return nullptr; }
__attribute__((weak)) SDL_Sensor* SDL_OpenSensor(SDL_SensorID a) { return nullptr; }
__attribute__((weak)) SDL_Storage* SDL_OpenStorage(const SDL_StorageInterface *a, void *b) { return nullptr; }
__attribute__((weak)) SDL_Storage* SDL_OpenTitleStorage(const char *a, SDL_PropertiesID b) { return nullptr; }
__attribute__((weak)) bool SDL_OpenURL(const char *a) { return false; }
__attribute__((weak)) SDL_Storage* SDL_OpenUserStorage(const char *a, const char *b, SDL_PropertiesID c) { return nullptr; }
__attribute__((weak)) bool SDL_OutOfMemory(void) { return false; }
__attribute__((weak)) bool SDL_PauseAudioDevice(SDL_AudioDeviceID a) { return false; }
__attribute__((weak)) bool SDL_PauseAudioStreamDevice(SDL_AudioStream *a) { return false; }
__attribute__((weak)) bool SDL_PauseHaptic(SDL_Haptic *a) { return false; }
__attribute__((weak)) int SDL_PeepEvents(SDL_Event *a, int b, SDL_EventAction c, Uint32 d, Uint32 e) { return 0; }
__attribute__((weak)) bool SDL_PlayHapticRumble(SDL_Haptic *a, float b, Uint32 c) { return false; }
__attribute__((weak)) bool SDL_PollEvent(SDL_Event *a) { return false; }
__attribute__((weak)) void SDL_PopGPUDebugGroup(SDL_GPUCommandBuffer *a) { return (void)0; }
__attribute__((weak)) bool SDL_PremultiplyAlpha(int a, int b, SDL_PixelFormat c, const void *d, int e, SDL_PixelFormat f, void *g, int h, bool i) { return false; }
__attribute__((weak)) bool SDL_PremultiplySurfaceAlpha(SDL_Surface *a, bool b) { return false; }
__attribute__((weak)) void SDL_PumpEvents(void) { return (void)0; }
__attribute__((weak)) bool SDL_PushEvent(SDL_Event *a) { return false; }
__attribute__((weak)) void SDL_PushGPUComputeUniformData(SDL_GPUCommandBuffer *a, Uint32 b, const void *c, Uint32 d) { return (void)0; }
__attribute__((weak)) void SDL_PushGPUDebugGroup(SDL_GPUCommandBuffer *a, const char *b) { return (void)0; }
__attribute__((weak)) void SDL_PushGPUFragmentUniformData(SDL_GPUCommandBuffer *a, Uint32 b, const void *c, Uint32 d) { return (void)0; }
__attribute__((weak)) void SDL_PushGPUVertexUniformData(SDL_GPUCommandBuffer *a, Uint32 b, const void *c, Uint32 d) { return (void)0; }
__attribute__((weak)) bool SDL_PutAudioStreamData(SDL_AudioStream *a, const void *b, int c) { return false; }
__attribute__((weak)) bool SDL_QueryGPUFence(SDL_GPUDevice *a, SDL_GPUFence *b) { return false; }
__attribute__((weak)) void SDL_Quit(void) { return (void)0; }
__attribute__((weak)) void SDL_QuitSubSystem(SDL_InitFlags a) { return (void)0; }
__attribute__((weak)) bool SDL_RaiseWindow(SDL_Window *a) { return false; }
__attribute__((weak)) size_t SDL_ReadIO(SDL_IOStream *a, void *b, size_t c) { return 0; }
__attribute__((weak)) void* SDL_ReadProcess(SDL_Process *a, size_t *b, int *c) { return nullptr; }
__attribute__((weak)) bool SDL_ReadS16BE(SDL_IOStream *a, Sint16 *b) { return false; }
__attribute__((weak)) bool SDL_ReadS16LE(SDL_IOStream *a, Sint16 *b) { return false; }
__attribute__((weak)) bool SDL_ReadS32BE(SDL_IOStream *a, Sint32 *b) { return false; }
__attribute__((weak)) bool SDL_ReadS32LE(SDL_IOStream *a, Sint32 *b) { return false; }
__attribute__((weak)) bool SDL_ReadS64BE(SDL_IOStream *a, Sint64 *b) { return false; }
__attribute__((weak)) bool SDL_ReadS64LE(SDL_IOStream *a, Sint64 *b) { return false; }
__attribute__((weak)) bool SDL_ReadS8(SDL_IOStream *a, Sint8 *b) { return false; }
__attribute__((weak)) bool SDL_ReadStorageFile(SDL_Storage *a, const char *b, void *c, Uint64 d) { return false; }
__attribute__((weak)) bool SDL_ReadSurfacePixel(SDL_Surface *a, int b, int c, Uint8 *d, Uint8 *e, Uint8 *f, Uint8 *g) { return false; }
__attribute__((weak)) bool SDL_ReadSurfacePixelFloat(SDL_Surface *a, int b, int c, float *d, float *e, float *f, float *g) { return false; }
__attribute__((weak)) bool SDL_ReadU16BE(SDL_IOStream *a, Uint16 *b) { return false; }
__attribute__((weak)) bool SDL_ReadU16LE(SDL_IOStream *a, Uint16 *b) { return false; }
__attribute__((weak)) bool SDL_ReadU32BE(SDL_IOStream *a, Uint32 *b) { return false; }
__attribute__((weak)) bool SDL_ReadU32LE(SDL_IOStream *a, Uint32 *b) { return false; }
__attribute__((weak)) bool SDL_ReadU64BE(SDL_IOStream *a, Uint64 *b) { return false; }
__attribute__((weak)) bool SDL_ReadU64LE(SDL_IOStream *a, Uint64 *b) { return false; }
__attribute__((weak)) bool SDL_ReadU8(SDL_IOStream *a, Uint8 *b) { return false; }
__attribute__((weak)) bool SDL_RegisterApp(const char *a, Uint32 b, void *c) { return false; }
__attribute__((weak)) Uint32 SDL_RegisterEvents(int a) { return 0; }
__attribute__((weak)) void SDL_ReleaseCameraFrame(SDL_Camera *a, SDL_Surface *b) { return (void)0; }
__attribute__((weak)) void SDL_ReleaseGPUBuffer(SDL_GPUDevice *a, SDL_GPUBuffer *b) { return (void)0; }
__attribute__((weak)) void SDL_ReleaseGPUComputePipeline(SDL_GPUDevice *a, SDL_GPUComputePipeline *b) { return (void)0; }
__attribute__((weak)) void SDL_ReleaseGPUFence(SDL_GPUDevice *a, SDL_GPUFence *b) { return (void)0; }
__attribute__((weak)) void SDL_ReleaseGPUGraphicsPipeline(SDL_GPUDevice *a, SDL_GPUGraphicsPipeline *b) { return (void)0; }
__attribute__((weak)) void SDL_ReleaseGPUSampler(SDL_GPUDevice *a, SDL_GPUSampler *b) { return (void)0; }
__attribute__((weak)) void SDL_ReleaseGPUShader(SDL_GPUDevice *a, SDL_GPUShader *b) { return (void)0; }
__attribute__((weak)) void SDL_ReleaseGPUTexture(SDL_GPUDevice *a, SDL_GPUTexture *b) { return (void)0; }
__attribute__((weak)) void SDL_ReleaseGPUTransferBuffer(SDL_GPUDevice *a, SDL_GPUTransferBuffer *b) { return (void)0; }
__attribute__((weak)) void SDL_ReleaseWindowFromGPUDevice(SDL_GPUDevice *a, SDL_Window *b) { return (void)0; }
__attribute__((weak)) bool SDL_ReloadGamepadMappings(void) { return false; }
__attribute__((weak)) void SDL_RemoveEventWatch(SDL_EventFilter a, void *b) { return (void)0; }
__attribute__((weak)) void SDL_RemoveHintCallback(const char *a, SDL_HintCallback b, void *c) { return (void)0; }
__attribute__((weak)) bool SDL_RemovePath(const char *a) { return false; }
__attribute__((weak)) bool SDL_RemoveStoragePath(SDL_Storage *a, const char *b) { return false; }
__attribute__((weak)) void SDL_RemoveSurfaceAlternateImages(SDL_Surface *a) { return (void)0; }
__attribute__((weak)) bool SDL_RemoveTimer(SDL_TimerID a) { return false; }
__attribute__((weak)) bool SDL_RenamePath(const char *a, const char *b) { return false; }
__attribute__((weak)) bool SDL_RenameStoragePath(SDL_Storage *a, const char *b, const char *c) { return false; }
__attribute__((weak)) bool SDL_RenderClear(SDL_Renderer *a) { return false; }
__attribute__((weak)) bool SDL_RenderClipEnabled(SDL_Renderer *a) { return false; }
__attribute__((weak)) bool SDL_RenderCoordinatesFromWindow(SDL_Renderer *a, float b, float c, float *d, float *e) { return false; }
__attribute__((weak)) bool SDL_RenderCoordinatesToWindow(SDL_Renderer *a, float b, float c, float *d, float *e) { return false; }
__attribute__((weak)) bool SDL_RenderFillRect(SDL_Renderer *a, const SDL_FRect *b) { return false; }
__attribute__((weak)) bool SDL_RenderFillRects(SDL_Renderer *a, const SDL_FRect *b, int c) { return false; }
__attribute__((weak)) bool SDL_RenderGeometry(SDL_Renderer *a, SDL_Texture *b, const SDL_Vertex *c, int d, const int *e, int f) { return false; }
__attribute__((weak)) bool SDL_RenderGeometryRaw(SDL_Renderer *a, SDL_Texture *b, const float *c, int d, const SDL_FColor *e, int f, const float *g, int h, int i, const void *j, int k, int l) { return false; }
__attribute__((weak)) bool SDL_RenderLine(SDL_Renderer *a, float b, float c, float d, float e) { return false; }
__attribute__((weak)) bool SDL_RenderLines(SDL_Renderer *a, const SDL_FPoint *b, int c) { return false; }
__attribute__((weak)) bool SDL_RenderPoint(SDL_Renderer *a, float b, float c) { return false; }
__attribute__((weak)) bool SDL_RenderPoints(SDL_Renderer *a, const SDL_FPoint *b, int c) { return false; }
__attribute__((weak)) bool SDL_RenderPresent(SDL_Renderer *a) { return false; }
__attribute__((weak)) SDL_Surface* SDL_RenderReadPixels(SDL_Renderer *a, const SDL_Rect *b) { return nullptr; }
__attribute__((weak)) bool SDL_RenderRect(SDL_Renderer *a, const SDL_FRect *b) { return false; }
__attribute__((weak)) bool SDL_RenderRects(SDL_Renderer *a, const SDL_FRect *b, int c) { return false; }
__attribute__((weak)) bool SDL_RenderTexture(SDL_Renderer *a, SDL_Texture *b, const SDL_FRect *c, const SDL_FRect *d) { return false; }
__attribute__((weak)) bool SDL_RenderTexture9Grid(SDL_Renderer *a, SDL_Texture *b, const SDL_FRect *c, float d, float e, float f, float g, float h, const SDL_FRect *i) { return false; }
__attribute__((weak)) bool SDL_RenderTextureRotated(SDL_Renderer *a, SDL_Texture *b, const SDL_FRect *c, const SDL_FRect *d, double e, const SDL_FPoint *f, SDL_FlipMode g) { return false; }
__attribute__((weak)) bool SDL_RenderTextureTiled(SDL_Renderer *a, SDL_Texture *b, const SDL_FRect *c, float d, const SDL_FRect *e) { return false; }
__attribute__((weak)) bool SDL_RenderViewportSet(SDL_Renderer *a) { return false; }
__attribute__((weak)) SDL_AssertState SDL_ReportAssertion(SDL_AssertData *a, const char *b, const char *c, int d) { return (SDL_AssertState)0; }
__attribute__((weak)) bool SDL_RequestAndroidPermission(const char *a, SDL_RequestAndroidPermissionCallback b, void *c) { return false; }
__attribute__((weak)) void SDL_ResetAssertionReport(void) { return (void)0; }
__attribute__((weak)) bool SDL_ResetHint(const char *a) { return false; }
__attribute__((weak)) void SDL_ResetHints(void) { return (void)0; }
__attribute__((weak)) void SDL_ResetKeyboard(void) { return (void)0; }
__attribute__((weak)) void SDL_ResetLogPriorities(void) { return (void)0; }
__attribute__((weak)) bool SDL_RestoreWindow(SDL_Window *a) { return false; }
__attribute__((weak)) bool SDL_ResumeAudioDevice(SDL_AudioDeviceID a) { return false; }
__attribute__((weak)) bool SDL_ResumeAudioStreamDevice(SDL_AudioStream *a) { return false; }
__attribute__((weak)) bool SDL_ResumeHaptic(SDL_Haptic *a) { return false; }
__attribute__((weak)) bool SDL_RumbleGamepad(SDL_Gamepad *a, Uint16 b, Uint16 c, Uint32 d) { return false; }
__attribute__((weak)) bool SDL_RumbleGamepadTriggers(SDL_Gamepad *a, Uint16 b, Uint16 c, Uint32 d) { return false; }
__attribute__((weak)) bool SDL_RumbleJoystick(SDL_Joystick *a, Uint16 b, Uint16 c, Uint32 d) { return false; }
__attribute__((weak)) bool SDL_RumbleJoystickTriggers(SDL_Joystick *a, Uint16 b, Uint16 c, Uint32 d) { return false; }
__attribute__((weak)) int SDL_RunApp(int a, char *b[], SDL_main_func c, void *d) { return 0; }
__attribute__((weak)) bool SDL_RunHapticEffect(SDL_Haptic *a, SDL_HapticEffectID b, Uint32 c) { return false; }
__attribute__((weak)) bool SDL_SaveBMP(SDL_Surface *a, const char *b) { return false; }
__attribute__((weak)) bool SDL_SaveBMP_IO(SDL_Surface *a, SDL_IOStream *b, bool c) { return false; }
__attribute__((weak)) SDL_Surface* SDL_ScaleSurface(SDL_Surface *a, int b, int c, SDL_ScaleMode d) { return nullptr; }
__attribute__((weak)) bool SDL_ScreenKeyboardShown(SDL_Window *a) { return false; }
__attribute__((weak)) bool SDL_ScreenSaverEnabled(void) { return false; }
__attribute__((weak)) Sint64 SDL_SeekIO(SDL_IOStream *a, Sint64 b, SDL_IOWhence c) { return 0; }
__attribute__((weak)) void SDL_SendAndroidBackButton(void) { return (void)0; }
__attribute__((weak)) bool SDL_SendAndroidMessage(Uint32 a, int b) { return false; }
__attribute__((weak)) bool SDL_SendGamepadEffect(SDL_Gamepad *a, const void *b, int c) { return false; }
__attribute__((weak)) bool SDL_SendJoystickEffect(SDL_Joystick *a, const void *b, int c) { return false; }
__attribute__((weak)) bool SDL_SendJoystickVirtualSensorData(SDL_Joystick *a, SDL_SensorType b, Uint64 c, const float *d, int e) { return false; }
__attribute__((weak)) bool SDL_SetAppMetadata(const char *a, const char *b, const char *c) { return false; }
__attribute__((weak)) bool SDL_SetAppMetadataProperty(const char *a, const char *b) { return false; }
__attribute__((weak)) void SDL_SetAssertionHandler(SDL_AssertionHandler a, void *b) { return (void)0; }
__attribute__((weak)) int SDL_SetAtomicInt(SDL_AtomicInt *a, int b) { return 0; }
__attribute__((weak)) void* SDL_SetAtomicPointer(void **a, void *b) { return nullptr; }
__attribute__((weak)) Uint32 SDL_SetAtomicU32(SDL_AtomicU32 *a, Uint32 b) { return 0; }
__attribute__((weak)) bool SDL_SetAudioDeviceGain(SDL_AudioDeviceID a, float b) { return false; }
__attribute__((weak)) bool SDL_SetAudioPostmixCallback(SDL_AudioDeviceID a, SDL_AudioPostmixCallback b, void *c) { return false; }
__attribute__((weak)) bool SDL_SetAudioStreamFormat(SDL_AudioStream *a, const SDL_AudioSpec *b, const SDL_AudioSpec *c) { return false; }
__attribute__((weak)) bool SDL_SetAudioStreamFrequencyRatio(SDL_AudioStream *a, float b) { return false; }
__attribute__((weak)) bool SDL_SetAudioStreamGain(SDL_AudioStream *a, float b) { return false; }
__attribute__((weak)) bool SDL_SetAudioStreamGetCallback(SDL_AudioStream *a, SDL_AudioStreamCallback b, void *c) { return false; }
__attribute__((weak)) bool SDL_SetAudioStreamInputChannelMap(SDL_AudioStream *a, const int *b, int c) { return false; }
__attribute__((weak)) bool SDL_SetAudioStreamOutputChannelMap(SDL_AudioStream *a, const int *b, int c) { return false; }
__attribute__((weak)) bool SDL_SetAudioStreamPutCallback(SDL_AudioStream *a, SDL_AudioStreamCallback b, void *c) { return false; }
__attribute__((weak)) bool SDL_SetBooleanProperty(SDL_PropertiesID a, const char *b, bool c) { return false; }
__attribute__((weak)) bool SDL_SetClipboardData(SDL_ClipboardDataCallback a, SDL_ClipboardCleanupCallback b, void *c, const char *const *d, size_t e) { return false; }
__attribute__((weak)) bool SDL_SetClipboardText(const char *a) { return false; }
__attribute__((weak)) bool SDL_SetCurrentThreadPriority(SDL_ThreadPriority a) { return false; }
__attribute__((weak)) bool SDL_SetCursor(SDL_Cursor *a) { return false; }
__attribute__((weak)) bool SDL_SetEnvironmentVariable(SDL_Environment *a, const char *b, const char *c, bool d) { return false; }
__attribute__((weak)) void SDL_SetEventEnabled(Uint32 a, bool b) { return (void)0; }
__attribute__((weak)) void SDL_SetEventFilter(SDL_EventFilter a, void *b) { return (void)0; }
__attribute__((weak)) bool SDL_SetFloatProperty(SDL_PropertiesID a, const char *b, float c) { return false; }
__attribute__((weak)) void SDL_SetGPUBlendConstants(SDL_GPURenderPass *a, SDL_FColor b) { return (void)0; }
__attribute__((weak)) void SDL_SetGPUBufferName(SDL_GPUDevice *a, SDL_GPUBuffer *b, const char *c) { return (void)0; }
__attribute__((weak)) void SDL_SetGPUScissor(SDL_GPURenderPass *a, const SDL_Rect *b) { return (void)0; }
__attribute__((weak)) void SDL_SetGPUStencilReference(SDL_GPURenderPass *a, Uint8 b) { return (void)0; }
__attribute__((weak)) bool SDL_SetGPUSwapchainParameters(SDL_GPUDevice *a, SDL_Window *b, SDL_GPUSwapchainComposition c, SDL_GPUPresentMode d) { return false; }
__attribute__((weak)) void SDL_SetGPUTextureName(SDL_GPUDevice *a, SDL_GPUTexture *b, const char *c) { return (void)0; }
__attribute__((weak)) void SDL_SetGPUViewport(SDL_GPURenderPass *a, const SDL_GPUViewport *b) { return (void)0; }
__attribute__((weak)) void SDL_SetGamepadEventsEnabled(bool a) { return (void)0; }
__attribute__((weak)) bool SDL_SetGamepadLED(SDL_Gamepad *a, Uint8 b, Uint8 c, Uint8 d) { return false; }
__attribute__((weak)) bool SDL_SetGamepadMapping(SDL_JoystickID a, const char *b) { return false; }
__attribute__((weak)) bool SDL_SetGamepadPlayerIndex(SDL_Gamepad *a, int b) { return false; }
__attribute__((weak)) bool SDL_SetGamepadSensorEnabled(SDL_Gamepad *a, SDL_SensorType b, bool c) { return false; }
__attribute__((weak)) bool SDL_SetHapticAutocenter(SDL_Haptic *a, int b) { return false; }
__attribute__((weak)) bool SDL_SetHapticGain(SDL_Haptic *a, int b) { return false; }
__attribute__((weak)) bool SDL_SetHint(const char *a, const char *b) { return false; }
__attribute__((weak)) bool SDL_SetHintWithPriority(const char *a, const char *b, SDL_HintPriority c) { return false; }
__attribute__((weak)) void SDL_SetInitialized(SDL_InitState *a, bool b) { return (void)0; }
__attribute__((weak)) void SDL_SetJoystickEventsEnabled(bool a) { return (void)0; }
__attribute__((weak)) bool SDL_SetJoystickLED(SDL_Joystick *a, Uint8 b, Uint8 c, Uint8 d) { return false; }
__attribute__((weak)) bool SDL_SetJoystickPlayerIndex(SDL_Joystick *a, int b) { return false; }
__attribute__((weak)) bool SDL_SetJoystickVirtualAxis(SDL_Joystick *a, int b, Sint16 c) { return false; }
__attribute__((weak)) bool SDL_SetJoystickVirtualBall(SDL_Joystick *a, int b, Sint16 c, Sint16 d) { return false; }
__attribute__((weak)) bool SDL_SetJoystickVirtualButton(SDL_Joystick *a, int b, bool c) { return false; }
__attribute__((weak)) bool SDL_SetJoystickVirtualHat(SDL_Joystick *a, int b, Uint8 c) { return false; }
__attribute__((weak)) bool SDL_SetJoystickVirtualTouchpad(SDL_Joystick *a, int b, int c, bool d, float e, float f, float g) { return false; }
__attribute__((weak)) bool SDL_SetLinuxThreadPriority(Sint64 a, int b) { return false; }
__attribute__((weak)) bool SDL_SetLinuxThreadPriorityAndPolicy(Sint64 a, int b, int c) { return false; }
__attribute__((weak)) void SDL_SetLogOutputFunction(SDL_LogOutputFunction a, void *b) { return (void)0; }
__attribute__((weak)) void SDL_SetLogPriorities(SDL_LogPriority a) { return (void)0; }
__attribute__((weak)) void SDL_SetLogPriority(int a, SDL_LogPriority b) { return (void)0; }
__attribute__((weak)) bool SDL_SetLogPriorityPrefix(SDL_LogPriority a, const char *b) { return false; }
__attribute__((weak)) void SDL_SetMainReady(void) { return (void)0; }
__attribute__((weak)) bool SDL_SetMemoryFunctions(SDL_malloc_func a, SDL_calloc_func b, SDL_realloc_func c, SDL_free_func d) { return false; }
__attribute__((weak)) void SDL_SetModState(SDL_Keymod a) { return (void)0; }
__attribute__((weak)) bool SDL_SetNumberProperty(SDL_PropertiesID a, const char *b, Sint64 c) { return false; }
__attribute__((weak)) bool SDL_SetPaletteColors(SDL_Palette *a, const SDL_Color *b, int c, int d) { return false; }
__attribute__((weak)) bool SDL_SetPointerProperty(SDL_PropertiesID a, const char *b, void *c) { return false; }
__attribute__((weak)) bool SDL_SetPointerPropertyWithCleanup(SDL_PropertiesID a, const char *b, void *c, SDL_CleanupPropertyCallback d, void *e) { return false; }
__attribute__((weak)) bool SDL_SetPrimarySelectionText(const char *a) { return false; }
__attribute__((weak)) bool SDL_SetRenderClipRect(SDL_Renderer *a, const SDL_Rect *b) { return false; }
__attribute__((weak)) bool SDL_SetRenderColorScale(SDL_Renderer *a, float b) { return false; }
__attribute__((weak)) bool SDL_SetRenderDrawBlendMode(SDL_Renderer *a, SDL_BlendMode b) { return false; }
__attribute__((weak)) bool SDL_SetRenderDrawColor(SDL_Renderer *a, Uint8 b, Uint8 c, Uint8 d, Uint8 e) { return false; }
__attribute__((weak)) bool SDL_SetRenderDrawColorFloat(SDL_Renderer *a, float b, float c, float d, float e) { return false; }
__attribute__((weak)) bool SDL_SetRenderLogicalPresentation(SDL_Renderer *a, int b, int c, SDL_RendererLogicalPresentation d) { return false; }
__attribute__((weak)) bool SDL_SetRenderScale(SDL_Renderer *a, float b, float c) { return false; }
__attribute__((weak)) bool SDL_SetRenderTarget(SDL_Renderer *a, SDL_Texture *b) { return false; }
__attribute__((weak)) bool SDL_SetRenderVSync(SDL_Renderer *a, int b) { return false; }
__attribute__((weak)) bool SDL_SetRenderViewport(SDL_Renderer *a, const SDL_Rect *b) { return false; }
__attribute__((weak)) bool SDL_SetScancodeName(SDL_Scancode a, const char *b) { return false; }
__attribute__((weak)) bool SDL_SetStringProperty(SDL_PropertiesID a, const char *b, const char *c) { return false; }
__attribute__((weak)) bool SDL_SetSurfaceAlphaMod(SDL_Surface *a, Uint8 b) { return false; }
__attribute__((weak)) bool SDL_SetSurfaceBlendMode(SDL_Surface *a, SDL_BlendMode b) { return false; }
__attribute__((weak)) bool SDL_SetSurfaceClipRect(SDL_Surface *a, const SDL_Rect *b) { return false; }
__attribute__((weak)) bool SDL_SetSurfaceColorKey(SDL_Surface *a, bool b, Uint32 c) { return false; }
__attribute__((weak)) bool SDL_SetSurfaceColorMod(SDL_Surface *a, Uint8 b, Uint8 c, Uint8 d) { return false; }
__attribute__((weak)) bool SDL_SetSurfaceColorspace(SDL_Surface *a, SDL_Colorspace b) { return false; }
__attribute__((weak)) bool SDL_SetSurfacePalette(SDL_Surface *a, SDL_Palette *b) { return false; }
__attribute__((weak)) bool SDL_SetSurfaceRLE(SDL_Surface *a, bool b) { return false; }
__attribute__((weak)) bool SDL_SetTLS(SDL_TLSID *a, const void *b, SDL_TLSDestructorCallback c) { return false; }
__attribute__((weak)) bool SDL_SetTextInputArea(SDL_Window *a, const SDL_Rect *b, int c) { return false; }
__attribute__((weak)) bool SDL_SetTextureAlphaMod(SDL_Texture *a, Uint8 b) { return false; }
__attribute__((weak)) bool SDL_SetTextureAlphaModFloat(SDL_Texture *a, float b) { return false; }
__attribute__((weak)) bool SDL_SetTextureBlendMode(SDL_Texture *a, SDL_BlendMode b) { return false; }
__attribute__((weak)) bool SDL_SetTextureColorMod(SDL_Texture *a, Uint8 b, Uint8 c, Uint8 d) { return false; }
__attribute__((weak)) bool SDL_SetTextureColorModFloat(SDL_Texture *a, float b, float c, float d) { return false; }
__attribute__((weak)) bool SDL_SetTextureScaleMode(SDL_Texture *a, SDL_ScaleMode b) { return false; }
__attribute__((weak)) bool SDL_SetWindowAlwaysOnTop(SDL_Window *a, bool b) { return false; }
__attribute__((weak)) bool SDL_SetWindowAspectRatio(SDL_Window *a, float b, float c) { return false; }
__attribute__((weak)) bool SDL_SetWindowBordered(SDL_Window *a, bool b) { return false; }
__attribute__((weak)) bool SDL_SetWindowFocusable(SDL_Window *a, bool b) { return false; }
__attribute__((weak)) bool SDL_SetWindowFullscreen(SDL_Window *a, bool b) { return false; }
__attribute__((weak)) bool SDL_SetWindowFullscreenMode(SDL_Window *a, const SDL_DisplayMode *b) { return false; }
__attribute__((weak)) bool SDL_SetWindowHitTest(SDL_Window *a, SDL_HitTest b, void *c) { return false; }
__attribute__((weak)) bool SDL_SetWindowIcon(SDL_Window *a, SDL_Surface *b) { return false; }
__attribute__((weak)) bool SDL_SetWindowKeyboardGrab(SDL_Window *a, bool b) { return false; }
__attribute__((weak)) bool SDL_SetWindowMaximumSize(SDL_Window *a, int b, int c) { return false; }
__attribute__((weak)) bool SDL_SetWindowMinimumSize(SDL_Window *a, int b, int c) { return false; }
__attribute__((weak)) bool SDL_SetWindowModal(SDL_Window *a, bool b) { return false; }
__attribute__((weak)) bool SDL_SetWindowMouseGrab(SDL_Window *a, bool b) { return false; }
__attribute__((weak)) bool SDL_SetWindowMouseRect(SDL_Window *a, const SDL_Rect *b) { return false; }
__attribute__((weak)) bool SDL_SetWindowOpacity(SDL_Window *a, float b) { return false; }
__attribute__((weak)) bool SDL_SetWindowParent(SDL_Window *a, SDL_Window *b) { return false; }
__attribute__((weak)) bool SDL_SetWindowPosition(SDL_Window *a, int b, int c) { return false; }
__attribute__((weak)) bool SDL_SetWindowRelativeMouseMode(SDL_Window *a, bool b) { return false; }
__attribute__((weak)) bool SDL_SetWindowResizable(SDL_Window *a, bool b) { return false; }
__attribute__((weak)) bool SDL_SetWindowShape(SDL_Window *a, SDL_Surface *b) { return false; }
__attribute__((weak)) bool SDL_SetWindowSize(SDL_Window *a, int b, int c) { return false; }
__attribute__((weak)) bool SDL_SetWindowSurfaceVSync(SDL_Window *a, int b) { return false; }
__attribute__((weak)) bool SDL_SetWindowTitle(SDL_Window *a, const char *b) { return false; }
__attribute__((weak)) void SDL_SetWindowsMessageHook(SDL_WindowsMessageHook a, void *b) { return (void)0; }
__attribute__((weak)) void SDL_SetX11EventHook(SDL_X11EventHook a, void *b) { return (void)0; }
__attribute__((weak)) bool SDL_SetiOSAnimationCallback(SDL_Window *a, int b, SDL_iOSAnimationCallback c, void *d) { return false; }
__attribute__((weak)) void SDL_SetiOSEventPump(bool a) { return (void)0; }
__attribute__((weak)) bool SDL_ShouldInit(SDL_InitState *a) { return false; }
__attribute__((weak)) bool SDL_ShouldQuit(SDL_InitState *a) { return false; }
__attribute__((weak)) bool SDL_ShowAndroidToast(const char *a, int b, int c, int d, int e) { return false; }
__attribute__((weak)) bool SDL_ShowCursor(void) { return false; }
__attribute__((weak)) bool SDL_ShowMessageBox(const SDL_MessageBoxData *a, int *b) { return false; }
__attribute__((weak)) void SDL_ShowOpenFileDialog(SDL_DialogFileCallback a, void *b, SDL_Window *c, const SDL_DialogFileFilter *d, int e, const char *f, bool g) { return (void)0; }
__attribute__((weak)) void SDL_ShowOpenFolderDialog(SDL_DialogFileCallback a, void *b, SDL_Window *c, const char *d, bool e) { return (void)0; }
__attribute__((weak)) void SDL_ShowSaveFileDialog(SDL_DialogFileCallback a, void *b, SDL_Window *c, const SDL_DialogFileFilter *d, int e, const char *f) { return (void)0; }
__attribute__((weak)) bool SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags a, const char *b, const char *c, SDL_Window *d) { return false; }
__attribute__((weak)) bool SDL_ShowWindow(SDL_Window *a) { return false; }
__attribute__((weak)) bool SDL_ShowWindowSystemMenu(SDL_Window *a, int b, int c) { return false; }
__attribute__((weak)) void SDL_SignalCondition(SDL_Condition *a) { return (void)0; }
__attribute__((weak)) void SDL_SignalSemaphore(SDL_Semaphore *a) { return (void)0; }
__attribute__((weak)) bool SDL_StartTextInput(SDL_Window *a) { return false; }
__attribute__((weak)) bool SDL_StartTextInputWithProperties(SDL_Window *a, SDL_PropertiesID b) { return false; }
__attribute__((weak)) Uint32 SDL_StepUTF8(const char **a, size_t *b) { return 0; }
__attribute__((weak)) bool SDL_StopHapticEffect(SDL_Haptic *a, SDL_HapticEffectID b) { return false; }
__attribute__((weak)) bool SDL_StopHapticEffects(SDL_Haptic *a) { return false; }
__attribute__((weak)) bool SDL_StopHapticRumble(SDL_Haptic *a) { return false; }
__attribute__((weak)) bool SDL_StopTextInput(SDL_Window *a) { return false; }
__attribute__((weak)) bool SDL_StorageReady(SDL_Storage *a) { return false; }
__attribute__((weak)) SDL_GUID SDL_StringToGUID(const char *a) { return {0}; }
__attribute__((weak)) bool SDL_SubmitGPUCommandBuffer(SDL_GPUCommandBuffer *a) { return false; }
__attribute__((weak)) SDL_GPUFence* SDL_SubmitGPUCommandBufferAndAcquireFence(SDL_GPUCommandBuffer *a) { return nullptr; }
__attribute__((weak)) bool SDL_SurfaceHasAlternateImages(SDL_Surface *a) { return false; }
__attribute__((weak)) bool SDL_SurfaceHasColorKey(SDL_Surface *a) { return false; }
__attribute__((weak)) bool SDL_SurfaceHasRLE(SDL_Surface *a) { return false; }
__attribute__((weak)) bool SDL_SyncWindow(SDL_Window *a) { return false; }
__attribute__((weak)) Sint64 SDL_TellIO(SDL_IOStream *a) { return 0; }
__attribute__((weak)) bool SDL_TextInputActive(SDL_Window *a) { return false; }
__attribute__((weak)) SDL_Time SDL_TimeFromWindows(Uint32 a, Uint32 b) { return (SDL_Time)0; }
__attribute__((weak)) bool SDL_TimeToDateTime(SDL_Time a, SDL_DateTime *b, bool c) { return false; }
__attribute__((weak)) void SDL_TimeToWindows(SDL_Time a, Uint32 *b, Uint32 *c) { return (void)0; }
__attribute__((weak)) bool SDL_TryLockMutex(SDL_Mutex *a) { return false; }
__attribute__((weak)) bool SDL_TryLockRWLockForReading(SDL_RWLock *a) { return false; }
__attribute__((weak)) bool SDL_TryLockRWLockForWriting(SDL_RWLock *a) { return false; }
__attribute__((weak)) bool SDL_TryLockSpinlock(SDL_SpinLock *a) { return false; }
__attribute__((weak)) bool SDL_TryWaitSemaphore(SDL_Semaphore *a) { return false; }
__attribute__((weak)) char* SDL_UCS4ToUTF8(Uint32 a, char *b) { return nullptr; }
__attribute__((weak)) void SDL_UnbindAudioStream(SDL_AudioStream *a) { return (void)0; }
__attribute__((weak)) void SDL_UnbindAudioStreams(SDL_AudioStream * const *a, int b) { return (void)0; }
__attribute__((weak)) void SDL_UnloadObject(SDL_SharedObject *a) { return (void)0; }
__attribute__((weak)) bool SDL_UnlockAudioStream(SDL_AudioStream *a) { return false; }
__attribute__((weak)) void SDL_UnlockJoysticks(void) { return (void)0; }
__attribute__((weak)) void SDL_UnlockMutex(SDL_Mutex *a) { return (void)0; }
__attribute__((weak)) void SDL_UnlockProperties(SDL_PropertiesID a) { return (void)0; }
__attribute__((weak)) void SDL_UnlockRWLock(SDL_RWLock *a) { return (void)0; }
__attribute__((weak)) void SDL_UnlockSpinlock(SDL_SpinLock *a) { return (void)0; }
__attribute__((weak)) void SDL_UnlockSurface(SDL_Surface *a) { return (void)0; }
__attribute__((weak)) void SDL_UnlockTexture(SDL_Texture *a) { return (void)0; }
__attribute__((weak)) void SDL_UnmapGPUTransferBuffer(SDL_GPUDevice *a, SDL_GPUTransferBuffer *b) { return (void)0; }
__attribute__((weak)) void SDL_UnregisterApp(void) { return (void)0; }
__attribute__((weak)) bool SDL_UnsetEnvironmentVariable(SDL_Environment *a, const char *b) { return false; }
__attribute__((weak)) void SDL_UpdateGamepads(void) { return (void)0; }
__attribute__((weak)) bool SDL_UpdateHapticEffect(SDL_Haptic *a, SDL_HapticEffectID b, const SDL_HapticEffect *c) { return false; }
__attribute__((weak)) void SDL_UpdateJoysticks(void) { return (void)0; }
__attribute__((weak)) bool SDL_UpdateNVTexture(SDL_Texture *a, const SDL_Rect *b, const Uint8 *c, int d, const Uint8 *e, int f) { return false; }
__attribute__((weak)) void SDL_UpdateSensors(void) { return (void)0; }
__attribute__((weak)) bool SDL_UpdateTexture(SDL_Texture *a, const SDL_Rect *b, const void *c, int d) { return false; }
__attribute__((weak)) bool SDL_UpdateWindowSurface(SDL_Window *a) { return false; }
__attribute__((weak)) bool SDL_UpdateWindowSurfaceRects(SDL_Window *a, const SDL_Rect *b, int c) { return false; }
__attribute__((weak)) bool SDL_UpdateYUVTexture(SDL_Texture *a, const SDL_Rect *b, const Uint8 *c, int d, const Uint8 *e, int f, const Uint8 *g, int h) { return false; }
__attribute__((weak)) void SDL_UploadToGPUBuffer(SDL_GPUCopyPass *a, const SDL_GPUTransferBufferLocation *b, const SDL_GPUBufferRegion *c, bool d) { return (void)0; }
__attribute__((weak)) void SDL_UploadToGPUTexture(SDL_GPUCopyPass *a, const SDL_GPUTextureTransferInfo *b, const SDL_GPUTextureRegion *c, bool d) { return (void)0; }
__attribute__((weak)) bool SDL_Vulkan_CreateSurface(SDL_Window *a, VkInstance b, const struct VkAllocationCallbacks *c, VkSurfaceKHR *d) { return false; }
__attribute__((weak)) void SDL_Vulkan_DestroySurface(VkInstance a, VkSurfaceKHR b, const struct VkAllocationCallbacks *c) { return (void)0; }
__attribute__((weak)) char const* const* SDL_Vulkan_GetInstanceExtensions(Uint32 *a) { return nullptr; }
__attribute__((weak)) bool SDL_Vulkan_GetPresentationSupport(VkInstance a, VkPhysicalDevice b, Uint32 c) { return false; }
__attribute__((weak)) SDL_FunctionPointer SDL_Vulkan_GetVkGetInstanceProcAddr(void) { return (SDL_FunctionPointer)0; }
__attribute__((weak)) bool SDL_Vulkan_LoadLibrary(const char *a) { return false; }
__attribute__((weak)) void SDL_Vulkan_UnloadLibrary(void) { return (void)0; }
__attribute__((weak)) void SDL_WaitCondition(SDL_Condition *a, SDL_Mutex *b) { return (void)0; }
__attribute__((weak)) bool SDL_WaitConditionTimeout(SDL_Condition *a, SDL_Mutex *b, Sint32 c) { return false; }
__attribute__((weak)) bool SDL_WaitEvent(SDL_Event *a) { return false; }
__attribute__((weak)) bool SDL_WaitEventTimeout(SDL_Event *a, Sint32 b) { return false; }
__attribute__((weak)) bool SDL_WaitForGPUFences(SDL_GPUDevice *a, bool b, SDL_GPUFence *const *c, Uint32 d) { return false; }
__attribute__((weak)) bool SDL_WaitForGPUIdle(SDL_GPUDevice *a) { return false; }
__attribute__((weak)) bool SDL_WaitProcess(SDL_Process *a, bool b, int *c) { return false; }
__attribute__((weak)) void SDL_WaitSemaphore(SDL_Semaphore *a) { return (void)0; }
__attribute__((weak)) bool SDL_WaitSemaphoreTimeout(SDL_Semaphore *a, Sint32 b) { return false; }
__attribute__((weak)) void SDL_WaitThread(SDL_Thread *a, int *b) { return (void)0; }
__attribute__((weak)) bool SDL_WarpMouseGlobal(float a, float b) { return false; }
__attribute__((weak)) void SDL_WarpMouseInWindow(SDL_Window *a, float b, float c) { return (void)0; }
__attribute__((weak)) SDL_InitFlags SDL_WasInit(SDL_InitFlags a) { return (SDL_InitFlags)0; }
__attribute__((weak)) bool SDL_WindowHasSurface(SDL_Window *a) { return false; }
__attribute__((weak)) bool SDL_WindowSupportsGPUPresentMode(SDL_GPUDevice *a, SDL_Window *b, SDL_GPUPresentMode c) { return false; }
__attribute__((weak)) bool SDL_WindowSupportsGPUSwapchainComposition(SDL_GPUDevice *a, SDL_Window *b, SDL_GPUSwapchainComposition c) { return false; }
__attribute__((weak)) size_t SDL_WriteIO(SDL_IOStream *a, const void *b, size_t c) { return 0; }
__attribute__((weak)) bool SDL_WriteS16BE(SDL_IOStream *a, Sint16 b) { return false; }
__attribute__((weak)) bool SDL_WriteS16LE(SDL_IOStream *a, Sint16 b) { return false; }
__attribute__((weak)) bool SDL_WriteS32BE(SDL_IOStream *a, Sint32 b) { return false; }
__attribute__((weak)) bool SDL_WriteS32LE(SDL_IOStream *a, Sint32 b) { return false; }
__attribute__((weak)) bool SDL_WriteS64BE(SDL_IOStream *a, Sint64 b) { return false; }
__attribute__((weak)) bool SDL_WriteS64LE(SDL_IOStream *a, Sint64 b) { return false; }
__attribute__((weak)) bool SDL_WriteS8(SDL_IOStream *a, Sint8 b) { return false; }
__attribute__((weak)) bool SDL_WriteStorageFile(SDL_Storage *a, const char *b, const void *c, Uint64 d) { return false; }
__attribute__((weak)) bool SDL_WriteSurfacePixel(SDL_Surface *a, int b, int c, Uint8 d, Uint8 e, Uint8 f, Uint8 g) { return false; }
__attribute__((weak)) bool SDL_WriteSurfacePixelFloat(SDL_Surface *a, int b, int c, float d, float e, float f, float g) { return false; }
__attribute__((weak)) bool SDL_WriteU16BE(SDL_IOStream *a, Uint16 b) { return false; }
__attribute__((weak)) bool SDL_WriteU16LE(SDL_IOStream *a, Uint16 b) { return false; }
__attribute__((weak)) bool SDL_WriteU32BE(SDL_IOStream *a, Uint32 b) { return false; }
__attribute__((weak)) bool SDL_WriteU32LE(SDL_IOStream *a, Uint32 b) { return false; }
__attribute__((weak)) bool SDL_WriteU64BE(SDL_IOStream *a, Uint64 b) { return false; }
__attribute__((weak)) bool SDL_WriteU64LE(SDL_IOStream *a, Uint64 b) { return false; }
__attribute__((weak)) bool SDL_WriteU8(SDL_IOStream *a, Uint8 b) { return false; }
__attribute__((weak)) int SDL_abs(int a) { return 0; }
__attribute__((weak)) double SDL_acos(double a) { return 0.0; }
__attribute__((weak)) float SDL_acosf(float a) { return 0.0f; }
__attribute__((weak)) void* SDL_aligned_alloc(size_t a, size_t b) { return nullptr; }
__attribute__((weak)) void SDL_aligned_free(void *a) { return (void)0; }
__attribute__((weak)) double SDL_asin(double a) { return 0.0; }
__attribute__((weak)) float SDL_asinf(float a) { return 0.0f; }
__attribute__((weak)) double SDL_atan(double a) { return 0.0; }
__attribute__((weak)) double SDL_atan2(double a, double b) { return 0.0; }
__attribute__((weak)) float SDL_atan2f(float a, float b) { return 0.0f; }
__attribute__((weak)) float SDL_atanf(float a) { return 0.0f; }
__attribute__((weak)) double SDL_atof(const char *a) { return 0.0; }
__attribute__((weak)) int SDL_atoi(const char *a) { return 0; }
__attribute__((weak)) void* SDL_bsearch(const void *a, const void *b, size_t c, size_t d, SDL_CompareCallback e) { return nullptr; }
__attribute__((weak)) void* SDL_bsearch_r(const void *a, const void *b, size_t c, size_t d, SDL_CompareCallback_r e, void *f) { return nullptr; }
__attribute__((weak)) void* SDL_calloc(size_t a, size_t b) { return nullptr; }
__attribute__((weak)) double SDL_ceil(double a) { return 0.0; }
__attribute__((weak)) float SDL_ceilf(float a) { return 0.0f; }
__attribute__((weak)) double SDL_copysign(double a, double b) { return 0.0; }
__attribute__((weak)) float SDL_copysignf(float a, float b) { return 0.0f; }
__attribute__((weak)) double SDL_cos(double a) { return 0.0; }
__attribute__((weak)) float SDL_cosf(float a) { return 0.0f; }
__attribute__((weak)) Uint16 SDL_crc16(Uint16 a, const void *b, size_t c) { return 0; }
__attribute__((weak)) Uint32 SDL_crc32(Uint32 a, const void *b, size_t c) { return 0; }
__attribute__((weak)) double SDL_exp(double a) { return 0.0; }
__attribute__((weak)) float SDL_expf(float a) { return 0.0f; }
__attribute__((weak)) double SDL_fabs(double a) { return 0.0; }
__attribute__((weak)) float SDL_fabsf(float a) { return 0.0f; }
__attribute__((weak)) double SDL_floor(double a) { return 0.0; }
__attribute__((weak)) float SDL_floorf(float a) { return 0.0f; }
__attribute__((weak)) double SDL_fmod(double a, double b) { return 0.0; }
__attribute__((weak)) float SDL_fmodf(float a, float b) { return 0.0f; }
__attribute__((weak)) void SDL_free(void *a) { return (void)0; }
__attribute__((weak)) const char* SDL_getenv(const char *a) { return nullptr; }
__attribute__((weak)) const char* SDL_getenv_unsafe(const char *a) { return nullptr; }
__attribute__((weak)) void SDL_hid_ble_scan(bool a) { return (void)0; }
__attribute__((weak)) int SDL_hid_close(SDL_hid_device *a) { return 0; }
__attribute__((weak)) Uint32 SDL_hid_device_change_count(void) { return 0; }
__attribute__((weak)) SDL_hid_device_info* SDL_hid_enumerate(unsigned short a, unsigned short b) { return nullptr; }
__attribute__((weak)) int SDL_hid_exit(void) { return 0; }
__attribute__((weak)) void SDL_hid_free_enumeration(SDL_hid_device_info *a) { return (void)0; }
__attribute__((weak)) SDL_hid_device_info* SDL_hid_get_device_info(SDL_hid_device *a) { return nullptr; }
__attribute__((weak)) int SDL_hid_get_feature_report(SDL_hid_device *a, unsigned char *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_hid_get_indexed_string(SDL_hid_device *a, int b, wchar_t *c, size_t d) { return 0; }
__attribute__((weak)) int SDL_hid_get_input_report(SDL_hid_device *a, unsigned char *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_hid_get_manufacturer_string(SDL_hid_device *a, wchar_t *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_hid_get_product_string(SDL_hid_device *a, wchar_t *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_hid_get_report_descriptor(SDL_hid_device *a, unsigned char *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_hid_get_serial_number_string(SDL_hid_device *a, wchar_t *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_hid_init(void) { return 0; }
__attribute__((weak)) SDL_hid_device* SDL_hid_open(unsigned short a, unsigned short b, const wchar_t *c) { return nullptr; }
__attribute__((weak)) SDL_hid_device* SDL_hid_open_path(const char *a) { return nullptr; }
__attribute__((weak)) int SDL_hid_read(SDL_hid_device *a, unsigned char *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_hid_read_timeout(SDL_hid_device *a, unsigned char *b, size_t c, int d) { return 0; }
__attribute__((weak)) int SDL_hid_send_feature_report(SDL_hid_device *a, const unsigned char *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_hid_set_nonblocking(SDL_hid_device *a, int b) { return 0; }
__attribute__((weak)) int SDL_hid_write(SDL_hid_device *a, const unsigned char *b, size_t c) { return 0; }
__attribute__((weak)) size_t SDL_iconv(SDL_iconv_t a, const char **b, size_t *c, char **d, size_t *e) { return 0; }
__attribute__((weak)) int SDL_iconv_close(SDL_iconv_t a) { return 0; }
__attribute__((weak)) SDL_iconv_t SDL_iconv_open(const char *a, const char *b) { return (SDL_iconv_t)0; }
__attribute__((weak)) char* SDL_iconv_string(const char *a, const char *b, const char *c, size_t d) { return nullptr; }
__attribute__((weak)) int SDL_isalnum(int a) { return 0; }
__attribute__((weak)) int SDL_isalpha(int a) { return 0; }
__attribute__((weak)) int SDL_isblank(int a) { return 0; }
__attribute__((weak)) int SDL_iscntrl(int a) { return 0; }
__attribute__((weak)) int SDL_isdigit(int a) { return 0; }
__attribute__((weak)) int SDL_isgraph(int a) { return 0; }
__attribute__((weak)) int SDL_isinf(double a) { return 0; }
__attribute__((weak)) int SDL_isinff(float a) { return 0; }
__attribute__((weak)) int SDL_islower(int a) { return 0; }
__attribute__((weak)) int SDL_isnan(double a) { return 0; }
__attribute__((weak)) int SDL_isnanf(float a) { return 0; }
__attribute__((weak)) int SDL_isprint(int a) { return 0; }
__attribute__((weak)) int SDL_ispunct(int a) { return 0; }
__attribute__((weak)) int SDL_isspace(int a) { return 0; }
__attribute__((weak)) int SDL_isupper(int a) { return 0; }
__attribute__((weak)) int SDL_isxdigit(int a) { return 0; }
__attribute__((weak)) char* SDL_itoa(int a, char *b, int c) { return nullptr; }
__attribute__((weak)) char* SDL_lltoa(long long a, char *b, int c) { return nullptr; }
__attribute__((weak)) double SDL_log(double a) { return 0.0; }
__attribute__((weak)) double SDL_log10(double a) { return 0.0; }
__attribute__((weak)) float SDL_log10f(float a) { return 0.0f; }
__attribute__((weak)) float SDL_logf(float a) { return 0.0f; }
__attribute__((weak)) long SDL_lround(double a) { return 0; }
__attribute__((weak)) long SDL_lroundf(float a) { return 0; }
__attribute__((weak)) char* SDL_ltoa(long a, char *b, int c) { return nullptr; }
__attribute__((weak)) void* SDL_malloc(size_t a) { return nullptr; }
__attribute__((weak)) int SDL_memcmp(const void *a, const void *b, size_t c) { return 0; }
__attribute__((weak)) void* SDL_memcpy(void *a,  const void *b, size_t c) { return nullptr; }
__attribute__((weak)) void* SDL_memmove(void *a,  const void *b, size_t c) { return nullptr; }
__attribute__((weak)) void* SDL_memset(void *a, int b, size_t c) { return nullptr; }
__attribute__((weak)) void* SDL_memset4(void *a, Uint32 b, size_t c) { return nullptr; }
__attribute__((weak)) double SDL_modf(double a, double *b) { return 0.0; }
__attribute__((weak)) float SDL_modff(float a, float *b) { return 0.0f; }
__attribute__((weak)) Uint32 SDL_murmur3_32(const void *a, size_t b, Uint32 c) { return 0; }
__attribute__((weak)) double SDL_pow(double a, double b) { return 0.0; }
__attribute__((weak)) float SDL_powf(float a, float b) { return 0.0f; }
__attribute__((weak)) void SDL_qsort(void *a, size_t b, size_t c, SDL_CompareCallback d) { return (void)0; }
__attribute__((weak)) void SDL_qsort_r(void *a, size_t b, size_t c, SDL_CompareCallback_r d, void *e) { return (void)0; }
__attribute__((weak)) Sint32 SDL_rand(Sint32 a) { return 0; }
__attribute__((weak)) Uint32 SDL_rand_bits(void) { return 0; }
__attribute__((weak)) Uint32 SDL_rand_bits_r(Uint64 *a) { return 0; }
__attribute__((weak)) Sint32 SDL_rand_r(Uint64 *a, Sint32 b) { return 0; }
__attribute__((weak)) float SDL_randf(void) { return 0.0f; }
__attribute__((weak)) float SDL_randf_r(Uint64 *a) { return 0.0f; }
__attribute__((weak)) void* SDL_realloc(void *a, size_t b) { return nullptr; }
__attribute__((weak)) double SDL_round(double a) { return 0.0; }
__attribute__((weak)) float SDL_roundf(float a) { return 0.0f; }
__attribute__((weak)) double SDL_scalbn(double a, int b) { return 0.0; }
__attribute__((weak)) float SDL_scalbnf(float a, int b) { return 0.0f; }
__attribute__((weak)) int SDL_setenv_unsafe(const char *a, const char *b, int c) { return 0; }
__attribute__((weak)) double SDL_sin(double a) { return 0.0; }
__attribute__((weak)) float SDL_sinf(float a) { return 0.0f; }
__attribute__((weak)) double SDL_sqrt(double a) { return 0.0; }
__attribute__((weak)) float SDL_sqrtf(float a) { return 0.0f; }
__attribute__((weak)) void SDL_srand(Uint64 a) { return (void)0; }
__attribute__((weak)) int SDL_strcasecmp(const char *a, const char *b) { return 0; }
__attribute__((weak)) char* SDL_strcasestr(const char *a, const char *b) { return nullptr; }
__attribute__((weak)) char* SDL_strchr(const char *a, int b) { return nullptr; }
__attribute__((weak)) int SDL_strcmp(const char *a, const char *b) { return 0; }
__attribute__((weak)) char* SDL_strdup(const char *a) { return nullptr; }
__attribute__((weak)) size_t SDL_strlcat(char *a, const char *b, size_t c) { return 0; }
__attribute__((weak)) size_t SDL_strlcpy(char *a, const char *b, size_t c) { return 0; }
__attribute__((weak)) size_t SDL_strlen(const char *a) { return 0; }
__attribute__((weak)) char* SDL_strlwr(char *a) { return nullptr; }
__attribute__((weak)) int SDL_strncasecmp(const char *a, const char *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_strncmp(const char *a, const char *b, size_t c) { return 0; }
__attribute__((weak)) char* SDL_strndup(const char *a, size_t b) { return nullptr; }
__attribute__((weak)) size_t SDL_strnlen(const char *a, size_t b) { return 0; }
__attribute__((weak)) char* SDL_strnstr(const char *a, const char *b, size_t c) { return nullptr; }
__attribute__((weak)) char* SDL_strpbrk(const char *a, const char *b) { return nullptr; }
__attribute__((weak)) char* SDL_strrchr(const char *a, int b) { return nullptr; }
__attribute__((weak)) char* SDL_strrev(char *a) { return nullptr; }
__attribute__((weak)) char* SDL_strstr(const char *a, const char *b) { return nullptr; }
__attribute__((weak)) double SDL_strtod(const char *a, char **b) { return 0.0; }
__attribute__((weak)) char* SDL_strtok_r(char *a, const char *b, char **c) { return nullptr; }
__attribute__((weak)) long SDL_strtol(const char *a, char **b, int c) { return 0; }
__attribute__((weak)) long long SDL_strtoll(const char *a, char **b, int c) { return (long long)0; }
__attribute__((weak)) unsigned long SDL_strtoul(const char *a, char **b, int c) { return (unsigned long)0; }
__attribute__((weak)) unsigned long long SDL_strtoull(const char *a, char **b, int c) { return (unsigned long long)0; }
__attribute__((weak)) char* SDL_strupr(char *a) { return nullptr; }
__attribute__((weak)) double SDL_tan(double a) { return 0.0; }
__attribute__((weak)) float SDL_tanf(float a) { return 0.0f; }
__attribute__((weak)) int SDL_tolower(int a) { return 0; }
__attribute__((weak)) int SDL_toupper(int a) { return 0; }
__attribute__((weak)) double SDL_trunc(double a) { return 0.0; }
__attribute__((weak)) float SDL_truncf(float a) { return 0.0f; }
__attribute__((weak)) char* SDL_uitoa(unsigned int a, char *b, int c) { return nullptr; }
__attribute__((weak)) char* SDL_ulltoa(unsigned long long a, char *b, int c) { return nullptr; }
__attribute__((weak)) char* SDL_ultoa(unsigned long a, char *b, int c) { return nullptr; }
__attribute__((weak)) int SDL_unsetenv_unsafe(const char *a) { return 0; }
__attribute__((weak)) size_t SDL_utf8strlcpy(char *a, const char *b, size_t c) { return 0; }
__attribute__((weak)) size_t SDL_utf8strlen(const char *a) { return 0; }
__attribute__((weak)) size_t SDL_utf8strnlen(const char *a, size_t b) { return 0; }
__attribute__((weak)) int SDL_vasprintf(char **a,  const char *b, va_list c) { return 0; }
__attribute__((weak)) int SDL_vsnprintf(char *a, size_t b,  const char *c, va_list d) { return 0; }
__attribute__((weak)) int SDL_vsscanf(const char *a,  const char *b, va_list c) { return 0; }
__attribute__((weak)) int SDL_vswprintf(wchar_t *a, size_t b, const wchar_t *c, va_list d) { return 0; }
__attribute__((weak)) int SDL_wcscasecmp(const wchar_t *a, const wchar_t *b) { return 0; }
__attribute__((weak)) int SDL_wcscmp(const wchar_t *a, const wchar_t *b) { return 0; }
__attribute__((weak)) wchar_t* SDL_wcsdup(const wchar_t *a) { return nullptr; }
__attribute__((weak)) size_t SDL_wcslcat(wchar_t *a, const wchar_t *b, size_t c) { return 0; }
__attribute__((weak)) size_t SDL_wcslcpy(wchar_t *a, const wchar_t *b, size_t c) { return 0; }
__attribute__((weak)) size_t SDL_wcslen(const wchar_t *a) { return 0; }
__attribute__((weak)) int SDL_wcsncasecmp(const wchar_t *a, const wchar_t *b, size_t c) { return 0; }
__attribute__((weak)) int SDL_wcsncmp(const wchar_t *a, const wchar_t *b, size_t c) { return 0; }
__attribute__((weak)) size_t SDL_wcsnlen(const wchar_t *a, size_t b) { return 0; }
__attribute__((weak)) wchar_t* SDL_wcsnstr(const wchar_t *a, const wchar_t *b, size_t c) { return nullptr; }
__attribute__((weak)) wchar_t* SDL_wcsstr(const wchar_t *a, const wchar_t *b) { return nullptr; }
__attribute__((weak)) long SDL_wcstol(const wchar_t *a, wchar_t **b, int c) { return 0; }
__attribute__((weak)) Uint32 SDL_StepBackUTF8(const char *a, const char **b) { return 0; }
__attribute__((weak)) void SDL_DelayPrecise(Uint64 a) { return (void)0; }
__attribute__((weak)) Uint32 SDL_CalculateGPUTextureFormatSize(SDL_GPUTextureFormat a, Uint32 b, Uint32 c, Uint32 d) { return 0; }
__attribute__((weak)) bool SDL_SetErrorV(const char *a,va_list b) { return false; }
__attribute__((weak)) SDL_LogOutputFunction SDL_GetDefaultLogOutputFunction(void) { return (SDL_LogOutputFunction)0; }
__attribute__((weak)) bool SDL_RenderDebugText(SDL_Renderer *a,float b,float c,const char *d) { return false; }
__attribute__((weak)) SDL_Sandbox SDL_GetSandbox(void) { return (SDL_Sandbox)0; }
__attribute__((weak)) bool SDL_CancelGPUCommandBuffer(SDL_GPUCommandBuffer *a) { return false; }
__attribute__((weak)) bool SDL_SaveFile_IO(SDL_IOStream *a,const void *b,size_t c,bool d) { return false; }
__attribute__((weak)) bool SDL_SaveFile(const char *a,const void *b,size_t c) { return false; }
__attribute__((weak)) char* SDL_GetCurrentDirectory(void) { return nullptr; }
__attribute__((weak)) bool SDL_IsAudioDevicePhysical(SDL_AudioDeviceID a) { return false; }
__attribute__((weak)) bool SDL_IsAudioDevicePlayback(SDL_AudioDeviceID a) { return false; }
__attribute__((weak)) SDL_AsyncIO* SDL_AsyncIOFromFile(const char *a, const char *b) { return nullptr; }
__attribute__((weak)) Sint64 SDL_GetAsyncIOSize(SDL_AsyncIO *a) { return 0; }
__attribute__((weak)) bool SDL_ReadAsyncIO(SDL_AsyncIO *a, void *b, Uint64 c, Uint64 d, SDL_AsyncIOQueue *e, void *f) { return false; }
__attribute__((weak)) bool SDL_WriteAsyncIO(SDL_AsyncIO *a, void *b, Uint64 c, Uint64 d, SDL_AsyncIOQueue *e, void *f) { return false; }
__attribute__((weak)) bool SDL_CloseAsyncIO(SDL_AsyncIO *a, bool b, SDL_AsyncIOQueue *c, void *d) { return false; }
__attribute__((weak)) SDL_AsyncIOQueue* SDL_CreateAsyncIOQueue(void) { return nullptr; }
__attribute__((weak)) void SDL_DestroyAsyncIOQueue(SDL_AsyncIOQueue *a) { return (void)0; }
__attribute__((weak)) bool SDL_GetAsyncIOResult(SDL_AsyncIOQueue *a, SDL_AsyncIOOutcome *b) { return false; }
__attribute__((weak)) bool SDL_WaitAsyncIOResult(SDL_AsyncIOQueue *a, SDL_AsyncIOOutcome *b, Sint32 c) { return false; }
__attribute__((weak)) void SDL_SignalAsyncIOQueue(SDL_AsyncIOQueue *a) { return (void)0; }
__attribute__((weak)) bool SDL_LoadFileAsync(const char *a, SDL_AsyncIOQueue *b, void *c) { return false; }
__attribute__((weak)) void SDL_ShowFileDialogWithProperties(SDL_FileDialogType a, SDL_DialogFileCallback b, void *c, SDL_PropertiesID d) { return (void)0; }
__attribute__((weak)) bool SDL_IsMainThread(void) { return false; }
__attribute__((weak)) bool SDL_RunOnMainThread(SDL_MainThreadCallback a,void *b,bool c) { return false; }
__attribute__((weak)) bool SDL_SetGPUAllowedFramesInFlight(SDL_GPUDevice *a,Uint32 b) { return false; }
__attribute__((weak)) bool SDL_RenderTextureAffine(SDL_Renderer *a,SDL_Texture *b,const SDL_FRect *c,const SDL_FPoint *d,const SDL_FPoint *e,const SDL_FPoint *f) { return false; }
__attribute__((weak)) bool SDL_WaitForGPUSwapchain(SDL_GPUDevice *a, SDL_Window *b) { return false; }
__attribute__((weak)) bool SDL_WaitAndAcquireGPUSwapchainTexture(SDL_GPUCommandBuffer *a,SDL_Window *b,SDL_GPUTexture **c,Uint32 *d,Uint32 *e) { return false; }
__attribute__((weak)) bool SDL_RenderDebugTextFormat(SDL_Renderer *a,float b,float c, const char *d,...) { return false; }
__attribute__((weak)) SDL_Tray* SDL_CreateTray(SDL_Surface *a,const char *b) { return nullptr; }
__attribute__((weak)) void SDL_SetTrayIcon(SDL_Tray *a,SDL_Surface *b) { return (void)0; }
__attribute__((weak)) void SDL_SetTrayTooltip(SDL_Tray *a,const char *b) { return (void)0; }
__attribute__((weak)) SDL_TrayMenu* SDL_CreateTrayMenu(SDL_Tray *a) { return nullptr; }
__attribute__((weak)) SDL_TrayMenu* SDL_CreateTraySubmenu(SDL_TrayEntry *a) { return nullptr; }
__attribute__((weak)) SDL_TrayMenu* SDL_GetTrayMenu(SDL_Tray *a) { return nullptr; }
__attribute__((weak)) SDL_TrayMenu* SDL_GetTraySubmenu(SDL_TrayEntry *a) { return nullptr; }
__attribute__((weak)) const SDL_TrayEntry** SDL_GetTrayEntries(SDL_TrayMenu *a,int *b) { return nullptr; }
__attribute__((weak)) void SDL_RemoveTrayEntry(SDL_TrayEntry *a) { return (void)0; }
__attribute__((weak)) SDL_TrayEntry* SDL_InsertTrayEntryAt(SDL_TrayMenu *a,int b,const char *c,SDL_TrayEntryFlags d) { return nullptr; }
__attribute__((weak)) void SDL_SetTrayEntryLabel(SDL_TrayEntry *a,const char *b) { return (void)0; }
__attribute__((weak)) const char* SDL_GetTrayEntryLabel(SDL_TrayEntry *a) { return nullptr; }
__attribute__((weak)) void SDL_SetTrayEntryChecked(SDL_TrayEntry *a,bool b) { return (void)0; }
__attribute__((weak)) bool SDL_GetTrayEntryChecked(SDL_TrayEntry *a) { return false; }
__attribute__((weak)) void SDL_SetTrayEntryEnabled(SDL_TrayEntry *a,bool b) { return (void)0; }
__attribute__((weak)) bool SDL_GetTrayEntryEnabled(SDL_TrayEntry *a) { return false; }
__attribute__((weak)) void SDL_SetTrayEntryCallback(SDL_TrayEntry *a,SDL_TrayCallback b,void *c) { return (void)0; }
__attribute__((weak)) void SDL_DestroyTray(SDL_Tray *a) { return (void)0; }
__attribute__((weak)) SDL_TrayMenu* SDL_GetTrayEntryParent(SDL_TrayEntry *a) { return nullptr; }
__attribute__((weak)) SDL_TrayEntry* SDL_GetTrayMenuParentEntry(SDL_TrayMenu *a) { return nullptr; }
__attribute__((weak)) SDL_Tray* SDL_GetTrayMenuParentTray(SDL_TrayMenu *a) { return nullptr; }
__attribute__((weak)) SDL_ThreadState SDL_GetThreadState(SDL_Thread *a) { return (SDL_ThreadState)0; }
__attribute__((weak)) bool SDL_AudioStreamDevicePaused(SDL_AudioStream *a) { return false; }
__attribute__((weak)) void SDL_ClickTrayEntry(SDL_TrayEntry *a) { return (void)0; }
__attribute__((weak)) void SDL_UpdateTrays(void) { return (void)0; }
__attribute__((weak)) bool SDL_StretchSurface(SDL_Surface *a,const SDL_Rect *b,SDL_Surface *c,const SDL_Rect *d,SDL_ScaleMode e) { return false; }
__attribute__((weak)) bool SDL_SetRelativeMouseTransform(SDL_MouseMotionTransformCallback a,void *b) { return false; }
__attribute__((weak)) bool SDL_RenderTexture9GridTiled(SDL_Renderer *a,SDL_Texture *b,const SDL_FRect *c,float d,float e,float f,float g,float h,const SDL_FRect *i,float j) { return false; }
__attribute__((weak)) bool SDL_SetDefaultTextureScaleMode(SDL_Renderer *a,SDL_ScaleMode b) { return false; }
__attribute__((weak)) bool SDL_GetDefaultTextureScaleMode(SDL_Renderer *a,SDL_ScaleMode *b) { return false; }
__attribute__((weak)) SDL_GPURenderState* SDL_CreateGPURenderState(SDL_Renderer *a,const SDL_GPURenderStateCreateInfo *b) { return nullptr; }
__attribute__((weak)) bool SDL_SetGPURenderStateFragmentUniforms(SDL_GPURenderState *a,Uint32 b,const void *c,Uint32 d) { return false; }
__attribute__((weak)) bool SDL_SetGPURenderState(SDL_Renderer *a,SDL_GPURenderState *b) { return false; }
__attribute__((weak)) void SDL_DestroyGPURenderState(SDL_GPURenderState *a) { return (void)0; }
__attribute__((weak)) bool SDL_SetWindowProgressState(SDL_Window *a,SDL_ProgressState b) { return false; }
__attribute__((weak)) bool SDL_SetWindowProgressValue(SDL_Window *a,float b) { return false; }
__attribute__((weak)) SDL_ProgressState SDL_GetWindowProgressState(SDL_Window *a) { return (SDL_ProgressState)0; }
__attribute__((weak)) float SDL_GetWindowProgressValue(SDL_Window *a) { return 0.0f; }
__attribute__((weak)) bool SDL_SetRenderTextureAddressMode(SDL_Renderer *a,SDL_TextureAddressMode b,SDL_TextureAddressMode c) { return false; }
__attribute__((weak)) bool SDL_GetRenderTextureAddressMode(SDL_Renderer *a,SDL_TextureAddressMode *b,SDL_TextureAddressMode *c) { return false; }
__attribute__((weak)) SDL_PropertiesID SDL_GetGPUDeviceProperties(SDL_GPUDevice *a) { return (SDL_PropertiesID)0; }
__attribute__((weak)) SDL_Renderer* SDL_CreateGPURenderer(SDL_GPUDevice *a,SDL_Window *b) { return nullptr; }
__attribute__((weak)) bool SDL_PutAudioStreamPlanarData(SDL_AudioStream *a,const void * const*b,int c,int d) { return false; }
__attribute__((weak)) int SDL_GetEventDescription(const SDL_Event *a,char *b,int c) { return 0; }
__attribute__((weak)) bool SDL_PutAudioStreamDataNoCopy(SDL_AudioStream *a,const void *b,int c,SDL_AudioStreamDataCompleteCallback d,void *e) { return false; }
__attribute__((weak)) Uint32 SDL_AddAtomicU32(SDL_AtomicU32 *a,int b) { return 0; }
__attribute__((weak)) SDL_PropertiesID SDL_hid_get_properties(SDL_hid_device *a) { return (SDL_PropertiesID)0; }
__attribute__((weak)) SDL_PixelFormat SDL_GetPixelFormatFromGPUTextureFormat(SDL_GPUTextureFormat a) { return (SDL_PixelFormat)0; }
__attribute__((weak)) SDL_GPUTextureFormat SDL_GetGPUTextureFormatFromPixelFormat(SDL_PixelFormat a) { return (SDL_GPUTextureFormat)0; }
__attribute__((weak)) Sint32 JNI_OnLoad(JavaVM *a, void *b) { return 0; }
__attribute__((weak)) bool SDL_SetTexturePalette(SDL_Texture *a,SDL_Palette *b) { return false; }
__attribute__((weak)) SDL_Palette* SDL_GetTexturePalette(SDL_Texture *a) { return nullptr; }
__attribute__((weak)) SDL_GPUDevice* SDL_GetGPURendererDevice(SDL_Renderer *a) { return nullptr; }
__attribute__((weak)) SDL_Surface* SDL_LoadPNG_IO(SDL_IOStream *a,bool b) { return nullptr; }
__attribute__((weak)) SDL_Surface* SDL_LoadPNG(const char *a) { return nullptr; }
__attribute__((weak)) bool SDL_SavePNG_IO(SDL_Surface *a,SDL_IOStream *b,bool c) { return false; }
__attribute__((weak)) bool SDL_SavePNG(SDL_Surface *a,const char *b) { return false; }
__attribute__((weak)) int SDL_GetSystemPageSize(void) { return 0; }
__attribute__((weak)) SDL_PenDeviceType SDL_GetPenDeviceType(SDL_PenID a) { return (SDL_PenDeviceType)0; }
__attribute__((weak)) SDL_Cursor* SDL_CreateAnimatedCursor(SDL_CursorFrameInfo *a,int b,int c,int d) { return nullptr; }
__attribute__((weak)) SDL_Surface* SDL_RotateSurface(SDL_Surface *a,float b) { return nullptr; }
__attribute__((weak)) SDL_Surface* SDL_LoadSurface_IO(SDL_IOStream *a,bool b) { return nullptr; }
__attribute__((weak)) SDL_Surface* SDL_LoadSurface(const char *a) { return nullptr; }
__attribute__((weak)) bool SDL_SetWindowFillDocument(SDL_Window *a,bool b) { return false; }
__attribute__((weak)) bool SDL_TryLockJoysticks(void) { return false; }
__attribute__((weak)) XrResult SDL_CreateGPUXRSession(SDL_GPUDevice *a,const XrSessionCreateInfo *b,XrSession *c) { return (XrResult)0; }
__attribute__((weak)) SDL_GPUTextureFormat* SDL_GetGPUXRSwapchainFormats(SDL_GPUDevice *a,XrSession b,int *c) { return nullptr; }
__attribute__((weak)) XrResult SDL_CreateGPUXRSwapchain(SDL_GPUDevice *a,XrSession b,const XrSwapchainCreateInfo *c,SDL_GPUTextureFormat d,XrSwapchain *e,SDL_GPUTexture ***f) { return (XrResult)0; }
__attribute__((weak)) XrResult SDL_DestroyGPUXRSwapchain(SDL_GPUDevice *a,XrSwapchain b,SDL_GPUTexture **c) { return (XrResult)0; }
__attribute__((weak)) bool SDL_OpenXR_LoadLibrary(void) { return false; }
__attribute__((weak)) void SDL_OpenXR_UnloadLibrary(void) { return (void)0; }
__attribute__((weak)) PFN_xrGetInstanceProcAddr SDL_OpenXR_GetXrGetInstanceProcAddr(void) { return (PFN_xrGetInstanceProcAddr)0; }
__attribute__((weak)) SDL_Tray* SDL_CreateTrayWithProperties(SDL_PropertiesID a) { return nullptr; }
__attribute__((weak)) bool SDL_SetGPURenderStateSamplerBindings(SDL_GPURenderState *a,int b,const SDL_GPUTextureSamplerBinding *c) { return false; }
__attribute__((weak)) bool SDL_SetGPURenderStateStorageTextures(SDL_GPURenderState *a,int b,SDL_GPUTexture *const*c) { return false; }
__attribute__((weak)) bool SDL_SetGPURenderStateStorageBuffers(SDL_GPURenderState *a,int b,SDL_GPUBuffer *const*c) { return false; }
__attribute__((weak)) void SDL_GDKSuspendRenderer(SDL_Renderer *a) { return (void)0; }
__attribute__((weak)) void SDL_GDKResumeRenderer(SDL_Renderer *a) { return (void)0; }
__attribute__((weak)) bool SDL_IsPhone(void) { return false; }
__attribute__((weak)) SDL_Surface* SDL_LoadJPG_IO(SDL_IOStream *a,bool b) { return nullptr; }
__attribute__((weak)) SDL_Surface* SDL_LoadJPG(const char *a) { return nullptr; }
__attribute__((weak)) bool SDL_HasSVE2(void) { return false; }
__attribute__((weak)) bool SDL_GamepadHasCapSense(SDL_Gamepad *a,SDL_GamepadCapSenseType b) { return false; }
__attribute__((weak)) bool SDL_GetGamepadCapSense(SDL_Gamepad *a,SDL_GamepadCapSenseType b) { return false; }
__attribute__((weak)) void* SDL_aligned_alloc_zero(size_t a,size_t b) { return nullptr; }
__attribute__((weak)) unsigned long SDL_wcstoul(const wchar_t *a,wchar_t **b,int c) { return (unsigned long)0; }
__attribute__((weak)) long long SDL_wcstoll(const wchar_t *a,wchar_t **b,int c) { return (long long)0; }
__attribute__((weak)) unsigned long long SDL_wcstoull(const wchar_t *a,wchar_t **b,int c) { return (unsigned long long)0; }
__attribute__((weak)) bool SDL_RequestNotificationPermission(void) { return false; }
__attribute__((weak)) SDL_NotificationID SDL_ShowNotificationWithProperties(SDL_PropertiesID a) { return (SDL_NotificationID)0; }
__attribute__((weak)) SDL_NotificationID SDL_ShowNotification(const char *a,const char *b,SDL_Surface *c,SDL_NotificationAction *d,int e) { return (SDL_NotificationID)0; }
__attribute__((weak)) bool SDL_RemoveNotification(SDL_NotificationID a) { return false; }
__attribute__((weak)) SDL_FormFactor SDL_GetDeviceFormFactor(void) { return (SDL_FormFactor)0; }
__attribute__((weak)) const char* SDL_GetDeviceFormFactorName(SDL_FormFactor a) { return nullptr; }
__attribute__((weak)) bool SDL_IsUbuntuTouch(void) { return false; }
__attribute__((weak)) int SDL_GetNumProperties(SDL_PropertiesID a) { return 0; }

} /* namespace sdl3 */
} /* namespace libtas */

#endif /* __APPLE__ */