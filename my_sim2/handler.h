#ifndef __REQUEST_H__
#include <stdio.h>
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

void handle(int s);
void* thread_serve(void* arg);
void parse(char* header,char* body);
void parse_auth_header(char* auth_header, char* key_value[2][6]);

#endif 