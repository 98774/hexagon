cmake_minimum_required( VERSION 3.5...3.31 )

project( hexagon )

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
set( GLAD_GL "" )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "Create compile_commands.json") # works

add_subdirectory( glfw )
include_directories( include )

option( HEXAGON-USE-GLFW-GLAD "Use GLAD from GLFW" ON )


if( HEXAGON-USE-GLFW-GLAD )
    include_directories("${GLFW_SOURCE_DIR}/deps")
    set( GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h" )
endif()

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

set( HEXAGON-SRC
        src/glad.c
        src/main.cpp
        src/hexagon.cpp
        src/walls.cpp
        )

add_executable( hexagon WIN32 ${HEXAGON-SRC} ${GLAD_GL} )
target_link_libraries( hexagon ${OPENGL_LIBRARIES} glfw )
if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GLFW-CMake-starter' as StartUp Project in Visual Studio.\n" )

    else()
        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT hexagon)
    endif()
endif()
