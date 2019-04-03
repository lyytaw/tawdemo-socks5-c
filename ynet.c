//
// Created by cayun on 2019-04-01.
//

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "ynet.h"
#include "yzip.h"

ystring ynetRecv(int sockfd, bool unzip) {
    ystring data = ysocketRecv(sockfd, unzip);
    if (unzip) {
        ystring unzipData = yzipUncompress(data);
        ystringFree(data);
        return unzipData;
    } else {
        return data;
    }
}

bool ynetSend(int sockfd, ystring str, bool zip) {
    if (zip) {
        ystring zipData = yzipCompress(str);
        bool ret = ysocketSend(sockfd, zipData, zip);
        ystringFree(zipData);
        return ret;
    } else {
        return ysocketSend(sockfd, str, zip);
    }
}

void ynetForward(int srcSockfd, int dstSockfd, int mark) {
    while (true) {
        ystring recvData = ynetRecv(srcSockfd, mark == UNZIP_AFTER_RECV);
        if (ystringEmpty(recvData)) {
            ystringFree(recvData);
            break;
        }
        bool sendRet = ynetSend(dstSockfd, recvData, mark == ZIP_BEFORE_SEND);
        ystringFree(recvData);
        if (!sendRet) {
            break;
        }
    }
    shutdown(srcSockfd, SHUT_RDWR);
    shutdown(dstSockfd, SHUT_RDWR);
}

int ynetCreateListenSock(int port) {
    int listeningSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listeningSock < 0) {
        return -1;
    }

    int optval;
    setsockopt(listeningSock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listeningSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        return -1;
    }

    if (listen(listeningSock, 30) != 0) {
        return -1;
    }

    return listeningSock;
}
