//
// Created by cayun on 2019-03-15.
//

#ifndef YYL_BRIDGE_CONFIG_H
#define YYL_BRIDGE_CONFIG_H

struct Config {
    int localPort;   // -p [port], 本地监听端口
    int daemon;      // -d, 是否后台运行
};

#endif //YYL_BRIDGE_CONFIG_H
