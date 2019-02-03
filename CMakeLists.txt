cmake_minimum_required (VERSION 3.0)
project (Servsim)
set (Servsim_VERSION_MAJOR 1)
set (Servsim_VERSION_MINOR 0)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../data)

# use relative path to src files to make __FILE__ macro more compact.
set (CMAKE_USE_RELATIVE_PATHS ON)

#set (CMAKE_C_COMPILER 		  "/usr/bin/clang")
#set (CMAKE_CXX_COMPILER             "/usr/bin/clang++")

set (CMAKE_C_FLAGS                "-Wall -m64 -std=c99 -fPIC -Wno-unused-function")
set (CMAKE_C_FLAGS_DEBUG          "-g -DDEBUG")
set (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
set (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")
set (CMAKE_DEBUG_POSTFIX          "d")

set (CMAKE_CXX_FLAGS                "-Wall -m64 -std=c++0x -fPIC -Wno-unused-function -Wno-unused-private-field -Wno-deprecated")
set (CMAKE_CXX_FLAGS_DEBUG          "-g -DDEBUG")
set (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set (CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework Cocoa -framework OpenGL -w")

find_package(GLEW REQUIRED)

set(COMMON_SRC
	common/include/common/vec3.h
	common/include/common/mat4.h
	common/include/common/vec4.h
	common/include/common/world.h
	common/include/common/input.h
	common/src/vec3.cpp
	common/src/vec4.cpp
	common/src/mat4.cpp
	common/src/world.cpp
)

add_library(servsim_common
	${COMMON_SRC}
)
target_include_directories(servsim_common PUBLIC
	common/include
)

add_executable (servsim_client 
	client/src/main.mm
	client/src/renderer.h
	client/src/renderer.cpp
)

target_link_libraries (servsim_client servsim_common ${GLEW_LIBRARIES})
target_include_directories (servsim_client PUBLIC ${GLEW_INCLUDES})

source_group (client src/client/*)
source_group (common src/common/*)
