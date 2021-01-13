cmake_minimum_required(VERSION 2.6)
project(create_raster)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11")
#find_package( OpenCV REQUIRED )
#include_directories( ${OpenCV_INCLUDE_DIRS} )
file(
GLOB_RECURSE
source_files
src/*.cpp
)
add_executable(
${PROJECT_NAME}
${source_files}
)
target_link_libraries(${PROJECT_NAME} proj) #${OpenCV_LIBS}
