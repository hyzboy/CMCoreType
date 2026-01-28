cmake_minimum_required(VERSION 3.20)
project(CMCoreType)

include(path_config.cmake)
CMCoreTypeSetup(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${CMCORETYPE_ROOT_SOURCE_PATH})

add_subdirectory(examples)
