//
// Created by cayun on 2019-04-01.
//

#ifndef YYL_BRIDGE_YSOCKET_H
#define YYL_BRIDGE_YSOCKET_H

#include "ystring.h"

bool ysocketConnect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

ystring ysocketRecv(int sockfd, bool unzip);

bool ysocketSend(int sockfd, ystring str, bool zip);

void ysocketSetTimeout(int sockfd);

#endif //YYL_BRIDGE_YSOCKET_H
