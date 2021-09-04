
/*
 * hash.h
 * 

 Interfaces for hash table ADT

 *
 */

typedef struct hash_entry{

    //generic pointer for data storage
    void* data;
    //pointer to next for linked list buckets
    struct hash_entry *next;
    //key variable for the hash table
    char* key;

}hash_entry;

typedef struct hash_table_ {
  
  //size of the hash table
  int size;  
  //pointer to the linked-list heads
  hash_entry **table;
  //pointer to user-defined printfunc
  void (*print_func)(void *);

  int num; //total number of data records
  int worst_search; //number of least-efficient searches

  float num_search; //no times you call find_hash 
  float total_search; //number of iterations in the key comparisons

} hash_table, *Phash_table;

Phash_table new_hash(int size, void (*print_func)(void *));
void free_hash(Phash_table table);
void insert_hash(Phash_table table, char *key, void *data);
void *find_hash(Phash_table table, char *key);
void stat_hash(Phash_table table, int *num, float *avg_search, int *worst_search);
void dump_hash(Phash_table table);
void resize_hash(Phash_table table, int size);
