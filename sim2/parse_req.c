#include <stdio.h>
#include<stdlib.h>
#include<string.h>
// #include"combined_signature.c"

// #define pk (unsigned char*) "4jJ5@2adHܿ(O{T SSd7KW4jJ5@2adЯq"
// #define sk (unsigned char*) "eQKԪ?*G4HEvO49:NêL��Tl8X<P<P39B"
// void verifier(char* signature, char* expires, char* created, char* request_body)
// {
//     char *message= (char*) malloc(strlen(created)+strlen(expires)+strlen(request_body)+2);
//     strcpy(message,create_message(created,expires,request_body));
//     printf("verifying signature.....");
//     if (verify_request(signature,message,pk) != 0) 
//         printf("incorrect signature\n");
//     else
//     {
//         printf("signature verified\n");
//         printf("signing message.....");
//         unsigned char* signed_message = (unsigned char*) malloc(strlen(sign_request(message,sk)));
//         strcpy(signed_message,sign_request(message,sk));
//         printf("signed message: %s\n",signed_message);
//     }
// }

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
        key_value[0][j] = malloc(strlen(key));
        key_value[0][j] = key;
        key_value[1][j] = malloc(strlen(value));
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
    
    //verifier(key_value[1][5], key_value[1][3], key_value[1][2], body);
}
