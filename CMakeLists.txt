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
	src/common/mat4.h
	src/common/mat4.cpp
	src/common/vec3.h
	src/common/vec3.cpp
	src/common/vec4.h
	src/common/vec4.cpp
	src/common/world.h
	src/common/world.cpp
	src/common/input.h
)
add_executable (Servsim 
	src/client/main.mm
	src/client/renderer.h
	src/client/renderer.cpp
	${COMMON_SRC}
)

target_link_libraries(Servsim ${GLEW_LIBRARIES})
target_include_directories(Servsim PUBLIC ${GLEW_INCLUDES})
