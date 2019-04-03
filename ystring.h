//
// Created by cayun on 2019-04-01.
//

#ifndef YYL_BRIDGE_YSTRING_H
#define YYL_BRIDGE_YSTRING_H

#include <stdbool.h>

typedef char* ystring;

ystring ystringNew(char *str);

ystring ystringNewN(char *str, int n);

void ystringFree(ystring str);

int ystringLength(ystring str);

char* ystringData(ystring str);

int ystringCmp(ystring a, ystring b);

bool ystringEmpty(ystring str);

void ystringDebug(ystring str);

#endif //YYL_BRIDGE_YSTRING_H
