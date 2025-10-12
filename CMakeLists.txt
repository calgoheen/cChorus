cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OSX deployment target")

file(STRINGS "VERSION" CurrentVersion)
project("cChorus" VERSION ${CurrentVersion})

include(FetchContent)

FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.8)
FetchContent_MakeAvailable(JUCE)

FetchContent_Declare(chowdsp_utils
    GIT_REPOSITORY https://github.com/Chowdhury-DSP/chowdsp_utils
    GIT_TAG ffc70ba399f9afaeefb996eb14e55a1d487270b8)
FetchContent_MakeAvailable(chowdsp_utils)

FetchContent_Declare(foleys_gui_magic
    GIT_REPOSITORY https://github.com/ffAudio/foleys_gui_magic
    GIT_TAG 5ae8534407d40736a477c59d029f7c7b1041966f)
FetchContent_Populate(foleys_gui_magic)
add_subdirectory(${foleys_gui_magic_SOURCE_DIR}/modules)

FetchContent_Declare(clap-juce-extensions
    GIT_REPOSITORY https://github.com/free-audio/clap-juce-extensions.git
    GIT_TAG ea834dfd366ab2103b98e92eaacb581c69fc2e0b)
FetchContent_MakeAvailable(clap-juce-extensions)

add_subdirectory(cgo_modules)

juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME CalGoheen
    COMPANY_WEBSITE "https://calgoheen.com"
    COMPANY_EMAIL "calgoheen@gmail.com"
    BUNDLE_ID com.CalGoheen.${PROJECT_NAME}
    PLUGIN_MANUFACTURER_CODE Calg
    PLUGIN_CODE Chor
    IS_SYNTH FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    FORMATS AU VST3 Standalone)

clap_juce_extensions_plugin(TARGET ${PROJECT_NAME}
    CLAP_ID "com.CalGoheen.${PROJECT_NAME}"
    CLAP_FEATURES "audio effect" "mono" "stereo")

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h")

target_sources(${PROJECT_NAME} PRIVATE ${SourceFiles})

juce_add_binary_data(Resources SOURCES "{CMAKE_CURRENT_SOURCE_DIR}/Resources/layout.xml")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(ShowFoleysEditor "FOLEYS_SHOW_GUI_EDITOR_PALLETTE=0")
else()
    set(ShowFoleysEditor "FOLEYS_SHOW_GUI_EDITOR_PALLETTE=1")
endif()

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        ${ShowFoleysEditor}
        FOLEYS_SAVE_EDITED_GUI_IN_PLUGIN_STATE=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_WEB_BROWSER=0)

if(MSVC)
    target_compile_definitions(${PROJECT_NAME}
        PUBLIC
            WIN32_LEAN_AND_MEAN
            NOMINMAX)
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
        chowdsp::chowdsp_dsp_utils
        chowdsp::chowdsp_filters
        cgo::cgo_dsp
        cgo::cgo_plugin
        cgo::cgo_processors
        foleys::foleys_gui_magic
        Resources

    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

juce_generate_juce_header(${PROJECT_NAME})
