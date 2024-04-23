#include <stdlib.h>
#include <string.h>
#include "hash_table.h"


static hash_table HT_DELETED_ITEM = {NULL , NULL};


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


static int hash(const char* s , const int a , const int m ){
    // s : string to hash
    // a : prime number
    // m : size of hash table
    long hash = 0 ;
    const int len_s = strlen(s);
    for(int i = 0 ; i < len_s ; i++){
        hash += (long)pow(a , len_s - (i+1)) * s[i];
        hash = hash % m;
    }

    return (int)hash;
}


static int hash_table_get_hash(const char* s , const int num_buckets , const int attempt){
    const int hash_a = hash(s , HT_PRIME_1 , num_buckets);
    const int hash_b = hash(s. , HT_PRIME_2 , num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
};


void insert_hash_table(hash_table* ht , const char* key , const char* value){
    hash_table_item item = hash_table_new_item(key , value);
    int index = hash_table_get_hash(item->key , ht->size , 0);
    hash_table_item cur_item = ht->items[index];
    int i = 1;
    while(cur_item != NULL && cur_item != &HT_DELETED_ITEM){
       index= hash_table_get_hash(item->key , ht->size , i);
       cur_item = ht->items[index];
        i++;
    }
    ht->items[index] = item;
    ht->count++;
};

void search_hash_table(hash_table* ht , const char* key){
    int index = hash_table_get_hash(key , ht->size , 0);
    hash_table_item item = ht->items[index];
    int i = 1;
    while(item != NULL){
        if(item != &HT_DELETED_ITEM){
            if(strcmp(item->key , key) == 0){
                return item->value;
            }
        }
        index = hash_table_get_hash(key , ht->size , i);
        item = ht->items[index];
        i++;
    }
    return NULL;
};


void delete_hash_table(hash_table* ht , const char* key){
    int index = hash_table_get_hash(key , ht->size , 0);
    hash_table_item item = ht->items[index];
    int i = 1;
    while(item !=NULL){
        if(item != &HT_DELETED_ITEM){
            if(strcmp(item->key , key) == 0){
                del_hash_table_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = hash_table_get_hash(key , ht->size , i);
        item = ht->items[index];
        i++;
    }
    ht->count--;
};

