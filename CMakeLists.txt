cmake_minimum_required(VERSION 2.6)
project(create_raster)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11")
file(
GLOB_RECURSE
source_files
src/*.cpp
)
add_executable(
create_raster
${source_files}
)
target_link_libraries(${PROJECT_NAME} proj)
