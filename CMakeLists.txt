cmake_minimum_required(VERSION 3.2)
project(RenderEngine)

# Add source files
file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add header files
file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/src/*.hpp)

# Add .lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Add Shader files (TODO: Allow for multiple shaders to be found)
#file(GLOB_RECURSE VERT_FILES ${CMAKE_SOURCE_DIR}/src/shaders/*.vert)
#file(GLOB_RECURSE FRAG_FILES ${CMAKE_SOURCE_DIR}/src/shaders/*.frag)
#file(GLOB_RECURSE MODEL_FILES ${CMAKE_SOURCE_DIR}/res/models/*.obj)

#configure_file(${VERT_FILES} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
#configure_file(${FRAG_FILES} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

# Add texture files
file(GLOB_RECURSE IMG_FILES
        ${CMAKE_SOURCE_DIR}/res/img/*.png
        ${CMAKE_SOURCE_DIR}/res/img/*.jpg
        ${CMAKE_SOURCE_DIR}/res/img/*.jpeg
        ${CMAKE_SOURCE_DIR}/res/img/*.bmp)

configure_file(${IMG_FILES} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

# Configure assets header file
configure_file(src/helpers/RootDir.h.in src/helpers/RootDir.h)
include_directories(${CMAKE_BINARY_DIR}/src)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} src/Window.cpp src/Window.h)

# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#############
# Libraries #
#############

# OpenGL
set(OpenGL_GL_PREFERENCE LEGACY) # TODO: Possibly change this
find_package(OpenGL REQUIRED)

# GLM
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

#GLFW
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# ASSIMP
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

# GLAD
ADD_LIBRARY(GLAD "include/glad.c")

# Put all libraries into a variable
set(LIBS GL glfw GLAD assimp X11 Xxf86vm Xrandr pthread Xi dl)

# Define the include directory
include_directories("${CMAKE_SOURCE_DIR}/include")

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})