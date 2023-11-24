cmake_minimum_required(VERSION 3.24)
project(conways-game-of-life VERSION 0.0.1 LANGUAGES C)
add_executable(${PROJECT_NAME} main.c)
target_compile_features(${PROJECT_NAME} PRIVATE c_std_11)
