cmake_minimum_required(VERSION 3.6)

set(APP_NAME scoreWarriorTest)
set(XCODE_STARTUP_PROJECT ${APP_NAME})

project(${APP_NAME})

set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)
set(INCLUDE_SOURCE)
set(INCLUDE_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
)

cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})

file(GLOB_RECURSE GAME_SOURCE src/*.cpp)
file(GLOB_RECURSE GAME_HEADER src/*.h)
file(GLOB_RECURSE INCLUDE_SOURCE include/*.mm)
file(GLOB_RECURSE INCLUDE_HEADER include/*.h)

set(APP_UI_RES
    Icon.icns
    Info.plist
)

list(APPEND GAME_SOURCE
    main.cpp
    Prefix.pch
    appDelegate.cpp
    appDelegate.h
    ${APP_UI_RES}
)

list(APPEND GAME_SOURCE ${common_res_files})

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    ${INCLUDE_SOURCE}
    ${INCLUDE_HEADER}
)

add_executable(${APP_NAME} ${all_code_files})

find_library(CORE_MEDIA_LIBRARY CoreMedia)
find_library(AV_LIBRARY AVFoundation)

target_link_libraries(${APP_NAME} ${CORE_MEDIA_LIBRARY} ${AV_LIBRARY} cocos2d)
target_include_directories(${APP_NAME}
        PRIVATE src
        PRIVATE include
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

# mark app resources
setup_cocos_app_config(${APP_NAME})
set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")

set_target_properties(${APP_NAME} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
)