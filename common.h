//
// Created by cayun on 2019-03-17.
//

#ifndef MZZ_BRIDGE_C_COMMON_H
#define MZZ_BRIDGE_C_COMMON_H

#include <stdlib.h>
#include <sys/socket.h>

typedef unsigned char Byte;

/**
 * 拷贝n个字节，不忽略'\0'
 * @param dst 目标串
 * @param src 源串
 * @param n 拷贝长度
 * @return 目标串的地址
 */
Byte* Byte_copyN(Byte *dst, const Byte *src, size_t n);

/**
 * 将Byte数组转换成字符串
 * @param src
 * @param n
 * @return
 */
char* Byte_arrayToStr(Byte *src, size_t n);

int	retryConnect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

ssize_t retryRecv(int sockfd, void *buf, size_t len);

ssize_t retrySend(int sockfd, const void *buf, size_t len);

/**
 * 创建监听socket
 * @param port
 * @return
 */
int createListeningSocket(int port);

/**
 * 传输数据
 * @param srcSock
 * @param dstSock
 * @param encryption 1.加密 0.解密
 */
void forwardData(int srcSock, int dstSock, int encryption);

#endif //MZZ_BRIDGE_C_COMMON_H
