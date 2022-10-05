#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>

int main()
{
    int socket_desc;
    socket_desc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // initialize socket
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    struct sockaddr_in addrport, client; // defined by c socket api
    addrport.sin_family = AF_INET;
    addrport.sin_port = htons(8888); // port number
    addrport.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(socket_desc, (struct sockaddr *)&addrport, sizeof(addrport)) < 0) // reserver the port for socket
    {
        puts("bind failed");
        return 1;
    }
    puts("bind done");
    int l_status = listen(socket_desc, 3); // listen for incoming connections, 3 is the maximum number of connections that can queue
    if (l_status < 0)
    {
        perror("listen failed");
        return 1;
    }
    int clen = sizeof(client);
    int s = accept(socket_desc, (struct sockaddr *)&client, &clen);
    char buff[1000];
    if(recv(s, buff, 1000, 0) < 0)
    {
        puts("recv failed");
        return 1;
    } 
    puts(buff);
    close(socket_desc);
    return 0;
}
