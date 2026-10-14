cmake_minimum_required(VERSION 4.3)
project(tpc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if(APPLE)
    enable_language(OBJCXX)
    find_package(raylib REQUIRED)

    add_executable(tpc MACOSX_BUNDLE
            main.cpp
            GlobalInput.h
            MacInput.mm
    )
else()
    include(FetchContent)

    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 6.0
    )

    FetchContent_MakeAvailable(raylib)

    add_executable(tpc
            main.cpp
            GlobalInput.h
            WinInput.cpp
    )

    target_link_options(tpc PRIVATE
            -static-libgcc
            -static-libstdc++
    )

    add_custom_command(TARGET tpc POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:tpc>/assets"
    )
endif()

target_link_libraries(tpc PRIVATE raylib)

if(WIN32)
    target_include_directories(tpc PRIVATE
            ${raylib_SOURCE_DIR}/src
    )
endif()

if(APPLE)
    set_target_properties(tpc PROPERTIES
            MACOSX_BUNDLE_BUNDLE_NAME "TPC"
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.bryan.tpc"
            MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    )

    file(GLOB ASSET_FILES "${CMAKE_SOURCE_DIR}/assets/*")
    target_sources(tpc PRIVATE ${ASSET_FILES})

    set_source_files_properties(${ASSET_FILES} PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources/assets"
    )

    target_link_libraries(tpc PRIVATE "-framework AppKit")
endif()