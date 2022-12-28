#include <mongoc.h>
#include<bson.h>
#include<stdio.h>


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

