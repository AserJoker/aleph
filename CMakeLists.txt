cmake_minimum_required(VERSION 3.12)
project(darkland)

set(CMAKE_CXX_STANDARD 20)

add_library(wtswidth ${PROJECT_SOURCE_DIR}/deps/wtswidth/wtswidth.c)

include_directories(${PROJECT_SOURCE_DIR}/deps/wtswidth)

file(GLOB PACKAGES_DIRS ${PROJECT_SOURCE_DIR}/packages/*)
include_directories(${PROJECT_SOURCE_DIR}/packages)
file(GLOB_RECURSE PACKAGES ${PROJECT_SOURCE_DIR}/packages/*.cc)
add_library(aleph ${PACKAGES})

file(GLOB PROJECTS ${PROJECT_SOURCE_DIR}/projects/*)

foreach(PROJECT ${PROJECTS})
    get_filename_component(CURRENT_PROJECT_NAME ${PROJECT} NAME)
    file(GLOB_RECURSE ${CURRENT_PROJECT_NAME}_SOURCES ${PROJECT}/src/*.cc)
    add_executable(${CURRENT_PROJECT_NAME} ${${CURRENT_PROJECT_NAME}_SOURCES})
    target_include_directories(${CURRENT_PROJECT_NAME} PUBLIC ${PROJECT}/include)
    target_link_libraries(${CURRENT_PROJECT_NAME} PRIVATE aleph wtswidth)
endforeach(PROJECT ${PROJECTS})
