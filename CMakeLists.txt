cmake_minimum_required(VERSION 3.5.1)
project(MindVisionCamera)
find_package(OpenCV REQUIRED)
include_directories("${PROJECT_SOURCE_DIR}/include")
link_libraries("/lib/libMVSDK.so")
set(CMAKE_CXX_STANDARD 14)
add_executable(MindVisionCamera 
src/main.cpp 
src/MindVisionCamera.cpp 
src/armordetector.cpp)
target_link_libraries(MindVisionCamera ${OpenCV_LIBS})
