cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Interpreter)

file(GLOB_RECURSE source source/*.cpp)

include_directories(${CMAKE_SOURCE_DIR}/libs ${CMAKE_SOURCE_DIR}/include)

include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.9.1)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_executable(interpreter ${source})
target_compile_options(interpreter PRIVATE -flto -Wl,-allow-multiple-definition -fuse-linker-plugin)

target_link_libraries(interpreter PRIVATE nlohmann_json::nlohmann_json)
