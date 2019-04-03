//
// Created by cayun on 2019-04-01.
//

#ifndef YYL_BRIDGE_YNET_H
#define YYL_BRIDGE_YNET_H

#include "ystring.h"
#include "ysocket.h"

#define NORMAL_FORWARD 0
#define UNZIP_AFTER_RECV 1
#define ZIP_BEFORE_SEND 2

ystring ynetRecv(int sockfd, bool unzip);

bool ynetSend(int sockfd, ystring str, bool zip);

void ynetForward(int srcSockfd, int dstSockfd, int mark);

int ynetCreateListenSock(int port);

#endif //YYL_BRIDGE_YNET_H
