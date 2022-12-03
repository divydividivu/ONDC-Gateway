#include "handler.h"
#include<stdlib.h>
#include<json-c/json.h>

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

void dequeue() 
{
    print_msg(queue[head]);
    printf("Message: %s is removed from the buffer.\n", queue[head].buf);
    buffer_size--;
    if(head == tail) // if queue found empty 
    { 
        head = -1; 
        tail = -1; 
    } 
    else
        head = (head + 1) % buffer_max_size; 
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

void handle(int s1, int s2)
{ 
    char msg[MAXBUF];

    if(recv(s1, msg, MAXBUF, 0) < 0)
    {
        puts("recv failed");
        return 1;
    }

    struct json_object *parsed;
    parsed = json_tokener_parse(msg);
    
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

    printf("\n\n\n");

    if(send(s2, msg, strlen(msg) , 0) < 0)
	{
		puts("Send failed");
		return 1;
	}

    printf("%d\n", s1);
    pthread_mutex_lock(&lock); // lock the critical section
    
    while(buffer_size > buffer_max_size)                                          
        pthread_cond_wait(&decreased, &lock); // wait till there is space in buffer
    
    assert(buffer_size <= buffer_max_size);
    msg_t t;
    t.len = strlen(msg);
    strcpy(t.buf, msg);

    buffer_size++;
    enqueue(t);                                                  
    
    printf("Message %s is added to the buffer.\n", t.buf);

    pthread_cond_signal(&increased); // signal that tasks in buffer have increased 
    pthread_mutex_unlock(&lock);

    return t.buf;
}