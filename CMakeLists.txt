cmake_minimum_required(VERSION 3.18)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 17)

project(cpp-playground LANGUAGES CXX)

if (MSVC) 
    add_compile_options(/Wall)
else()
    add_compile_options(-Wall -Wextra -Werror=return-type)
endif()

file(GLOB sources CONFIGURE_DEPENDS *.cpp)
foreach (source IN ITEMS ${sources})
    get_filename_component(name "${source}" NAME_WLE)
    add_executable(${name} ${name}.cpp)
endforeach()