#include <mongoc.h>
#include <json-c/json.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int bufferLength =255;
char * lookup(char (*keys)[bufferLength],char (*values)[bufferLength])
{
    mongoc_client_t *client;
    // Replace the uri string with your MongoDB deployment's connection string.
    client = mongoc_client_new ("mongodb+srv://ondc:1234@cluster0.pjl2rxa.mongodb.net/?retryWrites=true&w=majority");

    mongoc_collection_t *collection = mongoc_client_get_collection (client, "ondcRegistry", "registry");
    
    mongoc_cursor_t *cursor;
    const bson_t *doc;
    bson_t *query;
    char *str;
    char *result=malloc(10000);
    strcpy(result,"[");
    //printf("%s\n",result);
    int i = 0;
    query = bson_new ();
    while(values[i]!=NULL)
    {
        
        BSON_APPEND_UTF8 (query, keys[i], values[i]);
        i++;
    }

    cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
    while (mongoc_cursor_next (cursor, &doc)) {
        str = bson_as_canonical_extended_json (doc, NULL);
        //printf ("%s\n", str);
        //result = malloc(strlen(result)+strlen(str)+1);
        strcat(result, str);
        strcat(result,",");
        
        i++;
        bson_free (str);
    }
    strcat(result,"]");
    bson_destroy (query);
    mongoc_cursor_destroy (cursor);
    return result;   
}