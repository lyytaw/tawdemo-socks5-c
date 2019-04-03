//
// Created by cayun on 2019-04-01.
//

#include <zlib.h>
#include <stdlib.h>
#include <stdio.h>
#include "yzip.h"

#define MAX_DATA_LENGTH 300000

char dstBuf[MAX_DATA_LENGTH];

ystring yzipCompress(ystring str) {
    if (ystringLength(str) > MAX_DATA_LENGTH) {
        printf("ERROR: compress string length > %d\n", MAX_DATA_LENGTH);
        return ystringNew("");
    }
    uLong dstLen = compressBound((uLong)ystringLength(str));
    compress((Byte *)dstBuf, &dstLen, (Byte *)ystringData(str), (uLong)ystringLength(str));
    ystring result = ystringNewN(dstBuf, (int)dstLen);
    return result;
}

ystring yzipUncompress(ystring str) {
    uLong dstLen = MAX_DATA_LENGTH;
    uncompress((Byte *)dstBuf, &dstLen, (Byte *)ystringData(str), (uLong)ystringLength(str));
    ystring result = ystringNewN(dstBuf, (int)dstLen);
    return result;
}
