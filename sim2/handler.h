#ifndef __REQUEST_H__
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include <curl/curl.h>

#define MAXBUF (1024)
#define DEFAULT_BUFFER_SIZE 64
#define DEFAULT_THREADS 4

extern int buffer_max_size;
extern int buffer_size;
extern int num_threads;

void handle(int s);
void* thread_serve(void* arg);
void parse(char* header);
void parse_auth_header(char* auth_header, char* key_value[2][6]);
int replay_check(char* body);
void send_ack(char*body, int client_fd, int is_ack);

#endif 