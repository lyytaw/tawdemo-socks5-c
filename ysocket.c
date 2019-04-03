//
// Created by cayun on 2019-04-01.
//

#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ysocket.h"

#define RETRY_TIME 1
#define MAX_DATA_LENGTH 300000

char recvBuf[MAX_DATA_LENGTH];

int ysocketRecvInt(int sockfd);


bool ysocketConnect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    for (int i = 0; i < RETRY_TIME; i++) {
        int result = connect(sockfd, addr, addrlen);
        if (result >= 0) {
            return true;
        }
    }
    return false;
}

ystring ysocketRecv(int sockfd, bool unzip) {
    if (unzip) {
        int len = ysocketRecvInt(sockfd);
        if (len > 0) {
            char *buf = (char*)malloc((size_t)len);
            for (int i = 0; i < RETRY_TIME; i++) {
                int recvLen = (int)recv(sockfd, buf, (size_t)len, 0);
                if (recvLen >= 0) {
                    ystring result = ystringNewN(buf, len);
                    free(buf);
                    return result;
                }
            }
        }
    } else {
        for (int i = 0; i < RETRY_TIME; i++) {
            int recvLen = (int)recv(sockfd, recvBuf, MAX_DATA_LENGTH, 0);
            if (recvLen >= 0) {
                return ystringNewN(recvBuf, recvLen);
            }
        }
    }
    return ystringNew("");
}

bool ysocketSend(int sockfd, ystring str, bool zip) {
    bool sendRet = true;
    if (zip) {
        int len = ystringLength(str);
        char *buf = (char *)malloc(sizeof(int) + len);
        memcpy(buf, &len, sizeof(int));
        memcpy(buf + sizeof(int), ystringData(str), len);
        for (int i = 0; i < RETRY_TIME; i++) {
            sendRet = send(sockfd, buf, len + sizeof(int), 0) >= 0;
            if (sendRet) {
                break;
            }
        }
    } else {
        for (int i = 0; i < RETRY_TIME; i++) {
            sendRet = send(sockfd, ystringData(str), (size_t)ystringLength(str), 0) >= 0;
            if (sendRet) {
                break;
            }
        }
    }
    return sendRet;
}



int ysocketRecvInt(int sockfd) {
    int result = 0, recvLen = -1;
    char buf[sizeof(int)];
    for (int i = 0; i < RETRY_TIME && recvLen < 0; i++) {
        recvLen = (int)recv(sockfd, buf, sizeof(int), 0);
    }
    if (recvLen < 0) {
        return 0;
    }
    memcpy(&result, buf, sizeof(int));
    return result;
}

void ysocketSetTimeout(int sockfd) {
    struct timeval timeout = {10, 0};
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
}
