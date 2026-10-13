cmake_minimum_required(VERSION 4.3)
project(tpc LANGUAGES CXX OBJCXX)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)

add_executable(tpc MACOSX_BUNDLE main.cpp MacInput.h MacInput.mm)

set_target_properties(tpc PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "TPC"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.bryan.tpc"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
)

target_compile_definitions(tpc PRIVATE ASSET_PATH="${CMAKE_CURRENT_BINARY_DIR}/tpc.app/Contents/Resources/assets/")

file(GLOB ASSET_FILES "${CMAKE_SOURCE_DIR}/assets/*")

target_sources(tpc PRIVATE ${ASSET_FILES})

set_source_files_properties(${ASSET_FILES} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources/assets"
)

target_link_libraries(tpc raylib "-framework AppKit")