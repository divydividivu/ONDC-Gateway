#include <stdio.h>
#include<string.h>
#include <stdlib.h>
int main()
{
    //printf("Hello World");

    char* header = "HTTP/1.1 200 OK\nauth: yes\0";

    int bufferLength = 255;
    char (*keys)[bufferLength] = NULL;
    char (*values)[bufferLength] = NULL;
    char *end = header;
    char * line = strtok_r(strdup(end), "\n", &end);
    int i = 0;
    while(line) 
    {
        printf("%s\n", line);
        if(strstr(line, ":"))
        {
            keys = realloc(keys, (i + 1) * sizeof keys[0]);
            strcpy(keys[i], strtok(line, ":"));
           
            values = realloc(values, (i + 1) * sizeof values[0]);
            strcpy(values[i], strtok(NULL, ""));
            printf("%s, %s\n", keys[i], values[i]);
            i++;
        }
        line  = strtok_r(NULL, "\n", &end);
    }  
}