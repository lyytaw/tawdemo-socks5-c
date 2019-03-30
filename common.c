//
// Created by cayun on 2019-03-17.
//

#include <stdio.h>
#include <sys/socket.h>
#include <memory.h>
#include <netinet/in.h>
#include <zlib.h>
#include "common.h"
#include "exception.h"

#define RETRY_TIME 10

Byte* Byte_copyN(Byte *dst, const Byte *src, size_t n) {
    memcpy(dst, src, n);
    return dst;
}

char* Byte_arrayToStr(Byte *src, size_t n) {
    char* result = (char *)malloc(n + 1);
    memcpy(result, src, n);
    result[n] = '\0';
    return result;
}

int	retryConnect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    for (int i = 0; i < RETRY_TIME; i++) {
        int result = connect(sockfd, addr, addrlen);
        if (result >= 0) {
            return result;
        }
    }
    return -1;
}

ssize_t retryRecv(int sockfd, void *buf, size_t len) {
    for (int i = 0; i < RETRY_TIME; i++) {
        ssize_t result = recv(sockfd, buf, len, 0);
        if (result >= 0) {
            return result;
        }
    }
    return -1;
}

ssize_t retrySend(int sockfd, const void *buf, size_t len) {
    for (int i = 0; i < RETRY_TIME; i++) {
        ssize_t result = send(sockfd, buf, len, 0);
        if (result >= 0) {
            return result;
        }
    }
    return -1;
}

int createListeningSocket(int port) {
    int listeningSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listeningSock < 0) {
        return SERVER_SOCKET_CREATE_ERROR;
    }

    int optval;
    setsockopt(listeningSock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listeningSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        return SERVER_SOCKET_BIND_ERROR;
    }

    if (listen(listeningSock, 30) != 0) {
        return SERVER_SOCKET_LISTEN_ERROR;
    }

    return listeningSock;
}

void forwardData(int srcSock, int dstSock, int mark) {
    char buffer[8192];
    ssize_t n;
    while ((n = retryRecv(srcSock, buffer, 8000)) > 0) {
        if (mark == 1) {  // 接收时解压数据
            char dstBuffer[8192];
            uLong dstLen;
            uncompress((Byte *)dstBuffer, &dstLen, (Byte *)buffer, (uLong)n);
            if (retrySend(dstSock, dstBuffer, dstLen) < 0) {
                break;
            }
        } else if (mark == 2) {  // 发送前压缩数据
            char dstBuffer[8192];
            uLong dstLen;
            compress((Byte *)dstBuffer, &dstLen, (Byte *)buffer, (uLong)n);
            if (retrySend(dstSock, dstBuffer, dstLen) < 0) {
                break;
            }
        } else {  // 不做任何处理
            if (retrySend(dstSock, buffer, (size_t)n) < 0) {
                break;
            }
        }
    }
    shutdown(srcSock, SHUT_RDWR);
    shutdown(dstSock, SHUT_RDWR);
}