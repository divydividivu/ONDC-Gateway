#include <mongoc.h>
#include<bson.h>

void insert_document(mongoc_collection_t *collection,const char* data)
{
   bson_t *bson;
   bson_error_t error;
   bson = bson_new_from_json ((const uint8_t *)data, -1, &error);
   if (!mongoc_collection_insert_one (collection, bson, NULL, NULL, &error))
   {
      fprintf (stderr, "%s\n", error.message);
   }
   printf("Insertion Successfull !");

}

void  retrieve_document(mongoc_collection_t *collection,const char* transaction_id)
{
   bson_t *query;
   const bson_t *doc;
   mongoc_cursor_t *cursor;
   query = bson_new ();
   BSON_APPEND_UTF8 (query, "trans", "123");
   char *str;

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

int main (int argc, char *argv[])
{
   const char *uri_string = "mongodb://localhost:27017";
   mongoc_uri_t *uri;
   mongoc_client_t *client;
   mongoc_database_t *database;
   mongoc_collection_t *collection;
   bson_error_t error;

   /*
    * Required to initialize libmongoc's internals
    */
   mongoc_init ();

   /*
    * Safely create a MongoDB URI object from the given string
    */
   uri = mongoc_uri_new_with_error (uri_string, &error);
   if (!uri) {
      fprintf (stderr,
               "failed to parse URI: %s\n"
               "error message:       %s\n",
               uri_string,
               error.message);
      return EXIT_FAILURE;
   }

   /*
    * Create a new client instance
    */
   client = mongoc_client_new_from_uri (uri);
   if (!client) {
      return EXIT_FAILURE;
   }

   /*
    * Get a handle on the database "db_name" and collection "coll_name"
    */
   database = mongoc_client_get_database (client, "test");
   collection = mongoc_client_get_collection (client, "test", "mycoll");
   const char *json = "{\"trans\": \"123\"}";
   insert_document(collection,json);
   const char* trans="123";
   retrieve_document(collection,trans);

   /*
    * Release our handles and clean up libmongoc
    */
   mongoc_collection_destroy (collection);
   mongoc_database_destroy (database);
   mongoc_uri_destroy (uri);
   mongoc_client_destroy (client);
   mongoc_cleanup ();

   return EXIT_SUCCESS;
}