#include <omp.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <assert.h>

int main()
{
    int socket_desc;
    socket_desc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    
    struct sockaddr_in addrport, client; // defined by c socket api
    addrport.sin_family = AF_INET;
    addrport.sin_port = htons(8880); // port number
    addrport.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_desc, (struct sockaddr *)&addrport, sizeof(addrport)) < 0) // reserver the port for socket
    {
        puts("bind failed");
        return 1;
    }
    
    puts("bind done");
    int s;

    omp_set_num_threads(3);

    while(1)
    {
        #pragma parallel for
        for(int i=0; i<3; i++)
        {
            int l_status = listen(socket_desc, 3); // listen for incoming connections, 3 is the maximum number of connections that can queue
            
            if (l_status < 0)
            {
                perror("listen failed");
                return 1;
            }

            int clen = sizeof(client);
            s = accept(socket_desc, (struct sockaddr *)&client, &clen);

            char msg[1024];

            if(recv(s, msg, 1024, 0) < 0)
            {
                puts("recv failed");
                return 1;
            }

            printf("%d: %s:\n\n\n", i, msg);
        }
    }

    close(s);
    close(socket_desc);
    return 0;
}