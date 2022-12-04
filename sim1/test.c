#include <stdio.h>
#include<stdlib.h>
#include<string.h>

void parse_auth_header(char* auth_header, char* key_value[2][6])
{
    char* pairs[6];
    char* pair = strtok(auth_header, ",");
    int i = 0;
    while (pair != NULL)
    {
        if(pair[0] == ' ')
            pair++;
        pairs[i] = pair;
        pair = strtok(NULL, ",");
        i++;
    }
    for (int j = 0; j < 6; j++)
    {
        
        char* key = strtok(pairs[j], "=");
        char* value = strtok(NULL, "");
        key_value[0][j] = key;
        key_value[1][j] = value;
        
       // printf("%s, %s\n", key, value);
    }

    
}

void parse(char* header)
{
    //printf("Hello World");
    int bufferLength = 255;
    char (*keys)[bufferLength] = NULL;
    char (*values)[bufferLength] = NULL;
    char *end = header;
    char * line = strtok_r(strdup(end), "\n", &end);
    int i = 0;
    printf("Printing the header key-value pairs:\n\n");
    while(line) 
    {
        //printf("%s\n", line);
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
    printf("\n\n");
    char* auth_header = NULL;   
    for (int j = 0; j < i; j++) {
        if(strcmp(keys[j], "authorization") == 0)
        {
            auth_header = values[j];
            break;
        }
    }

    char* key_value[2][6];
    parse_auth_header(auth_header, key_value);
    printf("Printing the authorization key-value pairs:\n\n");
    for (int j = 0; j < 6; j++) {
        printf("%s: %s\n", key_value[0][j], key_value[1][j]);
    }
    printf("\n\n");
}
