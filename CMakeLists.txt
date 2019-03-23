cmake_minimum_required(VERSION 3.13)

project(conways-game-of-life VERSION 0.0.1 LANGUAGES C)

add_executable(${PROJECT_NAME} main.c)

target_compile_options(${PROJECT_NAME} PRIVATE "-Wall;-Wextra;")
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 11)
target_link_libraries(${PROJECT_NAME} curses)