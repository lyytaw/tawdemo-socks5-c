//
// Created by cayun on 2019-03-15.
//

#include <string.h>
#include <stdlib.h>
#include "sock5.h"
#include "common.h"

struct Sock5ValidateRequest Sock5ValidateRequest_read(Byte *buffer) {
    struct Sock5ValidateRequest request;
    request.version = buffer[0];
    request.methodNum = buffer[1];
    request.methods = (Byte *)malloc(request.methodNum);
    Byte_copyN(request.methods, buffer + 2, request.methodNum);
    return request;
}

size_t Sock5ValidateResponse_getLength(struct Sock5ValidateResponse response) {
    return 2;
}

char* Sock5ValidateResponse_toString(struct Sock5ValidateResponse response) {
    Byte *result = (Byte *)malloc(3);
    int p = 0;
    result[p++] = response.version;
    result[p] = response.method;
    return (char *)result;
}

struct Sock5BuildRequest Sock5BuildRequest_read(Byte *buffer) {
    struct Sock5BuildRequest request;
    int p = 0;
    request.version = buffer[p++];
    request.cmd = buffer[p++];
    request.rsv = buffer[p++];
    request.atyp = buffer[p++];
    switch (request.atyp) {
        case 0x01: request.addrLength = 4; break;
        case 0x03: request.addrLength = buffer[p++]; break;
        case 0x04: request.addrLength = 16; break;
        default: request.addrLength = 0;
    }
    request.dstAddr = (Byte *)malloc(request.addrLength);
    Byte_copyN(request.dstAddr, buffer + p, request.addrLength);
    p += request.addrLength;
    request.dstPort = (buffer[p] << 8) + buffer[p+1];
    return request;
}

size_t Sock5BuildResponse_getLength(struct Sock5BuildResponse response) {
    return response.addrLength + (response.atyp == 0x03 ? 7 : 6);
}

char* Sock5BuildResponse_toString(struct Sock5BuildResponse response) {
    Byte *result = (Byte *)malloc(Sock5BuildResponse_getLength(response));
    int p = 0;
    result[p++] = response.version;
    result[p++] = response.rep;
    result[p++] = response.rsv;
    result[p++] = response.atyp;
    if (response.atyp == 0x03) {
        result[p++] = response.addrLength;
    }
    Byte_copyN(result + p, response.bndAddr, response.addrLength);
    p += response.addrLength;
    result[p++] = (Byte)(response.bndPort >> 8);
    result[p] = (Byte)(response.bndPort & 0xff);
    return (char *)result;
}
