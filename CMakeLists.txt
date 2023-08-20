cmake_minimum_required(VERSION 3.5)

project(DoodleJump)

find_package(SFML REQUIRED network audio graphics window system)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/res" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

add_executable(doodle src/main.cpp)

target_link_libraries(doodle PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio)