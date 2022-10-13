#ifndef __REQUEST_H__
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>

#define MAXBUF (1024)
#define DEFAULT_BUFFER_SIZE 64
#define DEFAULT_THREADS 4

int buffer_max_size;
int buffer_size;
int num_threads;

void handle(char* msg);
void* thread_serve(void* arg);

#endif 