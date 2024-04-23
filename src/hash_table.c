#include <stdlib.h>
#include <string.h>
#include "hash_table.h"


static hash_table_item hash_table_new_item(const char* k , const char* v){
    hash_table_item i = malloc(sizeof(hash_table_item));
    i->key = strdup(k); 
    i->value = strdup(v);
    return i;
};


hash_table* new_hash_table(){
    hash_table* ht = malloc(sizeof(hash_table));
    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(hash_table_item*));
    return ht;
};


static void del_hash_table_item(hash_table_item i){
    free(i->key);
    free(i->value);
    free(i);
};


void del_hash_table(hash_table* ht){
    for(int i = 0; i < ht->size; i++){
        hash_table_item item = ht->items[i];
        if(item != NULL){
            del_hash_table_item(item);
        }
    }
    free(ht->items);
    free(ht);
};