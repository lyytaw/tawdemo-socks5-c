//
// Created by cayun on 2019-03-20.
//

#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include <unistd.h>
#include <stdlib.h>
#include "client.h"
#include "config.h"
#include "ysocket.h"
#include "ynet.h"

int handleUserRequest(struct Config config, int userSock) {
    int serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock < 0) {
        return -1;
    }

    ysocketSetTimeout(serverSock);

    struct sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(config.serverPort);
    struct hostent *dstHost = gethostbyname(ystringData(config.serverHost));
    remoteAddr.sin_addr.s_addr = *(in_addr_t *)dstHost->h_addr;

    if (!ysocketConnect(serverSock, (struct sockaddr*)&remoteAddr, sizeof(remoteAddr))) {
        return -1;
    }

    int forkRet = fork();
    if (forkRet == 0) {
        ynetForward(userSock, serverSock, 2);
    } else if (forkRet > 0) {
        ynetForward(serverSock, userSock, 1);
    }

    return 0;
}

void clientLoop(struct Config config, int clientSock) {
    printf("client loop...\n");
    struct sockaddr_in userAddr;
    socklen_t clientAddrLength = sizeof(userAddr);

    while (1) {
        int userSock = accept(clientSock, (struct sockaddr *)&userAddr, &clientAddrLength);
        if (userSock == -1) {
            continue;
        }
        if (fork() == 0) {
            close(clientSock);
            ysocketSetTimeout(userSock);
            handleUserRequest(config, userSock);
            exit(0);
        }
        close(userSock);
    }
}

void startClient(struct Config config) {
    printf("start client...\n");
    int clientSock = ynetCreateListenSock(config.localPort);
    if (clientSock < 0) {
        printf("Cannot create listening socket on port %d.\n", config.localPort);
        exit(clientSock);
    }

    clientLoop(config, clientSock);
}