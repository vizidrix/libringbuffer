set(TITLE bench_c)

#add_library(benchmark STATIC IMPORTED)
#set_property(TARGET benchmark PROPERTY IMPORTED_LOCATION /c/vizidrix/src/github.com/vizidrix/libringbuffer/includes/libbenchmark.a)

#find_package( benchmark )

add_library(benchmark STATIC IMPORTED)
set_property(TARGET benchmark PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/includes/libbenchmark.a)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pthread -D_REENTRANT -fPIC" )

set(SOURCES bench.c)

add_executable(${TITLE} ${SOURCES})
target_link_libraries( ${TITLE} ringbuffer )
target_link_libraries( ${TITLE} benchmark )