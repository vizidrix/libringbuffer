set(TITLE bench_c)

#set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/libfolder") 

#add_library(benchmark STATIC IMPORTED)
#set_property(TARGET benchmark PROPERTY IMPORTED_LOCATION /c/vizidrix/src/github.com/vizidrix/libringbuffer/includes/libbenchmark.a)

#find_package( benchmark )

#add_library(benchmark STATIC IMPORTED)
#set_property(TARGET benchmark PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/includes/libbenchmark.a)

set(BUILD_SHARED_LIBS FALSE)
set(CMAKE_EXE_LINKER_FLGAS '-static')

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pthread -D_REENTRANT -fPIC" )

set(SOURCES bench.c)

add_executable(${TITLE} ${SOURCES})
target_link_libraries( ${TITLE} ringbuffer )
#target_link_libraries( ${TITLE} /c/vizidrix/src/github.com/vizidrix/libringbuffer/includes/1.0.0/libbenchmark.a )