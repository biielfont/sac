// Server code

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 1337

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serverAddr, newAddr;
    socklen_t addr_size;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("[-] Error in connection.\n");
        exit(1);
    }
    printf("[+] Server socket created.\n");

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("[-] Error in binding.\n");
        exit(1);
    }
    printf("[+] Binding successul.\n");

    if (listen(sockfd, 10) == 0) {
        printf("[+] Listening....\n");
    } else {
        perror("[-] Error in listening.\n");
        exit(1);
    }

    addr_size = sizeof(newAddr);
    newsockfd = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);

    // Remote command execution loop
    while (1) {
        printf("Enter command: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(newsockfd, buffer, strlen(buffer), 0);
        printf("[+] Command sent.\n");

        if (strcmp(buffer, "exit\n") == 0) {
            printf("[-] Server shutting down...\n");
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        recv(newsockfd, buffer, sizeof(buffer), 0);
        printf("[+] Output: %s\n", buffer);
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}