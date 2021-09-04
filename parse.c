
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "hash.h"

#define WORD_SIZE 40
#define DICTIONARY_SIZE 1000

#define TRUE 1
#define FALSE 0

typedef struct dictionary_entry{
  //variable used for the words in the dictionary hash table
  char word[WORD_SIZE];
  //variable for counting the frequency of the words
  int freq;
}dictionary_entry;

void lower_case_word(char *);
void dump_dictionary(Phash_table dictionary);
void print_dictionary_entry(void *data);

//define the dictionary hash table
Phash_table dictionary;

int main(void)
{
  char c, word[WORD_SIZE];
  //initialize new dictionary
  dictionary = new_hash(DICTIONARY_SIZE, print_dictionary_entry);
  //initialize p
  dictionary_entry *p = find_hash(dictionary, word);

  int frequency[DICTIONARY_SIZE];
  int char_index = 0, dictionary_size = 0, num_words = 0, i;

  printf("Parsing input ...\n");
  /* Read in characters until end is reached */
  while ((c = getchar()) != EOF) {
    if ((c == ' ') || (c == ',') || (c == '.') || (c == '!') || (c == '"') ||
        (c == ':') || (c == '\n')) {
      /* End of a word */
      if (char_index) {
        /* Word is not empty */
        word[char_index] = '\0';
        lower_case_word(word);
        if (!(p = find_hash(dictionary, word))) {
          //allocate data for each entry p
          p = (dictionary_entry*)malloc(sizeof(dictionary_entry));
          //copy the word into the hash variable
          strcpy(p->word, word); 
          p->freq = 1;
          //insert the word into the dictionary hash table
          insert_hash(dictionary, word, p);
        }else{
          //increment the word's frequency if it already exists in the hash table
          p->freq++;
        }
        char_index = 0;
        num_words++;
      }
    } else {
      /* Continue assembling word */
      word[char_index++] = c;
    }
  }
  dump_dictionary(dictionary);
}

void print_dictionary_entry(void *data){
  
  //initialize p variable
  dictionary_entry *p;
  //set p equal to generic data pointer
  p = (dictionary_entry*)data;

  //loop to print word if the frequency is greater than 1
  if(p->freq > 1){
    printf("%s:  %d\n", p->word, p->freq);
    return;
  }
}

void lower_case_word(char *w)
{
  int i = 0;

  while (w[i] != '\0') {
    w[i] = tolower(w[i]);
    i++;
  }
}

void dump_dictionary(Phash_table dictionary)
{
  //declaration of loop variable and stat variables
  int i, num, worst_search;
  float avg_search;

  //calculate worst search
  stat_hash(dictionary, &num, &avg_search, &worst_search);
  printf("Number of unique words:  %d\n", num);
  printf("Average search:  %.2f\n", avg_search);
  printf("Worst-case search:  %d\n", worst_search);

  //print the dictionary
  dump_hash(dictionary);

}
