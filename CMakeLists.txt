cmake_minimum_required(VERSION 3.3)
project(Warlocks)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.1 REQUIRED)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${OPENGL_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR}
        lib/glm/)

# add_library(glew STATIC IMPORTED)

set(SOURCE_FILES
        Model.cpp
        Loaders.cpp
        TextView.cpp
        GameWindow.cpp
        Warlock.cpp
        Game.cpp
        WMath.cpp
        main.cpp
        Player.cpp
        Bot.cpp 
        Settings.cpp 
        Settings.h)

add_executable(Warlocks ${SOURCE_FILES})
target_link_libraries(Warlocks ${OPENGL_LIBRARY} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} glfw)