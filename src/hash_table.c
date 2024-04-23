#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hash_table.h"

#define HT_INITIAL_BASE_SIZE 10
#define HT_PRIME_1 151
#define HT_PRIME_2 163

// Function prototypes
static ht_item* ht_new_item(const char* k, const char* v);
static void ht_del_item(ht_item* i);
static int ht_hash(const char* s, const int a, const int m);
static int ht_get_hash(const char* s, const int num_buckets, const int attempt);
static void ht_resize(ht_hash_table* ht, const int base_size);
static void ht_resize_up(ht_hash_table* ht);
static void ht_resize_down(ht_hash_table* ht);
static ht_hash_table* ht_new_sized(const int base_size);
static int next_prime(int n);

// Constant placeholder for deleted items
static const ht_item HT_DELETED_ITEM = {NULL, NULL};

// Create a new item for the hash table
static ht_item* ht_new_item(const char* k, const char* v) {
    ht_item* i = malloc(sizeof(ht_item));
    if (i == NULL) {
        return NULL;
    }
    i->key = strdup(k);
    i->value = strdup(v);
    if (i->key == NULL || i->value == NULL) {
        free(i->key);
        free(i->value);
        free(i);
        return NULL;
    }
    return i;
}

// Delete an item from the hash table
static void ht_del_item(ht_item* i) {
    free(i->key);
    free(i->value);
    free(i);
}

// Generate a hash value for a string
static int ht_hash(const char* s, const int a, const int m) {
    long hash = 0;
    const int len_s = strlen(s);
    for (int i = 0; i < len_s; i++) {
        hash += (long)pow(a, len_s - (i+1)) * s[i];
        hash = hash % m;
    }
    return (int)hash;
}

// Get the hash value of a string
static int ht_get_hash(const char* s, const int num_buckets, const int attempt) {
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

// Create a new hash table with a specified base size
static ht_hash_table* ht_new_sized(const int base_size) {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    if (ht == NULL) {
        return NULL;
    }
    ht->base_size = base_size;
    ht->size = next_prime(ht->base_size);
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
    if (ht->items == NULL) {
        free(ht);
        return NULL;
    }
    return ht;
}

// Create a new hash table with initial size
ht_hash_table* ht_new() {
    return ht_new_sized(HT_INITIAL_BASE_SIZE);
}

// Insert a key-value pair into the hash table
void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    const int load = ht->count * 100 / ht->size;
    if (load > 70) {
        ht_resize_up(ht);
    }
    ht_item* item = ht_new_item(key, value);
    if (item == NULL) {
        return; // Allocation failed
    }
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item* cur_item = ht->items[index];
    int i = 1;
    while (cur_item != NULL) {
        if (cur_item != &HT_DELETED_ITEM && strcmp(cur_item->key, key) == 0) {
            ht_del_item(cur_item);
            ht->items[index] = item;
            return;
        }
        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    }
    ht->items[index] = item;
    ht->count++;
}

// Search for a value associated with a key in the hash table
char* ht_search(ht_hash_table* ht, const char* key) {
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM && strcmp(item->key, key) == 0) {
            return item->value;
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    return NULL;
}

// Delete a key-value pair from the hash table
void ht_delete(ht_hash_table* ht, const char* key) {
    const int load = ht->count * 100 / ht->size;
    if (load < 10) {
        ht_resize_down(ht);
    }
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM && strcmp(item->key, key) == 0) {
            ht_del_item(item);
            ht->items[index] = (ht_item*)&HT_DELETED_ITEM; // Cast away const qualifier
            ht->count--;
            return;
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
}

// Resize the hash table to a larger size
static void ht_resize_up(ht_hash_table* ht) {
    const int new_size = ht->base_size * 2;
    ht_resize(ht, new_size);
}

// Resize the hash table to a smaller size
static void ht_resize_down(ht_hash_table* ht) {
    const int new_size = ht->base_size / 2;
    ht_resize(ht, new_size);
}

// Resize the hash table to the specified size
static void ht_resize(ht_hash_table* ht, const int base_size) {
    if (base_size < HT_INITIAL_BASE_SIZE) {
        return;
    }
    ht_hash_table* new_ht = ht_new_sized(base_size);
    if (new_ht == NULL) {
        return; // Allocation failed
    }
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_insert(new_ht, item->key, item->value);
        }
    }
    // Free old items and swap contents
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_del_item(item);
        }
    }
    free(ht->items);
    ht->base_size = new_ht->base_size;
    ht->size = new_ht->size;
    ht->count = new_ht->count;
    ht->items = new_ht->items;
    // Free new_ht without freeing items
    new_ht->items = NULL;
    free(new_ht);
}

// Utility function to get the next prime number
static int next_prime(int n) {
    // Simple prime number generation, can be replaced with a more efficient algorithm
    for (int i = n + 1;; i++) {
        int is_prime = 1;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            return i;
        }
    }
}

// Free the memory allocated for the hash table
void ht_del_hash_table(ht_hash_table* ht) {
    if (ht == NULL) {
        return;
    }
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}
