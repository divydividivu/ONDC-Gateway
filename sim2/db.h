#ifndef DB_H
#define DB_H

#include <mongoc.h>

int bufferLength =255;
char * lookup(char (*keys)[bufferLength],char (*values)[bufferLength]);
void insert_document(mongoc_collection_t *collection,const char* data);
void retrieve_document(mongoc_collection_t *collection);

#endif