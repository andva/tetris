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
message(${GLEW_LIBRARY})
list(APPEND PLATFORM_LIBRARIES
	"${GLFW_LIBRARIES}"
	"${OPENGL_LIBRARY}"
)

include_directories(
	"${GLFW_INCLUDE_DIR}"
	"${GLM_INCLUDE_DIR}"
)
if(GLFW_FOUND AND (GLFW_VERSION VERSION_EQUAL 3.0 OR GLFW_VERSION VERSION_GREATER 3.0))
	add_definitions( -DGLFW_VERSION_3 )
endif()
if (WIN32)
	list(APPEND PLATFORM_LIBRARIES
		legacy_stdio_definitions.lib
	)
endif()
if ( GLEW_FOUND )
	if (WIN32)
		if ("${GLEW_LIBRARY}" MATCHES "glew32s(d|)")
			# Link against the static version of GLEW
			add_definitions(
				-DGLEW_STATIC
			)
		endif()
	endif()
	include_directories("${GLEW_INCLUDE_DIR}")
	list(APPEND PLATFORM_LIBRARIES "${GLEW_LIBRARY}")
endif()

set(SOURCES
	"src/main.cpp"
	"src/grid.cpp"
	"src/piecefactory.cpp"
	"src/piecemanager.cpp"
	"src/ai.cpp"
	"src/piece.cpp"
	"src/human.cpp"
	"src/game.cpp"
	"src/neural/neural.cpp"
	"src/neural/genome.cpp"
	"src/neural/gene.cpp"
	)

set(HEADERS
	"src/grid.h"
	"src/rendering.h"
	"src/piecefactory.h"
	"src/piece.h"
	"src/direction.h"
	"src/piecemanager.h"
	"src/ai.h"
	"src/game.h"
	"src/tetromino.h"
	"src/human.h"
	"src/neural/neural.h"
	"src/neural/gene.h"
	"src/neural/genome.h"
	"src/neural/neuron.h"
	"src/neural/pool.h"
	"src/neural/species.h"
	"src/neural/outputs.h"
	)

include_directories(
	${tetris_INCLUDE_DIRS}
	)

add_executable(tetris ${HEADERS} ${SOURCES} )

add_dependencies(${PROJ_NAME} glm)

target_link_libraries(tetris ${PLATFORM_LIBRARIES})

macro(group_files target_name root_folder replacepath)
	foreach(FILE ${ARGN})
		# Get file name part of file
		get_filename_component(PARENT_DIR "${FILE}" PATH)
		if (NOT IS_ABSOLUTE ${PARENT_DIR})
			set(PARENT_DIR "${CMAKE_CURRENT_LIST_DIR}/${PARENT_DIR}")
		endif ()
		file(RELATIVE_PATH relfile ${replacepath} ${PARENT_DIR})

		string(REPLACE "/" "\\" relfile "${relfile}")

		source_group("${relfile}" FILES "${FILE}")
		set_target_properties("${target_name}" PROPERTIES FOLDER "${root_folder}")
	endforeach()
endmacro()

group_files(tetris "" ${CMAKE_CURRENT_LIST_DIR} ${HEADERS} ${SOURCES})
