cmake_minimum_required(VERSION 3.14)

project (RES-Network)

set (TARGET_NAME )

set(CMAKE_UNITY_BUILD ON)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

add_subdirectory(Device)
add_subdirectory(GTC-3)
add_subdirectory(MS-1)
add_subdirectory(TwoSensors)
add_subdirectory(5-Governor)
