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
    const it load = ht->count * 100 / ht->size;
    if(load > 70){
        hash_table_resize_up(ht);
    }
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
    const int load = ht->count * 100 / ht->size;
    if(load < 10){
        hash_table_resize_down(ht);
    }
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


static hash_table* hash_table_new_sized(const int base_size){
    hash_table* ht = xmalloc(sizeof(hash_table));
    ht->base_size = base_size;
    ht->size = next_prime(ht->base_size);
    ht->count = 0;
    ht->items = xcalloc((size_t)ht->size , sizeof(hash_table_item*));
    return ht;
};


hash_table* hash_table_new(){
    return hash_table_new_sized(HT_INITIAL_BASE_SIZE);
};


static void hash_table_resize(hash_table* ht , const int base_size){
    if(base_size < HT_INITIAL_BASE_SIZE){
        return;
    }
    hash_table* new_ht = hash_table_new_sized(base_size);
    for(int i = 0 ; i < ht->size ; i++){
        hash_table_item item = ht->items[i];
        if(item != NULL && item != &HT_DELETED_ITEM){
            insert_hash_table(new_ht , item->key , item->value);
        }
    }
    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    hash_table_item* tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    del_hash_table(new_ht);
};


static void hash_table_resize_up(hash_table* ht){
    const int new_size = ht->base_size * 2;
    hash_table_resize(ht , new_size);
};

static void hash_table_resize_down(hash_table* ht){
    const int new_size = ht->base_size / 2;
    hash_table_resize(ht , new_size);
};


