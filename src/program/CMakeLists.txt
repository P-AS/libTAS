#### Dependencies ####

# Qt6 is preferred, Qt5 is still supported as a fallback
if(LIBTAS_QT_VERSION)
    set(QT_VERSION_MAJOR ${LIBTAS_QT_VERSION})
else()
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
endif()
if(QT_VERSION_MAJOR EQUAL 5)
    find_package(Qt5 5.6.0 REQUIRED COMPONENTS Core Widgets)
elseif(QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
else()
    message(FATAL_ERROR "Unsupported Qt version ${QT_VERSION_MAJOR}, only 5 and 6 are supported")
endif()
message(STATUS "Using Qt ${Qt${QT_VERSION_MAJOR}_VERSION}")

# Used by build.sh to bundle the right Qt version in the AppImage
set(LIBTAS_QT_VERSION_USED ${QT_VERSION_MAJOR} CACHE INTERNAL "")

pkg_check_modules(XCB REQUIRED IMPORTED_TARGET x11 xcb xcb-xkb xcb-keysyms)
pkg_check_modules(LIBCAP REQUIRED IMPORTED_TARGET libcap)
pkg_search_module(LIBLUA REQUIRED IMPORTED_TARGET lua54 lua5.4 lua)

if(NOT LIBTAS_HAS_XCB_RANDR)
    message(FATAL_ERROR "The xcb randr header is required!")
endif()

find_program(FFMPEG ffmpeg)
if(NOT FFMPEG)
    message(WARNING "ffmpeg was not found, it is required at runtime for encoding")
endif()

#### Sources ####

# Headers that need to be processed by moc
set(LIBTAS_PROGRAM_QT_HEADERS
    GameEvents.h
    GameLoop.h
    movie/MovieFileChangeLog.h
    movie/MovieFileInputs.h
    lua/Print.h
    ramsearch/IOProcessDevice.h
    ramsearch/MemScanner.h
    ui/AnalogInputsModel.h
    ui/AnalogInputsWindow.h
    ui/AnnotationsWindow.h
    ui/ComboBoxItemDelegate.h
    ui/ControllerAxisWidget.h
    ui/ControllerTabWindow.h
    ui/ControllerWidget.h
    ui/EncodeWindow.h
    ui/ExecutableWindow.h
    ui/GameInfoWindow.h
    ui/HexDelegate.h
    ui/HexViewWindow.h
    ui/InputChangeLogModel.h
    ui/InputChangeLogWindow.h
    ui/InputEditorModel.h
    ui/InputEditorView.h
    ui/InputEditorWindow.h
    ui/InputEventModel.h
    ui/InputEventWindow.h
    ui/InputWindow.h
    ui/KeyPressedDialog.h
    ui/LuaConsoleWindow.h
    ui/LuaConsoleModel.h
    ui/MainWindow.h
    ui/MarkerModel.h
    ui/MarkerView.h
    ui/MovieSettingsWindow.h
    ui/PointerScanModel.h
    ui/PointerScanWindow.h
    ui/RamSearchModel.h
    ui/RamSearchWindow.h
    ui/RamWatchEditWindow.h
    ui/RamWatchModel.h
    ui/RamWatchView.h
    ui/RamWatchWindow.h
    ui/TimeTraceModel.h
    ui/TimeTraceWindow.h
    ui/settings/RuntimePane.h
    ui/settings/AudioPane.h
    ui/settings/InputPane.h
    ui/settings/MoviePane.h
    ui/settings/VideoPane.h
    ui/settings/DebugPane.h
    ui/settings/GameSpecificPane.h
    ui/settings/PathPane.h
    ui/settings/SettingsWindow.h
    ui/settings/tooltip/BalloonTip.h
    ../external/qhexview/include/QHexView/model/buffer/qdevicebuffer.h
    ../external/qhexview/include/QHexView/model/buffer/qhexbuffer.h
    ../external/qhexview/include/QHexView/model/buffer/qmappedfilebuffer.h
    ../external/qhexview/include/QHexView/model/buffer/qmemorybuffer.h
    ../external/qhexview/include/QHexView/model/buffer/qmemoryrefbuffer.h
    ../external/qhexview/include/QHexView/model/commands/hexcommand.h
    ../external/qhexview/include/QHexView/model/commands/insertcommand.h
    ../external/qhexview/include/QHexView/model/commands/removecommand.h
    ../external/qhexview/include/QHexView/model/commands/replacecommand.h
    ../external/qhexview/include/QHexView/model/qhexcursor.h
    ../external/qhexview/include/QHexView/model/qhexdelegate.h
    ../external/qhexview/include/QHexView/model/qhexdocument.h
    ../external/qhexview/include/QHexView/model/qhexmetadata.h
    ../external/qhexview/include/QHexView/model/qhexoptions.h
    ../external/qhexview/include/QHexView/model/qhexutils.h
    ../external/qhexview/include/QHexView/qhexview.h
)

set(LIBTAS_PROGRAM_SOURCES
    AutoDetect.cpp
    AutoSave.cpp
    Config.cpp
    GameEvents.cpp
    GameEventsXcb.cpp
    GameLoop.cpp
    GameThread.cpp
    KeyMapping.cpp
    KeyMappingXcb.cpp
    main.cpp
    SaveState.cpp
    SaveStateList.cpp
    Signature.cpp
    UnityPatching.cpp
    utils.cpp
    lua/Callbacks.cpp
    lua/Gui.cpp
    lua/LuaFunctionList.cpp
    lua/NamedLuaFunction.cpp
    lua/Input.cpp
    lua/Main.cpp
    lua/Memory.cpp
    lua/Movie.cpp
    lua/Print.cpp
    lua/Runtime.cpp
    movie/InputSerialization.cpp
    movie/MovieActionEditFrames.cpp
    movie/MovieActionInsertFrames.cpp
    movie/MovieActionPaint.cpp
    movie/MovieActionRemoveFrames.cpp
    movie/MovieFile.cpp
    movie/MovieFileAnnotations.cpp
    movie/MovieFileChangeLog.cpp
    movie/MovieFileEditor.cpp
    movie/MovieFileHeader.cpp
    movie/MovieFileInputs.cpp
    ui/AnalogInputsModel.cpp
    ui/AnalogInputsWindow.cpp
    ui/AnnotationsWindow.cpp
    ui/ComboBoxItemDelegate.cpp
    ui/ControllerAxisWidget.cpp
    ui/ControllerTabWindow.cpp
    ui/ControllerWidget.cpp
    ui/EncodeWindow.cpp
    ui/ErrorChecking.cpp
    ui/ExecutableWindow.cpp
    ui/GameInfoWindow.cpp
    ui/HexDelegate.cpp
    ui/HexViewWindow.cpp
    ui/InputChangeLogModel.cpp
    ui/InputChangeLogWindow.cpp
    ui/InputEditorModel.cpp
    ui/InputEditorView.cpp
    ui/InputEditorWindow.cpp
    ui/InputEventModel.cpp
    ui/InputEventWindow.cpp
    ui/InputWindow.cpp
    ui/KeyPressedDialog.cpp
    ui/LuaConsoleWindow.cpp
    ui/LuaConsoleModel.cpp
    ui/MainWindow.cpp
    ui/WindowManager.cpp
    ui/MarkerModel.cpp
    ui/MarkerView.cpp
    ui/MovieSettingsWindow.cpp
    ui/PointerScanModel.cpp
    ui/PointerScanWindow.cpp
    ui/RamSearchModel.cpp
    ui/RamSearchWindow.cpp
    ui/RamWatchEditWindow.cpp
    ui/RamWatchModel.cpp
    ui/RamWatchView.cpp
    ui/RamWatchWindow.cpp
    ui/TimeTraceModel.cpp
    ui/TimeTraceWindow.cpp
    ui/qtutils.cpp
    ui/settings/RuntimePane.cpp
    ui/settings/AudioPane.cpp
    ui/settings/InputPane.cpp
    ui/settings/MoviePane.cpp
    ui/settings/VideoPane.cpp
    ui/settings/DebugPane.cpp
    ui/settings/GameSpecificPane.cpp
    ui/settings/PathPane.cpp
    ui/settings/SettingsWindow.cpp
    ui/settings/WrapInScrollArea.cpp
    ui/settings/tooltip/BalloonTip.cpp
    ui/settings/tooltip/ToolTipCheckBox.cpp
    ui/settings/tooltip/ToolTipComboBox.cpp
    ui/settings/tooltip/ToolTipGroupBox.cpp
    ui/settings/tooltip/ToolTipRadioButton.cpp
    ui/settings/tooltip/ToolTipSlider.cpp
    ui/settings/tooltip/ToolTipSpinBox.cpp
    ramsearch/BaseAddresses.cpp
    ramsearch/CompareOperations.cpp
    ramsearch/IOProcessDevice.cpp
    ramsearch/RamWatchDetailed.cpp
    ramsearch/MemAccess.cpp
    ramsearch/MemLayout.cpp
    ramsearch/MemScanner.cpp
    ramsearch/MemScannerThread.cpp
    ramsearch/MemSection.cpp
    ramsearch/MemValue.cpp
    ../shared/inputs/AllInputs.cpp
    ../shared/inputs/ControllerInputs.cpp
    ../shared/inputs/MiscInputs.cpp
    ../shared/inputs/MouseInputs.cpp
    ../shared/inputs/SingleInput.cpp
    ../shared/sockethelpers.cpp
    ../external/qhexview/src/model/commands/hexcommand.cpp
    ../external/qhexview/src/model/commands/insertcommand.cpp
    ../external/qhexview/src/model/commands/removecommand.cpp
    ../external/qhexview/src/model/commands/replacecommand.cpp
    ../external/qhexview/src/model/buffer/qdevicebuffer.cpp
    ../external/qhexview/src/model/buffer/qhexbuffer.cpp
    ../external/qhexview/src/model/buffer/qmemorybuffer.cpp
    ../external/qhexview/src/model/buffer/qmemoryrefbuffer.cpp
    ../external/qhexview/src/model/buffer/qmappedfilebuffer.cpp
    ../external/qhexview/src/model/qhexdelegate.cpp
    ../external/qhexview/src/model/qhexutils.cpp
    ../external/qhexview/src/model/qhexcursor.cpp
    ../external/qhexview/src/model/qhexmetadata.cpp
    ../external/qhexview/src/model/qhexdocument.cpp
    ../external/qhexview/src/qhexview.cpp
)

#### Target ####

add_executable(libTAS ${LIBTAS_PROGRAM_SOURCES} ${LIBTAS_PROGRAM_QT_HEADERS})

# Qt requires the program to be position independent, otherwise signals are
# not found when Qt is linked with -Bsymbolic-functions (as in Ubuntu)
include(CheckPIESupported)
check_pie_supported()
set_target_properties(libTAS PROPERTIES AUTOMOC ON POSITION_INDEPENDENT_CODE ON)

target_compile_options(libTAS PRIVATE -fno-stack-protector -Wno-float-equal)

target_include_directories(libTAS PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/src/external/qhexview/include)

target_link_libraries(libTAS PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    PkgConfig::LIBLUA
    PkgConfig::XCB
    PkgConfig::LIBCAP
    Threads::Threads)

install(TARGETS libTAS RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Allow libTAS to restore checkpoints without root. This needs privileges, so
# only warn if it fails (e.g. when installing into a staging directory).
install(CODE "set(_libtas_bindir \"${CMAKE_INSTALL_BINDIR}\")")
install(CODE [[
    find_program(SETCAP setcap)
    if(SETCAP)
        set(_libtas_bin "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/${_libtas_bindir}/libTAS")
        execute_process(COMMAND ${SETCAP} cap_checkpoint_restore+eip ${_libtas_bin} RESULT_VARIABLE _res)
        if(NOT _res EQUAL 0)
            message(WARNING "Could not set cap_checkpoint_restore capability on ${_libtas_bin}")
        endif()
    endif()
]])
