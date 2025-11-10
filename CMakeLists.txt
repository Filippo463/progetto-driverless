cmake_minimum_required(VERSION 3.10)

project(DriverlessControl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)


FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.0 
)

FetchContent_MakeAvailable(raylib)

file(GLOB SOURCES "src/*.cpp")

add_executable(my_car_simulator ${SOURCES})

target_include_directories(my_car_simulator PUBLIC include)


target_link_libraries(
  my_car_simulator
  PUBLIC
  raylib
)
