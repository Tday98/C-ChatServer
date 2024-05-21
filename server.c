#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address = {
        AF_INET,
        htons(9999), // Port address
        0 // IPv4 address (0 is localhost)
    };

    bind(sockfd, &address, sizeof(address));

    listen(sockfd, 10);

    int clientfd = accept(sockfd, 0, 0);

    // stdin - 0
    struct pollfd fds[2] = {
        {
            0,
            POLLIN,
            0
        },
        {
            clientfd,
            POLLIN,
            0
        }
    };

    int millisec = 50000;

    for (;;) {
        char buffer[256] = { 0 };

        poll(fds, 2, millisec);

        if (fds[0].revents & POLLIN) {
            read(0, buffer, 255);
            send(clientfd, buffer, 255, 0);
        } else if (fds[1].revents & POLLIN) {
            if (recv(clientfd, buffer, 255, 0) == 0) {
                return 0;
            }
            
            printf("%s\n", buffer);
        }
    }
    return 0;
}