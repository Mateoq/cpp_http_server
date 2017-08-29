# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.0 )

# Project's name

project(HttpServer)
# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_C_COMPILER /usr/local/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/local/bin/g++)

# Set the compiler config
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(flags "-std=c++1z -m64 -Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(flags "-std=c++1z -stdlib=libc++ -m64 -Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(flags "/std:c++17 /W4 /WX /EHsc /ZI /MACHINE:X64")
endif()

if (NOT CONFIGURED_ONCE)
  set(CMAKE_CXX_FLAGS "${flags}")
endif()

find_package(Boost 1.64.0 REQUIRED COMPONENTS system)

# The following folder will be included
include_directories("${CMAKE_SOURCE_DIR}")
include_directories(${CMAKE_SOURCE_DIR}/dependencies/include)

add_executable(HttpServer
  ${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/server.cpp
	${PROJECT_SOURCE_DIR}/src/connection_manager.cpp
	${PROJECT_SOURCE_DIR}/src/request_parser.cpp
	${PROJECT_SOURCE_DIR}/src/request_handler.cpp
	${PROJECT_SOURCE_DIR}/src/response.cpp
	${PROJECT_SOURCE_DIR}/src/mime_types.cpp
	${PROJECT_SOURCE_DIR}/src/connection.cpp)

target_link_libraries(HttpServer Boost::system)
