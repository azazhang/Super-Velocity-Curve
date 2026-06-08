cmake_minimum_required(VERSION 3.22)

project(SuperVelocityCurve VERSION 0.6.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Post-build install to ~/Library/Audio/Plug-Ins is for local dev only — slows/hangs CI.
if(DEFINED ENV{CI})
    set(SVC_COPY_PLUGIN_AFTER_BUILD FALSE)
else()
    set(SVC_COPY_PLUGIN_AFTER_BUILD TRUE)
endif()

# LTO makes Apple Clang CI links take 60–90+ minutes for dual JUCE plugins. Keep for local Release.
option(SVC_ENABLE_LTO "Link-time optimization (disable in CI)" ON)
if(DEFINED ENV{CI})
    set(SVC_ENABLE_LTO OFF CACHE BOOL "LTO off in CI" FORCE)
endif()

add_subdirectory(JUCE)

juce_add_binary_data(SuperVelocityCurveBinaryData SOURCES Resources/pluginval_badge.png)

set(SVC_PLUGIN_SOURCES
    Source/Engine/VelocityCurve.cpp
    Source/Engine/VelocityEngine.cpp
    Source/Engine/MidiUtilities.cpp
    Source/Profiles/ControllerProfile.cpp
    Source/Profiles/ProfileStore.cpp
    Source/Plugin/PluginProcessor.cpp
    Source/Plugin/PluginEditor.cpp
    Source/Standalone/StandaloneMidiPanel.cpp
    Source/UI/PadGridComponent.cpp
    Source/UI/CurveEditorComponent.cpp
    Source/UI/PadInspectorComponent.cpp
    Source/UI/HistogramComponent.cpp
    Source/UI/CalibrationWizardComponent.cpp
    Source/UI/MidiRoutingPanel.cpp
    Source/UI/NoteRemapEditorComponent.cpp
    Source/UI/MidiActivityMeterComponent.cpp
    Source/UI/AboutPanelComponent.cpp)

function(svc_configure_plugin_target target)
    juce_generate_juce_header(${target})

    target_sources(${target} PRIVATE ${SVC_PLUGIN_SOURCES})

    target_compile_definitions(${target}
        PRIVATE
            SVC_VERSION="${PROJECT_VERSION}")

    target_compile_definitions(${target}
        PUBLIC
            JUCE_WEB_BROWSER=0
            JUCE_USE_CURL=0
            JUCE_VST3_CAN_REPLACE_VST2=0
            JUCE_DISPLAY_SPLASH_SCREEN=0)

    target_include_directories(${target} PRIVATE
        ${CMAKE_BINARY_DIR}/juce_binarydata_SuperVelocityCurveBinaryData/JuceLibraryCode)

    target_link_libraries(${target}
        PRIVATE
            juce::juce_audio_utils
            SuperVelocityCurveBinaryData
        PUBLIC
            juce::juce_recommended_config_flags
            juce::juce_recommended_warning_flags)

    if(SVC_ENABLE_LTO)
        target_link_libraries(${target} PUBLIC juce::juce_recommended_lto_flags)
    endif()
endfunction()

# Instrument VST3 + AU Music Effect (aumf) + Standalone — required for Ableton Live.
juce_add_plugin(SuperVelocityCurve
    COMPANY_NAME "SuperVelocityCurve"
    PLUGIN_MANUFACTURER_CODE Svvc
    PLUGIN_CODE Svcv
    BUNDLE_ID com.supervelocitycurve.instrument
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "Super VelocityCurve"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT FALSE
    VST3_CATEGORIES Instrument
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD ${SVC_COPY_PLUGIN_AFTER_BUILD})

svc_configure_plugin_target(SuperVelocityCurve)

if (APPLE AND TARGET SuperVelocityCurve_Standalone)
    add_custom_command (TARGET SuperVelocityCurve_Standalone POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E touch "$<TARGET_BUNDLE_DIR:SuperVelocityCurve_Standalone>"
        COMMENT "Refresh Standalone .app bundle timestamp")
endif()

# MIDI FX VST3 + AU MIDI Processor (aumi) — Logic MIDI FX slot, Reaper, Bitwig.
juce_add_plugin(SuperVelocityCurveMidiFx
    COMPANY_NAME "SuperVelocityCurve"
    PLUGIN_MANUFACTURER_CODE Svvc
    PLUGIN_CODE Svc2
    BUNDLE_ID com.supervelocitycurve.midifx
    FORMATS AU VST3
    PRODUCT_NAME "Super VelocityCurve MIDI FX"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT TRUE
    VST3_CATEGORIES Fx
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD ${SVC_COPY_PLUGIN_AFTER_BUILD})

svc_configure_plugin_target(SuperVelocityCurveMidiFx)

option(SVC_BUILD_CLAP "Build CLAP format via clap-juce-extensions" ON)

if(SVC_BUILD_CLAP AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/clap-juce-extensions/CMakeLists.txt")
    add_subdirectory(third_party/clap-juce-extensions EXCLUDE_FROM_ALL)

    clap_juce_extensions_plugin(TARGET SuperVelocityCurveMidiFx
        CLAP_ID "com.supervelocitycurve.midifx"
        CLAP_FEATURES note-effect)
endif()

option(SVC_BUILD_TESTS "Build engine unit tests" ON)

if(SVC_BUILD_TESTS)
    enable_testing()

    juce_add_console_app(SuperVelocityCurveTests
        PRODUCT_NAME "SuperVelocityCurveTests")

    juce_generate_juce_header(SuperVelocityCurveTests)

    target_sources(SuperVelocityCurveTests
        PRIVATE
            Tests/EngineTests.cpp
            Source/Engine/VelocityCurve.cpp
            Source/Engine/VelocityEngine.cpp
            Source/Engine/MidiUtilities.cpp
            Source/Profiles/ControllerProfile.cpp
            Source/Profiles/ProfileStore.cpp)

    target_include_directories(SuperVelocityCurveTests PRIVATE Source)

    target_link_libraries(SuperVelocityCurveTests PRIVATE
        juce::juce_core
        juce::juce_audio_basics
        juce::juce_data_structures
        juce::juce_recommended_warning_flags)

    add_test(NAME EngineTests COMMAND SuperVelocityCurveTests)
endif()

option(SVC_EXPORT_FACTORY_PROFILES "Build factory profile export tool" ON)

if(SVC_EXPORT_FACTORY_PROFILES)
    juce_add_console_app(ExportFactoryProfiles
        PRODUCT_NAME "ExportFactoryProfiles")

    juce_generate_juce_header(ExportFactoryProfiles)

    target_sources(ExportFactoryProfiles
        PRIVATE
            Tools/ExportFactoryProfiles.cpp
            Source/Profiles/ControllerProfile.cpp
            Source/Engine/VelocityCurve.cpp
            Source/Engine/VelocityEngine.cpp
            Source/Engine/MidiUtilities.cpp)

    target_include_directories(ExportFactoryProfiles PRIVATE Source)

    target_link_libraries(ExportFactoryProfiles PRIVATE
        juce::juce_core
        juce::juce_audio_basics
        juce::juce_data_structures
        juce::juce_recommended_warning_flags)

    add_custom_target(export-factory-profiles
        COMMAND ExportFactoryProfiles
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ExportFactoryProfiles
        COMMENT "Export factory .svcp profile files")
endif()
