//
// Created by cayun on 2019-03-15.
//

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "config.h"

struct Config configNew(int argc, char *argv[]) {
    struct Config config;
    int opt;
    opterr = 0;
    while ((opt = getopt(argc, argv, "P:csh:p:")) != EOF) {
        switch (opt) {
            case 'P': config.localPort = atoi(optarg); break;
            case 'c': config.client = 1; break;
            case 's': config.server = 1; break;
            case 'h': config.serverHost = ystringNew(optarg); break;
            case 'p': config.serverPort = atoi(optarg); break;
        }
    }
    return config;
}

void configFree(struct Config config) {
    ystringFree(config.serverHost);
}