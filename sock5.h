//
// Created by cayun on 2019-03-15.
//

#ifndef MZZ_BRIDGE_C_SOCK5_H
#define MZZ_BRIDGE_C_SOCK5_H

#include "common.h"

#define SOCK5_VALIDATE_REQUEST_MAX_LENGTH 3
#define SOCK5_BUILD_REQUEST_MAX_LENGTH 263

struct Sock5ValidateRequest {
    Byte version;
    Byte methodNum;
    Byte *methods;
};

struct Sock5ValidateResponse {
    Byte version;
    Byte method;
};

struct Sock5BuildRequest {
    Byte version;
    Byte cmd;
    Byte rsv;
    Byte atyp;
    Byte addrLength;  // 地址长度
    Byte *dstAddr;
    ushort dstPort;
};

struct Sock5BuildResponse {
    Byte version;
    Byte rep;
    Byte rsv;
    Byte atyp;
    Byte addrLength; // 地址长度
    Byte *bndAddr;
    ushort bndPort;
};

/**
 * 读取sock5验证请求
 * @param buffer
 * @return
 */
struct Sock5ValidateRequest Sock5ValidateRequest_read(Byte *buffer);

/**
 * 获取Sock5ValidateResponse的总长度
 * @param response
 * @return
 */
size_t Sock5ValidateResponse_getLength(struct Sock5ValidateResponse response);

/**
 * 将sock5验证响应转换为字符串
 * @param response
 * @return
 */
char* Sock5ValidateResponse_toString(struct Sock5ValidateResponse response);

/**
 * 读取sock5建立连接请求
 * @param buffer
 * @return
 */
struct Sock5BuildRequest Sock5BuildRequest_read(Byte *buffer);

/**
 * 获取Sock5BuildResponse的总长度
 * @param response
 * @return
 */
size_t Sock5BuildResponse_getLength(struct Sock5BuildResponse response);

/**
 * 将sock5建立连接请求转换为字符串
 * @param response
 * @return
 */
char* Sock5BuildResponse_toString(struct Sock5BuildResponse response);

#endif //MZZ_BRIDGE_C_SOCK5_H
