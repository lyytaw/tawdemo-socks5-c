//
// Created by cayun on 2019-03-15.
//

#ifndef YYL_BRIDGE_CONFIG_H
#define YYL_BRIDGE_CONFIG_H

struct Config {
    int localPort;   // -p [port], 本地监听端口
    int client;      // 客户端模式
    int server;      // 服务端模式
    char* serverHost;  // 客户端模式下，需要指定服务端地址
    int serverPort;  // 客户端模式下，需要指定服务端端口
};

#endif //YYL_BRIDGE_CONFIG_H
