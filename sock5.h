//
// Created by cayun on 2019-03-15.
//

#ifndef YYL_BRIDGE_SOCK5_H
#define YYL_BRIDGE_SOCK5_H

#include "ystring.h"

#define CMD_CONNECT 0x01
#define CMD_BIND 0x02
#define CMD_UDP 0x03

#define ATYP_IPV4 0x01
#define ATYP_DOMAIN 0x03
#define ATYP_IPV6 0x04

struct SocksValidateRequest {
    char version;
    char methodNum;
    char methods[300];
};

struct SocksValidateResponse {
    char version;
    char method;
};

struct SocksBuildRequest {
    char version;
    char cmd;
    char rsv;
    char atyp;
    ystring dstAddr;
    short dstPort;
};

struct SocksBuildResponse {
    char version;
    char rep;
    char rsv;
    char atyp;
    ystring bndAddr;
    short bndPort;
};

struct SocksValidateRequest socksVReqNew(ystring buffer);

ystring socksVRespToStr(struct SocksValidateResponse response);

struct SocksBuildRequest socksBReqNew(ystring buffer);

void socksBReqFree(struct SocksBuildRequest request);

ystring socksBRespToStr(struct SocksBuildResponse response);

void socksBRespFree(struct SocksBuildResponse response);

#endif //YYL_BRIDGE_SOCK5_H
