cmake_minimum_required(VERSION 3.7)
project(tawdemo-socks5-c C)

set(CMAKE_C_STANDARD 99)

add_executable(tawdemo-socks5-c main.c sock5.c sock5.h config.c config.h exception.h common.h common.c client.c client.h server.c server.h)
