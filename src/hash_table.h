typedef struct {
    char* key;
    char* value;
} hash_table_item;


typedef struct {
    int size;
    int count;
    hash_table_item** items;
} hash_table;


void insert_hash_table(hash_table* ht , const char* key , const char* value);
char* search_hash_table(hash_table* ht , const char* key);
void delete_hash_table(hash_table* ht , const char* key);