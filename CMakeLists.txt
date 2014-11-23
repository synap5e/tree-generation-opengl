cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME tree)
project(${PROJECT_NAME})

#### GLFW ####
set(GLFW_DIR "${PROJECT_SOURCE_DIR}/external/glfw")

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Don't build glfw examples")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Don't build glfw tests")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Don't build glfw docs")
set(GLFW_INSTALL OFF CACHE BOOL "Don't install glfw")
add_subdirectory(${GLFW_DIR} glfw)
include_directories(${GLFW_DIR}/include)
#set(Link_libs glfw ${GLFW_LIBRARIES})


#### GLEW ####
set(GLEW_DIR "${PROJECT_SOURCE_DIR}/external/glew")
add_subdirectory(${GLEW_DIR} glew)
include_directories(${GLEW_DIR}/include)


#### muparser ####
set(MUPARSER_DIR "${PROJECT_SOURCE_DIR}/external/muparser")
add_subdirectory(${MUPARSER_DIR} muparser)
include_directories(${MUPARSER_DIR}/include)


#### GLM ####
set(GLM_DIR "${PROJECT_SOURCE_DIR}/external/glm")
include_directories(${GLM_DIR})


#### PicoJson ####
set(PICOJSON_DIR "${PROJECT_SOURCE_DIR}/external/picojson")
include_directories(${PICOJSON_DIR})


#### SOIL ####
set(SOIL_DIR "${PROJECT_SOURCE_DIR}/external/Simple-OpenGL-Image-Library")
add_subdirectory(${SOIL_DIR} soil)
include_directories(${SOIL_DIR}/src)


#### Application ####

add_definitions("-D DEBUG")
set(CMAKE_CXX_FLAGS "-g -std=c++11 ${CMAKE_CXX_FLAGS}")

FILE(GLOB Source_files
	*.cpp
)

FILE(GLOB Header_files
    *.h,
    *.hpp
)

add_executable(${PROJECT_NAME} ${Source_files} ${Header_files})

target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${MUPARSER_LIBRARIES} ${LIBJPEG_LIBRARIES} glfw glew muparser soil)

