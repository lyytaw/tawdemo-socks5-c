cmake_minimum_required(VERSION 3.7)
project(yyl_bridge C)

set(CMAKE_C_STANDARD 99)

add_executable(yyl_bridge main.c sock5.c sock5.h config.c config.h exception.h common.h common.c client.c client.h server.c server.h)

find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries(yyl_bridge ${ZLIB_LIBRARIES})
endif(ZLIB_FOUND)
