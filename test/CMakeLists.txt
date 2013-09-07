set(TITLE libringbuffer_test)

set(SOURCES test.cpp)

include_directories(${GTEST_INCLUDE_DIRS})

set_source_files_properties( ${SOURCES}
                             PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread -D_REENTRANT -Wno-write-strings" )

add_executable(${TITLE} ${SOURCES})
target_link_libraries(${TITLE} ${GTEST_BOTH_LIBRARIES} ringbuffer ${CMAKE_THREAD_LIBS_INIT})
add_test(${TITLE} ${EXECUTABLE_OUTPUT_PATH}/${TITLE})


