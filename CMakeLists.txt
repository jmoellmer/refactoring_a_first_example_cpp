cmake_minimum_required(VERSION 3.14)
project(Refactoring)

set(CMAKE_CXX_STANDARD 17)
find_package(nlohmann_json REQUIRED)

add_executable(Refactoring main.cpp Statement.cpp Statement.h)
target_link_libraries(Refactoring PRIVATE nlohmann_json::nlohmann_json)