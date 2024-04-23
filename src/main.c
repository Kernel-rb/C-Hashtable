#include "hash_table.h"


int main(){
    hash_table* ht = new_hash_table();
    del_hash_table(ht);
    return 0;
}