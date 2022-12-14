#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curl/curl.h>
 
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

void get_response(char* url, const char* header_filename, const char* body_filename)
{
    CURL *curl_handle;
    FILE *headerfile;
    FILE *bodyfile;
    
    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
    
    headerfile = fopen(header_filename, "wb");
    if(!headerfile) {
        curl_easy_cleanup(curl_handle);
        return -1;
    }
    
    bodyfile = fopen(body_filename, "wb");
    if(!bodyfile) {
        curl_easy_cleanup(curl_handle);
        fclose(headerfile);
        return -1;
    }
    
    curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, headerfile);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, bodyfile);
    curl_easy_perform(curl_handle);
    fclose(headerfile);
    fclose(bodyfile);
    
    curl_easy_cleanup(curl_handle);
}

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
        char* value = strtok(NULL, "=");
        key_value[0][j] = key;
        key_value[1][j] = value;
        
       // printf("%s, %s\n", key, value);
    }
    
}

void parse_headerfile(char* fileName)
{
    FILE* file = fopen(fileName, "r");
    int bufferLength = 255;
    char line[bufferLength];
    char (*keys)[bufferLength] = NULL;
    char (*values)[bufferLength] = NULL;
    int i = 0;

    while (fgets(line, bufferLength, file)) {
        if(strstr(line, ":"))
        {
            keys = realloc(keys, (i + 1) * sizeof keys[0]);
            strcpy(keys[i], strtok(line, ":"));
           
            values = realloc(values, (i + 1) * sizeof values[0]);
            strcpy(values[i], strtok(NULL, ""));
            printf("%s: %s\n", keys[i], values[i]);
            i++;
        }
    }
    fclose(file);
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
    for (int j = 0; j < 6; j++) {
        //printf("%s: %s\n", key_value[0][j], key_value[1][j]);
    }
}


 
int main(void)
{
    //get_response("https://www.google.com", "head.out", "body.out");

    parse_headerfile("head.out");
    
    return 0;
}