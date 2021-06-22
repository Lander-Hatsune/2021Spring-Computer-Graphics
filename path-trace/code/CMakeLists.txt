cmake_minimum_required(VERSION 2.6)
project(path-trace CXX)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif ()

add_subdirectory(deps/vecmath)

set(SRC
  src/main.cpp
  src/scene_parser.cpp
  src/image.cpp
  src/mesh.cpp
  src/material.cpp)

set(INCLUDE
  include/camera.hpp
  include/group.hpp
  include/hit.hpp
  include/image.hpp
  include/light.hpp
  include/material.hpp
  include/mesh.hpp
  include/object3d.hpp
  include/plane.hpp
  include/ray.hpp
  include/scene_parser.hpp
  include/sphere.hpp
  include/transform.hpp
  include/triangle.hpp
  include/curve.hpp
  include/revsurface.hpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-fopenmp -O3")


add_executable(${PROJECT_NAME} ${SRC} ${INCLUDE})
target_link_libraries(${PROJECT_NAME} vecmath)
target_include_directories(${PROJECT_NAME} PRIVATE include/)
