cmake_minimum_required(VERSION 3.24)

include(FetchContent)

FetchContent_Declare(
  dyn-array
  GIT_REPOSITORY https://github.com/pr0g/dyn-array.git
  GIT_TAG 7fd336909d3b53f44e643f9110e14cdf666d3b59)

FetchContent_MakeAvailable(dyn-array)

project(
  conways-game-of-life
  VERSION 0.0.1
  LANGUAGES C)
add_executable(${PROJECT_NAME} main.c)
target_compile_features(${PROJECT_NAME} PRIVATE c_std_11)
target_link_libraries(${PROJECT_NAME} PRIVATE dyn-array)
