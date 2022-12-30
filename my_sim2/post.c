#include <stdio.h>
#include <curl/curl.h>
#include <time.h>
#include <omp.h>
#include "db.c"

int main()
{
    char (*keys)[bufferLength] = NULL;
    char (*values)[bufferLength] = NULL;
    
    char *response = lookup(keys, values);
    
    struct json_object *parsed;
    parsed = json_tokener_parse(response);

    int len = json_object_array_length(parsed);

    struct json_object *obj;
    struct json_object *sid;

    char *urls[len];

    for(int i=0; i<len; i++)
    {
        obj = json_object_array_get_idx(parsed, i);
        json_object_object_get_ex(obj, "subscriber_url", &sid);

        char *url = json_object_get_string(sid);
        strcat(url, "/search");
        urls[i] = url;
    }

    #pragma omp parallel for
    for(int i=0; i<len; i++)
    {
        printf("\n\nThread: %d\n\n", omp_get_thread_num());
        for(int j=0; j<3; j++)
        {
            CURL *curl;
            CURLcode response;

            curl_global_init(CURL_GLOBAL_ALL);

            curl = curl_easy_init();
            if(curl)
            {
                curl_easy_setopt(curl, CURLOPT_URL, urls[i]);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl&lastname=Maxb");
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

                response = curl_easy_perform(curl);
                
                if(response!=CURLE_OK)
                {
                    fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(response));
                }
                else 
                {
                    break;
                }

                curl_easy_cleanup(curl);
            }

            curl_global_cleanup();
        }
    }

    return 0;
}