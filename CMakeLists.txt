cmake_minimum_required(VERSION 3.0.0)
project(redis-tutorial VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
include(CTest)
enable_testing()

include_directories(/usr/local/include/hiredis /usr/local/include/ae)

link_libraries(hiredis /usr/local/lib64/libuv.so pthread)
link_libraries(event event_core event_pthreads event_extra)

# add_executable(SET_GET_blocking SET_GET_blocking.cpp)
# add_executable(async_tutorial async_tutorial.cpp)

add_executable(mytest mytest.cpp)

add_subdirectory(libuv)
add_subdirectory(async_libuv)
add_subdirectory(async_libevent)

add_subdirectory(connect)
add_subdirectory(redis_command)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

