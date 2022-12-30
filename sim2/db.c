#include <mongoc.h>
#include <json-c/json.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bson.h>
#include "db.h"

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

void insert_document(mongoc_collection_t *collection,const char* data)
{
   bson_t *bson;
   bson_error_t error;
   bson = bson_new_from_json (data, -1, &error);
   if(!bson)
   {
      fprintf (stderr, "%s\n", error.message);
   }
   if (!mongoc_collection_insert_one (collection, bson, NULL, NULL, &error))
   {
      fprintf (stderr, "%s\n", error.message);
   }
   printf("Insertion Successfull !");

}

void retrieve_document(mongoc_collection_t *collection)
{
    mongoc_cursor_t *cursor;
    const bson_t *doc;
    bson_t *query;
    const char* str;
    query = bson_new ();
    BSON_APPEND_UTF8 (query, "context.message_id", "string");
    cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
    while (mongoc_cursor_next (cursor, &doc))
    {
      str = bson_as_canonical_extended_json (doc, NULL);
      printf ("%s\n", str);
      bson_free (str);
    }
    bson_destroy (query);
    mongoc_cursor_destroy (cursor);

}