#include "handler.h"
#include<stdlib.h>
#include<json-c/json.h>
#include<string.h>
#include <mongoc.h>
#include<bson.h>


pthread_mutex_t lock;
pthread_cond_t increased;
pthread_cond_t decreased;

typedef struct message{
    char buf[MAXBUF];
    int len;
} msg_t;

int head = -1;
int tail = -1;
msg_t queue[100];

void parse_json(char* body)
{
    struct json_object *parsed;
    parsed = json_tokener_parse(body);

    struct json_object *context;
    json_object_object_get_ex(parsed, "context", &context);

    struct json_object *domain;
    struct json_object *country;
    struct json_object *city;
    struct json_object *action;
    struct json_object *core_version;
    struct json_object *bap_id;
    struct json_object *bap_uri;
    struct json_object *bpp_id;
    struct json_object *bpp_uri;
    struct json_object *transaction_id;
    struct json_object *message_id;
    struct json_object *timestamp;
    struct json_object *key;
    struct json_object *ttl;
    
    json_object_object_get_ex(context, "domain", &domain);
    json_object_object_get_ex(context, "country", &country);
    json_object_object_get_ex(context, "city", &city);
    json_object_object_get_ex(context, "action", &action);
    json_object_object_get_ex(context, "core_version", &core_version);
    json_object_object_get_ex(context, "bap_id", &bap_id);
    json_object_object_get_ex(context, "bap_uri", &bap_uri);
    json_object_object_get_ex(context, "bpp_id", &bpp_id);
    json_object_object_get_ex(context, "bap_uri", &bpp_uri);
    json_object_object_get_ex(context, "transaction_id", &transaction_id);
    json_object_object_get_ex(context, "message_id", &message_id);
    json_object_object_get_ex(context, "timestamp", &timestamp);
    json_object_object_get_ex(context, "key", &key);
    json_object_object_get_ex(context, "ttl", &ttl);

    printf("Domain: %s\n", json_object_get_string(domain));
    printf("Country: %s\n", json_object_get_string(country));
    printf("City: %s\n", json_object_get_string(city));
    printf("Action: %s\n", json_object_get_string(action));
    printf("Core Version: %s\n", json_object_get_string(core_version));
    printf("BAP ID: %s\n", json_object_get_string(bap_id));
    printf("BAP URI: %s\n", json_object_get_string(bap_uri));
    printf("BPP ID: %s\n", json_object_get_string(bpp_id));
    printf("BPP URI: %s\n", json_object_get_string(bpp_uri));
    printf("Transaction ID: %s\n", json_object_get_string(transaction_id));
    printf("Message ID: %s\n", json_object_get_string(message_id));
    printf("Timestamp: %s\n", json_object_get_string(timestamp));
    printf("Key: %s\n", json_object_get_string(key));
    printf("TTL: %s\n", json_object_get_string(ttl));

    printf("\n");
}

void print_msg(msg_t msg){
    printf("Message: %s\n", msg.buf);
}


void enqueue(msg_t t)                    //add into queue followin SFF
{
    int temp;
    
    if(head == -1) // if queue is empty simply add
    { 
        head = tail = 0; 
        queue[tail].len = t.len;
        strcpy(queue[tail].buf, t.buf); 
    } 
    else
    {
        temp = tail;
        tail = (tail + 1) % buffer_max_size;
        if(queue[temp].len < t.len) // if file to be added already larger than tail file simply add it at the end
        {
            strcpy(queue[tail].buf, t.buf); 
            queue[tail].len = t.len;
            return;
        }

        while(temp >= 0 && queue[temp].len > t.len) // else move files one step towards tail till right spot found
        {
            queue[(temp + 1) % buffer_max_size].len = queue[temp].len;
            strcpy(queue[(temp + 1) % buffer_max_size].buf, queue[temp].buf);
            temp--;
        }

        if(temp == -1) // if file is smallest in the queue
        {
            queue[0].len = t.len;
            strcpy(queue[0].buf, t.buf); 
        }
        else // larger than any files before temp so add after temp
        {
            queue[(temp + 1) % buffer_max_size].len = t.len;
            strcpy(queue[(temp + 1) % buffer_max_size].buf, t.buf); 
        }
    }
}

// void parse_auth_header(char* auth_header, char* key_value[2][6])
// {
//     char* pairs[6];
//     char* pair = strtok(auth_header, ",");
//     int i = 0;
//     while (pair != NULL)
//     {
//         if(pair[0] == ' ')
//             pair++;
//         pairs[i] = pair;
//         pair = strtok(NULL, ",");
//         i++;
//     }
//     for (int j = 0; j < 6; j++)
//     {
        
//         char* key = strtok(pairs[j], "=");
//         char* value = strtok(NULL, "=");
//         key_value[0][j] = key;
//         key_value[1][j] = value;
        
//        // printf("%s, %s\n", key, value);
//     }
    
// }

