cmake_minimum_required(VERSION 3.20)
project(VkTests LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-O0 -g -DDEBUG -Wall -Wextra)
else()
  add_compile_options(-O3 -march=native -flto)
  add_link_options(-flto)
endif()

include_directories(
  src
  vendor
)

set_source_files_properties(vendor/vma/vma.cpp
  PROPERTIES COMPILE_OPTIONS "-Wno-nullability-completeness"
)

file(GLOB_RECURSE TESTBED_SRC CONFIGURE_DEPENDS
  src/*.c
)

add_executable(vktest
  ${TESTBED_SRC}
  vendor/vma/vma.cpp
)

target_link_libraries(vktest PRIVATE
  vulkan
  glfw
)
