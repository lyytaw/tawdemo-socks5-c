#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "config.h"
#include "client.h"
#include "server.h"

void sigchld_handler(int signal) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char *argv[]) {
    struct Config config = configNew(argc, argv);
    signal(SIGCHLD, sigchld_handler);
    if (config.client) {
        startClient(config);
    } else if (config.server) {
        startServer(config);
    }
    configFree(config);
    return 0;
}