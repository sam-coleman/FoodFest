cmake_minimum_required(VERSION 2.8)
project( FoodFest )
set(CMAKE_CXX_FLAGS "-O3")
add_subdirectory(./build/dlib/dlib-19.6 dlib_build)
find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS})

add_executable( fallingFood fallingFood.cpp food.h food.cpp)
add_executable( staticFood staticFood.cpp food.h food.cpp)

target_link_libraries(fallingFood dlib::dlib ${OpenCV_LIBS})
target_link_libraries(staticFood dlib::dlib ${OpenCV_LIBS} )

