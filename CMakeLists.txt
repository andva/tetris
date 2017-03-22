cmake_minimum_required(VERSION 2.6.0)
set(PROJ_NAME tetris)
project(tetris)

find_package(GLFW 3.0.0)
find_package(OpenGL)

if (APPLE)
    find_package(GLEW)
else()
    find_package(GLEW REQUIRED)
endif()

IF(UNIX)
	SET(CMAKE_CXX_FLAGS "-std=c++11 -Werror -Wall -Werror=sign-compare -Wno-error=unused-variable -Wno-error=unused-but-set-variable")
ENDIF(UNIX)

list(APPEND PLATFORM_LIBRARIES
    "${GLFW_LIBRARIES}"
)

include_directories(
    "${GLFW_INCLUDE_DIR}"
)

if ( GLEW_FOUND )
    include_directories("${GLEW_INCLUDE_DIR}")
    list(APPEND PLATFORM_LIBRARIES "${GLEW_LIBRARY}")
endif()

FILE(GLOB HEADERS  ./*.h*)

FILE(GLOB SRCS  ./*.cpp)

include_directories(
	${tetris_INCLUDE_DIRS}
	)

link_directories(
    ${EXTRA_LIB_FILES}
	${CMAKE_SOURCE_DIR}/externals/libs
	${EXTRA_LIB_FILES}
	)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/export)

add_executable(tetris ${HEADERS} ${SRCS} )

if(WIN32)
	target_link_libraries(tetris GLFW.lib FreeImage.lib freeglut.lib)
endif(WIN32)

if(UNIX)
	target_link_libraries(tetris freeimage ${EXTRA_LIBS})
endif(UNIX)
