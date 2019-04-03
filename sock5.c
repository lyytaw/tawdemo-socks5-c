//
// Created by cayun on 2019-03-15.
//

#include <string.h>
#include <stdlib.h>
#include "sock5.h"
#include "ystring.h"

struct SocksValidateRequest socksVReqNew(ystring buffer) {
    struct SocksValidateRequest request;
    char *data = ystringData(buffer);
    request.version = data[0];
    request.methodNum = data[1];
    memcpy(request.methods, data + 2, request.methodNum);
    return request;
}

ystring socksVRespToStr(struct SocksValidateResponse response) {
    char buf[2];
    buf[0] = response.version;
    buf[1] = response.method;
    return ystringNewN(buf, 2);
}

struct SocksBuildRequest socksBReqNew(ystring buffer) {
    struct SocksBuildRequest request;
    char *data = ystringData(buffer);
    int p = 0;
    request.version = data[p++];
    request.cmd = data[p++];
    request.rsv = data[p++];
    request.atyp = data[p++];
    if (request.atyp == ATYP_IPV4) {
        char buf[4];
        memcpy(buf, data + p, 4);
        request.dstAddr = ystringNewN(buf, 4);
        p += 4;
    } else if (request.atyp == ATYP_DOMAIN) {
        int len = data[p++];
        char *buf = (char *)malloc((size_t)len);
        memcpy(buf, data + p, len);
        request.dstAddr = ystringNewN(buf, len);
        free(buf);
        p += len;
    } else if (request.atyp == ATYP_IPV6) {
        char buf[16];
        memcpy(buf, data + p, 16);
        request.dstAddr = ystringNewN(buf, 16);
        p += 16;
    }
    memcpy(&request.dstPort, data + p, 2);
    return request;
}

void socksBReqFree(struct SocksBuildRequest request) {
    ystringFree(request.dstAddr);
}

ystring socksBRespToStr(struct SocksBuildResponse response) {
    int len = 6 + ystringLength(response.bndAddr) + (int)(response.atyp == ATYP_DOMAIN);
    char *buf = malloc((size_t)len);
    int p = 0;
    buf[p++] = response.version;
    buf[p++] = response.rep;
    buf[p++] = response.rsv;
    buf[p++] = response.atyp;
    if (response.atyp == ATYP_DOMAIN) {
        memcpy(buf + p, ystringData(response.bndAddr), ystringLength(response.bndAddr));
        p += ystringLength(response.bndAddr);
    }
    memcpy(buf + p, &response.bndPort, 2);
    return ystringNewN(buf, len);
}

void socksBRespFree(struct SocksBuildResponse response) {
    ystringFree(response.bndAddr);
}
