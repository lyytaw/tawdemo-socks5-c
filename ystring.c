//
// Created by cayun on 2019-04-01.
//

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "ystring.h"

struct YstringInfo {
    int length;
};

struct YstringInfo ystringInfo(ystring str);

ystring ystringNew(char *str) {
    int len = (int)strlen(str);
    ystring result = (ystring)malloc(len + sizeof(struct YstringInfo) + 1);
    struct YstringInfo info;
    info.length = len;
    memcpy(result, &info, sizeof(struct YstringInfo));
    memcpy(result + sizeof(struct YstringInfo), str, len);
    result[len + sizeof(struct YstringInfo)] = 0;
    return result;
}

ystring ystringNewN(char *str, int n) {
    ystring result = (ystring)malloc(n + sizeof(struct YstringInfo) + 1);
    struct YstringInfo info;
    info.length = n;
    memcpy(result, &info, sizeof(struct YstringInfo));
    memcpy(result + sizeof(struct YstringInfo), str, n);
    result[n + sizeof(struct YstringInfo)] = 0;
    return result;
}

void ystringFree(ystring str) {
    free(str);
}

int ystringLength(ystring str) {
    return ystringInfo(str).length;
}

char* ystringData(ystring str) {
    return str + sizeof(struct YstringInfo);
}

int ystringCmp(ystring a, ystring b) {
    struct YstringInfo infoA = ystringInfo(a);
    struct YstringInfo infoB = ystringInfo(b);
    int cmpLen = infoA.length < infoB.length ? infoA.length : infoB.length;
    int cmpSameLen = strncmp(a + sizeof(struct YstringInfo), b + sizeof(struct YstringInfo), (size_t)cmpLen);
    return cmpSameLen == 0 ? infoA.length - infoB.length : cmpSameLen;
}

bool ystringEmpty(ystring str) {
    return ystringInfo(str).length == 0;
}

void ystringDebug(ystring str) {
    int n = ystringLength(str);
    char *data = ystringData(str);
    printf("n = %d, data:\n", n);
    for (int i = 0; i < n; i++) {
        printf("%x ", data[i]);
    }
    printf("\n");
}

struct YstringInfo ystringInfo(ystring str) {
    struct YstringInfo info;
    memcpy(&info, str, sizeof(struct YstringInfo));
    return info;
}
