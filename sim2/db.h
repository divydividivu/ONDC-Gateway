#ifndef DB_H
#define DB_H

void insert_document(mongoc_collection_t *collection,const char* data);
void retrieve_document(mongoc_collection_t *collection);

#endif