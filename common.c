//
// Created by cayun on 2019-03-17.
//

#include <sys/socket.h>
#include <memory.h>
#include "common.h"

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
