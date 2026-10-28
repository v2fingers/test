cmake_minimum_required(VERSION 3.20)
project(Engine C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-O0 -g -DDEBUG -DTODOS)
else()
  add_compile_options(-O3 -march=native -flto)
  add_link_options(-flto)
endif()

file(GLOB_RECURSE RUNTIME_SRC CONFIGURE_DEPENDS
  runtime/src/*.c
  runtime/vendor/*.c
)

file(GLOB_RECURSE TESTBED_SRC CONFIGURE_DEPENDS
  testbed/src/*.c
)

add_library(Runtime STATIC
  ${RUNTIME_SRC}
  runtime/vendor/vma/vma.cpp
)

set_source_files_properties(runtime/vendor/vma/vma.cpp
  PROPERTIES COMPILE_FLAGS "-Wno-nullability-completeness"
)

add_executable(testbed ${TESTBED_SRC})

include_directories(
  runtime/src
  runtime/vendor
)

target_link_libraries(testbed PRIVATE
  Runtime
  vulkan
  glfw
)
