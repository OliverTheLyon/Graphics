cmake_minimum_required(VERSION 3.10)
project(Graphics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE DEBUG)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# stb_image is header-only, just need to include its directory
# Download stb_image.h if not present
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/stb_image.h")
    message(STATUS "Downloading stb_image.h...")
    file(DOWNLOAD "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h"
         "${CMAKE_SOURCE_DIR}/stb_image.h")
endif()

# Copy resources to build directory so relative shader paths work
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

# Core engine sources shared by all executables
set(CORE_SOURCES
    screen.hpp screen.cpp
    scene.hpp scene.cpp
    camera.hpp camera.cpp
    texture.hpp texture.cpp
    shader.hpp shader.cpp
    mesh.hpp mesh.cpp
    logger.hpp logger.cpp
    lighting/light.hpp lighting/light.cpp
    lighting/normalmap.hpp lighting/normalmap.cpp
    Utility/entity.hpp Utility/entity.cpp
    Utility/movable_entity.hpp Utility/movable_entity.cpp
    Utility/errors.hpp Utility/errors.cpp
    Utility/transform.hpp
)

add_executable(Graphics main.cpp ${CORE_SOURCES})
add_executable(lightTest lighting/lighttest.cpp ${CORE_SOURCES})

foreach(target Graphics lightTest)
    target_include_directories(${target} PRIVATE
        ${OPENGL_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}
    )

    target_link_libraries(${target}
        ${OPENGL_LIBRARIES}
        GLEW::glew
        glfw
    )
endforeach()
