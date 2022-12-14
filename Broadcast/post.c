#include <stdio.h>
#include <curl/curl.h>
#include <omp.h>

int main()
{
    char *urls[] = {"https://yoube.com", "https://aims.vnit.ac.in", "https://httpbin.org/post"};

    #pragma omp parallel for
    for(int i=0; i<3; i++)
    {
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