// void parse_headerfile(char* header)
// {
//     printf("Header: %s\n", header);
//     int bufferLength = 255;
//     char (*keys)[bufferLength] = NULL;
//     char (*values)[bufferLength] = NULL;
//     char *end = header;
//     char * line = strtok_r(strdup(end), "\n", &end);
//     int i = 0;
//     while(line) 
//     {
//         printf("%s\n", line);
//         if(strstr(line, ":"))
//         {
//             keys = realloc(keys, (i + 1) * sizeof keys[0]);
//             strcpy(keys[i], strtok(line, ":"));
           
//             values = realloc(values, (i + 1) * sizeof values[0]);
//             strcpy(values[i], strtok(NULL, ""));
//             printf("%s, %s\n", keys[i], values[i]);
//             i++;
//         }
//         line  = strtok_r(NULL, "\n", &end);
    
//         printf("%s\n", line);
//     }  

//     printf("outside");
//     char* auth_header = NULL;   
//     for (int j = 0; j < i; j++) {
//         if(strcmp(keys[j], "authorization") == 0)
//         {
//             auth_header = values[j];
//             break;
//         }
//     }

//     char* key_value[2][6];
//     parse_auth_header(auth_header, key_value);
//     for (int j = 0; j < 6; j++) {
//         //printf("%s: %s\n", key_value[0][j], key_value[1][j]);
//     }
// }

void insert_document(mongoc_collection_t *collection,const char* data)
{
   bson_t *bson;
   bson_error_t error;
   bson = bson_new_from_json (data, -1, &error);
   if(!bson)
   {
      fprintf (stderr, "%s\n", error.message);
   }
   if (!mongoc_collection_insert_one (collection, bson, NULL, NULL, &error))
   {
      fprintf (stderr, "%s\n", error.message);
   }
   printf("Insertion Successfull !");

}

void retrieve_document(mongoc_collection_t *collection)
{
    mongoc_cursor_t *cursor;
    const bson_t *doc;
    bson_t *query;
    const char* str;
    query = bson_new ();
    BSON_APPEND_UTF8 (query, "context.message_id", "string");
    cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
    while (mongoc_cursor_next (cursor, &doc))
    {
      str = bson_as_canonical_extended_json (doc, NULL);
      printf ("%s\n", str);
      bson_free (str);
    }
    bson_destroy (query);
    mongoc_cursor_destroy (cursor);

}

void dequeue() 
{
    mongoc_client_t *client;
    mongoc_collection_t *collection;

    mongoc_init ();

    client =mongoc_client_new ("mongodb+srv://fyp:2ykYtCR6tQnLhbcj@cluster0.usqwuqs.mongodb.net/?retryWrites=true&w=majority");
    collection = mongoc_client_get_collection (client, "mydb", "mycoll");

    //print_msg(queue[head]);
    char* buf = queue[head].buf;
    //buf[strlen(buf) - 1] = '\0';
    //printf("%s\n", buf);
    char* header;
    char* body = strstr(queue[head].buf, "\r\n\r\n");
    int length = body - buf + 1;
    header = (char*)malloc(length);
    strncpy(header, buf, length);
    //printf("HEAD: %s\n", header);
    // const char *json = "{\"name\": {\"first\":\"Grace\", \"last\":\"Hopper\"}}";
    parse(header,body);
    // retrieve_document(collection);
    insert_document(collection,body);
    parse_json(body);
    printf("Message is removed from the buffer.\n");
    buffer_size--;
    if(head == tail) // if queue found empty 
    { 
        head = -1; 
        tail = -1; 
    } 
    else
        head = (head + 1) % buffer_max_size; 
    /*
    * Release our handles and clean up libmongoc
    */
    mongoc_collection_destroy (collection);
    mongoc_client_destroy (client);
    mongoc_cleanup ();

}

void* thread_serve(void* arg) 
{
    while(1)
    {  // consumer code
        pthread_mutex_lock(&lock);      // lock the critical section                                        
        while(buffer_size == 0)
            pthread_cond_wait(&increased, &lock); // wait till there is something to execute
        
        assert(buffer_size > 0);
        dequeue();                         // work on the request
        pthread_cond_signal(&decreased); // signal that num of tasks in buff have decreased
        pthread_mutex_unlock(&lock);
    }
}

void handle(int s)
{ 
    char msg[MAXBUF];

    if(recv(s, msg, MAXBUF, 0) < 0)
    {
        puts("recv failed");
        return 1;
    }

    //char *ack = "HTTP/2 200 OK\r\n\r\nMessage Recieved\0";
    //TODO - send acknowledgement asynch

    printf("%d\n", s);
    pthread_mutex_lock(&lock); // lock the critical section

    while(buffer_size > buffer_max_size)                                          
        pthread_cond_wait(&decreased, &lock); // wait till there is space in buffer
    
    assert(buffer_size <= buffer_max_size);
    msg_t t;
    t.len = strlen(msg);
    strcpy(t.buf, msg);

    buffer_size++;
    enqueue(t);                                                  
    
    printf("Message is added to the buffer.\n");

    pthread_cond_signal(&increased); // signal that tasks in buffer have increased 
    pthread_mutex_unlock(&lock);

    return t.buf;
}