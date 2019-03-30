//
// Created by cayun on 2019-03-20.
//

#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <memory.h>
#include <netdb.h>
#include "server.h"
#include "common.h"
#include "sock5.h"

int validateSock5Connection(int clientSock) {
    printf("validate sock5 connection.\n");
    char buffer[SOCK5_VALIDATE_REQUEST_MAX_LENGTH];

    if (retryRecv(clientSock, buffer, SOCK5_VALIDATE_REQUEST_MAX_LENGTH) < 0) {
        return -1;
    }

    struct Sock5ValidateRequest request = Sock5ValidateRequest_read((Byte *)buffer);

    int ok = 1;

    ok &= request.version == 0x05;    // 验证客户端协议版本是否正确

    // 验证客户端是否支持无验证方式的请求
    int allowNoAuth = 0;
    for (int i = 0; i < request.methodNum; i++) {
        if (request.methods[i] == 0x00) {
            allowNoAuth = 1;
            break;
        }
    }
    ok &= allowNoAuth;

    struct Sock5ValidateResponse response;
    response.version = 0x05;
    response.method = ok ? (Byte)0x00 : (Byte)0xFF;
    return (int)retrySend(clientSock, Sock5ValidateResponse_toString(response), 2);
}

int createSock5Connection(struct Config config, int clientSock) {
    printf("create sock5 connection.\n");
    char buffer[SOCK5_BUILD_REQUEST_MAX_LENGTH];
    if (retryRecv(clientSock, buffer, SOCK5_BUILD_REQUEST_MAX_LENGTH) < 0) {
        return -1;
    }

    struct Sock5BuildRequest request = Sock5BuildRequest_read((Byte *)buffer);
    if (request.version < 0) {
        return -1;
    }
    if (request.cmd != 0x01) {   // 目前支持CONNECT方式，后续扩展
        return -1;
    }

    // 代理服务器创建到远程主机的连接
    int remoteSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (remoteSock < 0) {
        return -1;
    }

    struct sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(request.dstPort);
    if (request.atyp == 0x01) {
        in_addr_t ip;
        memcpy(&ip, request.dstAddr, 4);
        remoteAddr.sin_addr.s_addr = htonl(ip);
    } else if (request.atyp == 0x03) {
        struct hostent *dstHost = gethostbyname(Byte_arrayToStr(request.dstAddr, request.addrLength));
        remoteAddr.sin_addr.s_addr = *(in_addr_t *)dstHost->h_addr;
    } else {
        return -1;
    }

    if (retryConnect(remoteSock, (struct sockaddr*)&remoteAddr, sizeof(remoteAddr)) < 0) {
        return -1;
    }

    // 响应客户端
    struct Sock5BuildResponse response;
    response.version = 0x05;
    response.rep = 0x00;
    response.rsv = 0x00;
    response.atyp = 0x01;
    response.addrLength = 4;
    uint ip = (127U << 24) + 1;
    response.bndAddr = (Byte *)&ip;
    response.bndPort = (ushort)config.localPort;
    char* responseStr = Sock5BuildResponse_toString(response);
    if (retrySend(clientSock, responseStr, Sock5BuildResponse_getLength(response)) < 0) {
        return -1;
    }

    return remoteSock;
}

void handleClientRequest(struct Config config, int clientSock) {
    printf("handle client socket.\n");
    if (validateSock5Connection(clientSock) < 0) {
        return;
    }
    int remoteSock = createSock5Connection(config, clientSock);
    if (remoteSock < 0) {
        return;
    }

    if (fork() == 0) {
        forwardData(clientSock, remoteSock, 1);
        exit(0);
    }
    if (fork() == 0) {
        forwardData(remoteSock, clientSock, 2);
        exit(0);
    }
}


void serverLoop(struct Config config, int serverSock) {
    printf("server loop...\n");
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLength = sizeof(clientAddr);

    while (1) {
        printf("wait for client...\n");
        int clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientAddrLength);
        if (clientSock == -1) {
            continue;
        }
        printf("client sock: %d\n", clientSock);
        if (fork() == 0) {
            close(serverSock);
            handleClientRequest(config, clientSock);
            exit(0);
        }
        close(clientSock);
    }
}

void startServer(struct Config config) {
    printf("start server...\n");
    int serverSock = createListeningSocket(config.localPort);
    if (serverSock < 0) {
        printf("Cannot create server socket.");
        exit(serverSock);
    }

    serverLoop(config, serverSock);
}
