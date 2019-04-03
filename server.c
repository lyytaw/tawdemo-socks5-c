//
// Created by cayun on 2019-03-20.
//

#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <memory.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdbool.h>
#include "server.h"
#include "sock5.h"
#include "ynet.h"

bool validateSock5Connection(int clientSock) {
    ystring buffer = ynetRecv(clientSock, true);
    if (ystringEmpty(buffer)) {
        ystringFree(buffer);
        return false;
    }

    struct SocksValidateRequest request = socksVReqNew(buffer);

    bool ok = true;

    ok = ok && (request.version == 0x05);    // 验证客户端协议版本是否正确

    // 验证客户端是否支持无验证方式的请求
    bool allowNoAuth = false;
    for (int i = 0; i < request.methodNum; i++) {
        if (request.methods[i] == 0x00) {
            allowNoAuth = true;
            break;
        }
    }
    ok = ok && allowNoAuth;

    struct SocksValidateResponse response;
    response.version = 0x05;
    response.method = ok ? (char)0x00 : (char)0xFF;
    ystring respStr = socksVRespToStr(response);
    bool ret = ynetSend(clientSock, respStr, true);

    ystringFree(buffer);
    ystringFree(respStr);

    return ret;
}

int createSock5Connection(struct Config config, int clientSock) {
    ystring buffer = ynetRecv(clientSock, true);
    if (ystringEmpty(buffer)) {
        ystringFree(buffer);
        return -1;
    }

    struct SocksBuildRequest request = socksBReqNew(buffer);
    ystringFree(buffer);

    if (request.version != 0x05) {
        socksBReqFree(request);
        return -1;
    }

    if (request.cmd != CMD_CONNECT) {   // 目前支持CONNECT方式，后续扩展
        socksBReqFree(request);
        return -1;
    }

    // 代理服务器创建到远程主机的连接
    int remoteSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (remoteSock < 0) {
        socksBReqFree(request);
        return -1;
    }

    struct sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = (in_port_t)request.dstPort;
    if (request.atyp == ATYP_IPV4) {
        in_addr_t ip;
        memcpy(&ip, request.dstAddr, 4);
        remoteAddr.sin_addr.s_addr = htonl(ip);
    } else if (request.atyp == ATYP_DOMAIN) {
        struct hostent *dstHost = gethostbyname(ystringData(request.dstAddr));
        remoteAddr.sin_addr.s_addr = *(in_addr_t *)dstHost->h_addr;
    } else {
        socksBReqFree(request);
        return -1;
    }
    socksBReqFree(request);

    if (!ysocketConnect(remoteSock, (struct sockaddr*)&remoteAddr, sizeof(remoteAddr))) {
        return -1;
    }

    // 响应客户端
    struct SocksBuildResponse response;
    response.version = 0x05;
    response.rep = 0x00;
    response.rsv = 0x00;
    response.atyp = ATYP_IPV4;
    char ipStr[4];
    uint ip = (127U << 24) + 1;
    memcpy(ipStr, &ip, 4);
    response.bndAddr = ystringNewN(ipStr, 4);
    response.bndPort = 4321;

    ystring respStr = socksBRespToStr(response);
    ynetSend(clientSock, respStr, true);

    ystringFree(respStr);
    socksBRespFree(response);

    return remoteSock;
}

void handleClientRequest(struct Config config, int clientSock) {
    if (!validateSock5Connection(clientSock)) {
        return;
    }
    int remoteSock = createSock5Connection(config, clientSock);
    if (remoteSock < 0) {
        return;
    }

    ysocketSetTimeout(remoteSock);

    int forkRet = fork();
    if (forkRet == 0) {
        ynetForward(clientSock, remoteSock, UNZIP_AFTER_RECV);
    } else if (forkRet > 0) {
        ynetForward(remoteSock, clientSock, ZIP_BEFORE_SEND);
    }
}


void serverLoop(struct Config config, int serverSock) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLength = sizeof(clientAddr);

    while (1) {
        int clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientAddrLength);
        if (clientSock == -1) {
            continue;
        }
        if (fork() == 0) {
            close(serverSock);
            ysocketSetTimeout(clientSock);
            handleClientRequest(config, clientSock);
            exit(0);
        }
        close(clientSock);
    }
}

void startServer(struct Config config) {
    int serverSock = ynetCreateListenSock(config.localPort);
    if (serverSock < 0) {
        exit(serverSock);
    }

    serverLoop(config, serverSock);
}
