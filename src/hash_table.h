typedef struct {
    char* key;
    char* value;
} hash_table_item;


typedef struct {
    int size;
    int count;
    hash_table_item** items;
} hash_table;