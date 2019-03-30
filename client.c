//
// Created by cayun on 2019-03-20.
//

#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include <unistd.h>
#include "client.h"
#include "common.h"
#include "config.h"

int handleUserRequest(struct Config config, int userSock) {
    int serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock < 0) {
        return -1;
    }

    struct sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(config.serverPort);
    struct hostent *dstHost = gethostbyname(config.serverHost);
    remoteAddr.sin_addr.s_addr = *(in_addr_t *)dstHost->h_addr;

    if (retryConnect(serverSock, (struct sockaddr*)&remoteAddr, sizeof(remoteAddr)) < 0) {
        return -1;
    }

    if (fork() == 0) {
        forwardData(userSock, serverSock, 2);
        exit(0);
    }
    if (fork() == 0) {
        forwardData(serverSock, userSock, 1);
        exit(0);
    }

    return 0;
}

void clientLoop(struct Config config, int clientSock) {
    printf("client loop...\n");
    struct sockaddr_in userAddr;
    socklen_t clientAddrLength = sizeof(userAddr);

    while (1) {
        printf("wait for user...\n");
        int userSock = accept(clientSock, (struct sockaddr *)&userAddr, &clientAddrLength);
        if (userSock == -1) {
            continue;
        }
        if (fork() == 0) {
            close(clientSock);
            handleUserRequest(config, userSock);
            exit(0);
        }
        close(userSock);
    }
}

void startClient(struct Config config) {
    printf("start client...\n");
    int clientSock = createListeningSocket(config.localPort);
    if (clientSock < 0) {
        printf("Cannot create listening socket on port %d.\n", config.localPort);
        exit(clientSock);
    }

    clientLoop(config, clientSock);
}