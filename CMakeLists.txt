cmake_minimum_required(VERSION 2.6.0)
set(PROJ_NAME tetris)
project(tetris)
set(CMAKE_MODULE_PATH
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

find_package(GLFW REQUIRED)
find_package(OpenGL REQUIRED)

if (APPLE)
	find_package(GLEW)
else()
	find_package(GLEW REQUIRED)
endif()

include(ExternalProject)
externalproject_add(glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	INSTALL_COMMAND ""
	BUILD_COMMAND ""
	CONFIGURE_COMMAND ""
	)
ExternalProject_Get_Property(glm source_dir)
message(${source_dir})
set(GLM_INCLUDE_DIR ${source_dir})

IF(UNIX)
	SET(CMAKE_CXX_FLAGS
		"-std=c++11"
		#"-Werror"
		#"-Wall"
		#"-Werror=sign-compare"
		#"-Wno-error=unused-variable"
		#"-Wno-error=unused-but-set-variable"
		)
ENDIF(UNIX)

list(APPEND PLATFORM_LIBRARIES
	"${GLFW_LIBRARIES}"
	"${OPENGL_LIBRARY}"
)

include_directories(
	"${GLFW_INCLUDE_DIR}"
	"${GLM_INCLUDE_DIR}"
)

if ( GLEW_FOUND )
	include_directories("${GLEW_INCLUDE_DIR}")
	list(APPEND PLATFORM_LIBRARIES "${GLEW_LIBRARY}")
endif()

set(SOURCES
	"src/main.cpp"
	)

set(HEADERS
	"src/grid.h"
	"src/rendering.h"
	)

include_directories(
	${tetris_INCLUDE_DIRS}
	)

add_executable(tetris ${HEADERS} ${SOURCES} )

add_dependencies(${PROJ_NAME} glm)

target_link_libraries(tetris ${PLATFORM_LIBRARIES})
