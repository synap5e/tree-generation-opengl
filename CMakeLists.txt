cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME tree)
project(${PROJECT_NAME})

#### GLFW ####
set(GLFW_DIR external/glfw)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Don't build glfw examples")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Don't build glfw tests")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Don't build glfw docs")
set(GLFW_INSTALL OFF CACHE BOOL "Don't install glfw")
add_subdirectory(${GLFW_DIR} glfw)
include_directories(${GLFW_DIR}/include)
set(Link_libs glfw ${GLFW_LIBRARIES})


#### GLEW ####
#set(GLEW_DIR external/glew)

#add_library(glew_static STATIC IMPORTED)
#add_subdirectory($(GLEW_DIR) glew)
#include_directories(${GLEW_DIR}/include)
#set(Link_libs glew ${GLEW_LIBRARIES})

#add_library(glew_static STATIC IMPORTED)
#set_target_properties(glew_static PROPERTIES IMPORTED_LOCATION /usr/lib/libGLEW.a)
#target_link_libraries(${PROJECT_NAME} glew_static)

find_package(GLEW REQUIRED)
include_directories(${GLEW_DIR}/include)


#### GLM ####
set(GLM_DIR external/glm)

include_directories(${GLM_DIR})

#### Application ####

add_definitions("-D DEBUG")
set(CMAKE_CXX_FLAGS "-g -std=c++11 ${CMAKE_CXX_FLAGS}")

FILE(GLOB Source_files
	*.cpp
)

FILE(GLOB Header_files
    *.h
)

add_executable(${PROJECT_NAME} ${Source_files} ${Header_files})

target_link_libraries(${PROJECT_NAME} ${Link_libs} ${GLEW_LIBRARY})

