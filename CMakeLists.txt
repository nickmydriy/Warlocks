cmake_minimum_required(VERSION 3.3)
project(Warlocks)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenGL REQUIRED)

add_subdirectory(lib/glfw-3.1.2/)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        lib/glm/
        lib/glew/include/
        lib/glfw/include/)

add_library(glew STATIC IMPORTED)
set_property(TARGET glew PROPERTY IMPORTED_LOCATION
       ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/lib/libglew32dll.a)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glew
        glfw)

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
        Bot.cpp Settings.cpp Settings.h)

add_executable(Warlocks ${SOURCE_FILES})
target_link_libraries(Warlocks ${ALL_LIBS})