#To compile faster: cmake .. -DUSE_SSE4_INSTRUCTIONS=ON 

cmake_minimum_required(VERSION 2.8)
project( FoodFest )
set(CMAKE_CXX_FLAGS "-O3")
add_subdirectory(./build/dlib/dlib-19.6 dlib_build)
find_package( OpenCV REQUIRED )
#find_package(dlib REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS})
#include_directories(${DLIB_INCLUDE_DIRECTORY}) 
add_executable( project project.cpp )
add_executable(dlibtest dlibTest.cpp)
add_executable(own dlibOwn.cpp)
target_link_libraries(dlibtest dlib::dlib)
target_link_libraries(own dlib::dlib)
target_link_libraries( dlibtest ${OpenCV_LIBS} )
target_link_libraries( project ${OpenCV_LIBS} )
target_link_libraries( own ${OpenCV_LIBS} )

