#include <mongoc.h>
#include <json-c/json.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "handler.h"

int buffer_max_size;
int num_threads;
int buffer_size;
int bufferLength = 255;

mongoc_collection_t *collection;

char* parse(char* body,char (*keys)[bufferLength],char (*values)[bufferLength])
{
    //strcpy(body,strtok(body,"data-raw '{"));
    char *end = body;
    char * line = strtok_r(strdup(end), "\n", &end);
    int i = 0;
    while(line) 
    {
        
        if(strstr(line, ":"))
        {
            keys = realloc(keys, (i + 1) * sizeof keys[0]);
            strcpy(keys[i], strtok(line, "\""));
            strcpy(keys[i], strtok(NULL, "\""));
            values = realloc(values, (i + 1) * sizeof values[0]);
            strcpy(values[i], strtok(NULL, "\""));

            strcpy(values[i], strtok(NULL,"\""));
            i++;
        }
        line  = strtok_r(NULL, "\n", &end);
    } 
    char *sellers = malloc(10000);
    strcpy(sellers,lookup(collection,keys,values));
    return sellers;
}

char * lookup(mongoc_collection_t *collection,char (*keys)[bufferLength],char (*values)[bufferLength])
{
    mongoc_cursor_t *cursor;
    const bson_t *doc;
    bson_t *query;
    char *str;
    char *result=malloc(10000);
    strcpy(result,"[");
    printf("%s\n",result);
    int i = 0;
    query = bson_new ();
    while(values[i]!=NULL)
    {
        
        BSON_APPEND_UTF8 (query, keys[i], values[i]);
        i++;
    }
    cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
    
    while (mongoc_cursor_next (cursor, &doc)) {
        str = bson_as_canonical_extended_json (doc, NULL);
        //printf ("%s\n", str);
        //result = malloc(strlen(result)+strlen(str)+1);
        strcat(result, str);
        strcat(result,",");
        
        i++;
        bson_free (str);
    }
    strcat(result,"]");
    bson_destroy (query);
    mongoc_cursor_destroy (cursor);
    return result;   
}

int main (int argc, char *argv[])
{
    mongoc_client_t *client;
    
    mongoc_cursor_t *cursor;
    const bson_t *doc;
    // Replace the uri string with your MongoDB deployment's connection string.
    client = mongoc_client_new ("mongodb+srv://ondc:1234@cluster0.pjl2rxa.mongodb.net/?retryWrites=true&w=majority");

    collection = mongoc_client_get_collection (client, "ondcRegistry", "registry");
    num_threads = DEFAULT_THREADS;
    buffer_max_size = DEFAULT_BUFFER_SIZE;
    pthread_t thread_pool[num_threads];
    void* response;
    for(int i=0; i<num_threads; i++)
    	pthread_create(&thread_pool[i], NULL, thread_serve, NULL);
    
    buffer_size = 0;
    int socket_desc;
    socket_desc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // initialize socket
    
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    
    struct sockaddr_in addrport, client1; // defined by c socket api
    addrport.sin_family = AF_INET;
    addrport.sin_port = htons(8080); // port number
    addrport.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_desc, (struct sockaddr *)&addrport, sizeof(addrport)) < 0) // reserver the port for socket
    {
        puts("bind failed");
        return 1;
    }
    
    puts("bind done");
    int s;
    
    while(1)
    {
        int l_status = listen(socket_desc, 3); // listen for incoming connections, 3 is the maximum number of connections that can queue
        if (l_status < 0)
        {
            perror("listen failed");
            return 1;
        }
        int clen = sizeof(client1);
        s = accept(socket_desc, (struct sockaddr *)&client1, &clen);
        //send(s,"hello",strlen("hello"),0);


        //printf("%d -----\n",socket_desc);
        handle(s);
        
        //send(socket_desc,"hello",strlen("hello"),0);
    }


    
    
    mongoc_collection_destroy (collection);
    mongoc_client_destroy (client);

    mongoc_cleanup ();
    //close(s);
    close(socket_desc);
    return 0;
}







