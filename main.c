#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "config.h"
#include "client.h"
#include "server.h"

int main(int argc, char *argv[]) {
    struct Config config;
    int opt;
    opterr = 0;
    while ((opt = getopt(argc, argv, "P:csh:p:")) != EOF) {
        switch (opt) {
            case 'P': config.localPort = atoi(optarg); break;
            case 'c': config.client = 1; break;
            case 's': config.server = 1; break;
            case 'h': config.serverHost = optarg; break;
            case 'p': config.serverPort = atoi(optarg); break;
        }
    }
    signal(SIGCHLD, SIG_IGN);
    if (config.client) {
        startClient(config);
    } else if (config.server) {
        startServer(config);
    }
    return 0;
}