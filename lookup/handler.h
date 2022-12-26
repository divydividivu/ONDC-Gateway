#ifndef __REQUEST_H__
#include <stdio.h>
#include <mongoc.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <assert.h>

#define MAXBUF (1024)
#define DEFAULT_BUFFER_SIZE 64
#define DEFAULT_THREADS 4

extern int buffer_max_size;
extern int buffer_size;
extern int num_threads;
extern int bufferLength;
extern mongoc_collection_t *collection;
void handle(int s);
void* thread_serve(void* arg);
char* parse(char* body,char (*keys)[bufferLength],char (*values)[bufferLength]);
void parse_auth_header(char* auth_header, char* key_value[2][6]);
char * lookup(mongoc_collection_t *collection,char (*keys)[bufferLength],char (*values)[bufferLength]);

#endif 