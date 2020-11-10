CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)

project(vector_pbf)

if(CMAKE_VERSION VERSION_LESS "3.1")
   add_definitions(-std=c++11)
else()
   set(CMAKE_CXX_STANDARD 11)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)
   if(NOT CYGWIN)
	set(CMAKE_CXX_EXTENSIONS OFF) 
   endif()
endif()

include_directories(include)
include_directories(include/protozero)
include_directories(include/vtzero)

add_executable(${PROJECT_NAME} vtile-show.cpp)
#add_executable(test tile_handler.cpp)
