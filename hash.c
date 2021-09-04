/* Project #4
ENEE150: Spring 2021
Eugenia Srodowski
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "hash.h"

Phash_table new_hash(int size, void (*print_func)(void *)){
    
    //define hash table
    Phash_table new_table; 
    new_table = (Phash_table)malloc(sizeof(hash_table));
    new_table->size = size;
    new_table->table = (hash_entry**)malloc(size * sizeof(hash_entry*));
    
    //loop variable
    int i;
    //loop to make all the buckets NULL
	for (i = 0; i < size; i++){
        new_table->table[i] = NULL;
    }

    new_table->print_func = print_func;

    //initialize all stat variables to zero
    new_table->num = 0;
    new_table->worst_search = 0;
    new_table->num_search= 0; 
    new_table->total_search = 0;  
    
    return new_table;
} 
    
unsigned int hash(char *key, Phash_table table){
    
    //unsigned variable so there's no unexpected behavior
    unsigned int accum = 0;

    //variable to keep track of key's string length
    int keyLen = strlen(key);
    int i;
    //converts the string into its proper key
    for (i = 0; i < keyLen; i++)
    {
        accum += (key[i] * (1 << i));
    }
    
    //return the hash key to the function 
    return(accum % table->size);
}

void insert_hash(Phash_table table, char *key, void *data){

    //calculate the hash key
    int index = hash(key, table);

    //allocate data for the new hash_entry and key
    hash_entry *new_entry = (hash_entry*)malloc(sizeof(hash_entry));
    new_entry->data = data;
    new_entry->key = (char*)malloc(sizeof(char) * strlen(key)+1);
    strcpy(new_entry->key, key);
    
    //initialize pointer for head insertion
    hash_entry* cur_head = table->table[index];

    //complete head insertion
    new_entry->next = cur_head;
    table->table[index]= new_entry;

    //num is incremented for every time insert_hash is called
    table->num = table->num + 1;
}

void *find_hash(Phash_table table, char *key){

    //ptr for linked list traversal
    hash_entry* cur;
    //variables for loop traversal and hash-key calculation
    int i, index;
    //ptr for linked list
    hash_entry *data;
    
    //initialize index as key
    index = hash(key, table);
    //tracker variable for worst_search
    int trckr = 0;

    //loop to find specific hash value based on key
    for (cur = table->table[index], i = 0; cur; cur = cur->next) {    
        i++;    
        trckr++;
        table->total_search = table->total_search + 1;
        if (strcmp(cur->key, key) == 0) {
            break;    
        }
    }  
        //this loop is supposed to keep track of the larger "worst" search for
        //every find_hash call
        if(trckr > table->worst_search){
           table->worst_search = trckr;
        }

        //increment num_search
        table->num_search = table->num_search + 1;
        if (cur) {    
            return cur->data;  
        }
        else {
            return NULL;
        }

}

void stat_hash(Phash_table table, int *num, float *avg_search, int *worst_search){

    //set all variables aquired through function incrementations as equal to the function
    //pointer parameters
    *num = table->num;
    *avg_search = table->total_search/table->num_search;
    *worst_search = table->worst_search;
    //It's a really simple function all things considered.
    
}

void dump_hash(Phash_table table){

    //variable used to run through the hash table
   int i;  
   //pointer used to traverse the buckets
   hash_entry *cur;
   
   /*runs through the hash table and prints out the data*/
   for (i = 0; i < table->size; i++) {    
       for (cur = table->table[i]; cur; cur = cur->next){ 
           table->print_func(cur->data);
        }

}

}

void resize_hash(Phash_table table, int size){
    
    //variable to store old table in
    hash_entry *cur;
    //variable for looping through hash indeces
    int i;
    //swap variable to use later
    hash_table swap;
    //use new_hash to produce new hash table
    //allocate memory for the new table
    Phash_table new_table = new_hash(size, table->print_func);
    
    for(i = 0; i < table->size; i++) {
        for(cur = table->table[i]; cur != 0; cur = cur->next) {
            insert_hash(new_table, cur->key, cur->data);
        }
     }
    
    //swap the old and new table variables and free the data
    swap = *table;
    *table = *new_table;
    *new_table = swap;
    free_hash(new_table);
}   
    
    
void free_hash(Phash_table table){
    
    //Might actually all be right :)
    // driver seems to respond well to it...

    //definition of cur and next
    hash_entry *cur;
    hash_entry *next;
    //initialize loop variable
    int i;

    //loop that frees the various parts of the hash table's data
    for(i = 0; i < table->size; i++) {
        for(cur = table->table[i]; cur != 0; cur = next) {
            next = cur->next;
            free(cur);
        }     
    }
    
    //free the table header and table last
    free(table->table);
    free(table);

}