#include <stdio.h>
#include "hash_table.h"

int main() {
    // Create a new hash table
    ht_hash_table* ht = ht_new();
    if (ht == NULL) {
        fprintf(stderr, "Failed to create hash table\n");
        return 1;
    }

    // Insert some key-value pairs
    ht_insert(ht, "apple", "red");
    ht_insert(ht, "banana", "yellow");
    ht_insert(ht, "grape", "purple");

    // Search for values
    char* color = ht_search(ht, "apple");
    if (color != NULL) {
        printf("The color of apple is %s\n", color);
    } else {
        printf("Apple not found\n");
    }

    color = ht_search(ht, "banana");
    if (color != NULL) {
        printf("The color of banana is %s\n", color);
    } else {
        printf("Banana not found\n");
    }

    color = ht_search(ht, "grape");
    if (color != NULL) {
        printf("The color of grape is %s\n", color);
    } else {
        printf("Grape not found\n");
    }

    // Delete a key-value pair
    ht_delete(ht, "apple");

    // Search for the deleted key
    color = ht_search(ht, "apple");
    if (color != NULL) {
        printf("The color of apple is %s\n", color);
    } else {
        printf("Apple not found\n");
    }
    return 0;
}
