#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>

int main()
{
    int socket_desc;
    socket_desc = socket(AF_INET, SOCK_STREAM, 0); // initialize socket
    
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    
    struct sockaddr_in addrport; // defined by c socket api
    
    addrport.sin_family = AF_INET;
    addrport.sin_port = htons(8000); // port number
    addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int c_status = connect(socket_desc, (struct sockaddr *)&addrport, sizeof(addrport)); // connect to the server
    
    sleep(50);

    if(c_status < 0)
    {
        perror("connect failed");
        return 1;
    }
    
    char* message = "Hello, World!\n";
    
    if( send(socket_desc , message , strlen(message) , 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
    
    close(socket_desc);
    return 0;
}
