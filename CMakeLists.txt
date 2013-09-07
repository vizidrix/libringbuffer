cmake_minimum_required(VERSION 2.6)

project(LIBRINGBUFFER)

set(LIBRINGBUFFER_SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBRINGBUFFER_BUILD_ROOT ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${LIBRINGBUFFER_BUILD_ROOT}/lib" )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${LIBRINGBUFFER_BUILD_ROOT}/lib" )
set(EXECUTABLE_OUTPUT_PATH         "${LIBRINGBUFFER_BUILD_ROOT}/bin" )

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -pedantic-errors -Wall -pedantic -Wextra")

#add_library(benchmark STATIC IMPORTED)
#set_property(TARGET benchmark PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/includes/libbenchmark.a)

include_directories(${LIBRINGBUFFER_SRC_ROOT})
add_subdirectory(src)
add_subdirectory(bench)
add_subdirectory(example)

find_package(GTest)
find_package(Threads)
if (GTEST_FOUND AND Threads_FOUND)
    enable_testing()
    add_subdirectory(test)
else(GTEST_FOUND AND Threads_FOUND)
    message(STATUS "No gtest or pthreads found - test not build")
endif(GTEST_FOUND AND Threads_FOUND)

#target_link_libraries( ${TITLE} benchmark )

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)