#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <json-c/json.h>
#include <omp.h>
#include "/home/divy/FYP/sim2/replay_protect.c"
#include "/home/divy/FYP/sim2/ack.c"

#define PORT 9999
#define BUFFER_SIZE 1024

// TODO: try to use same send_ack from the ack.c in sim2 (reusability of code)


void error(char *msg)
{
    perror(msg);
    exit(1);
}

void handle_request(int client_socket)
{
    char buffer[BUFFER_SIZE];
    int num_bytes;
    num_bytes = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (num_bytes < 0)
        error("ERROR reading from socket");
    buffer[num_bytes] = '\0';
    printf("%s\n", buffer);

    char* body = strstr(buffer, "\r\n\r\n");
    int is_replay = replay_check(body);
    send_ack(body, client_socket, !is_replay);
    
    close(client_socket);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    #pragma omp parallel
    {
        while (1)
        {
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            printf("%d\n", newsockfd);
            if (newsockfd < 0) 
                error("ERROR on accept");

            #pragma omp critical
            {
                printf("here\n");
                handle_request(newsockfd);
            }
        }
    }

    close(sockfd);
    return 0; 
}
