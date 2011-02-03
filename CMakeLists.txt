  project (harlan)
cmake_minimum_required(VERSION 2.8)

include_directories("/usr/include/SDL")

add_executable(harlan src/main.cpp)
target_link_libraries(harlan SDL)
target_link_libraries(harlan SDL_image)
target_link_libraries(harlan GL)
