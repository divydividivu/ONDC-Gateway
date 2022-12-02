#include "handler.h"

int buffer_max_size;
int num_threads;
int buffer_size;

int main()
{
    num_threads = DEFAULT_THREADS;
    buffer_max_size = DEFAULT_BUFFER_SIZE;
    pthread_t thread_pool[num_threads];
	
    for(int i=0; i<num_threads; i++)
    	pthread_create(&thread_pool[i], NULL, thread_serve, NULL);
    
    buffer_size = 0;
    int socket_desc;
    socket_desc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // initialize socket
    
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
    
    while(1)
    {
        int l_status = listen(socket_desc, 3); // listen for incoming connections, 3 is the maximum number of connections that can queue
        if (l_status < 0)
        {
            perror("listen failed");
            return 1;
        }

        int clen = sizeof(client);
        int s1 = accept(socket_desc, (struct sockaddr *)&client, &clen);
        int s2 = accept(socket_desc, (struct sockaddr *)&client, &clen);
        
        handle(s1, s2);
        handle(s2, s1);
    }

    close(socket_desc);
    return 0;
}
