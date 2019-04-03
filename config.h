//
// Created by cayun on 2019-03-15.
//

#ifndef YYL_BRIDGE_CONFIG_H
#define YYL_BRIDGE_CONFIG_H

#include <stdbool.h>
#include "ystring.h"

struct Config {
    int localPort;   // -p [port], 本地监听端口
    bool client;      // 客户端模式
    bool server;      // 服务端模式
    ystring serverHost;  // 客户端模式下，需要指定服务端地址
    int serverPort;  // 客户端模式下，需要指定服务端端口
};

struct Config configNew(int argc, char *argv[]);

void configFree(struct Config config);

#endif //YYL_BRIDGE_CONFIG_H
