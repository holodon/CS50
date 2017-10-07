/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include <stdint.h>

// trie struct
typedef struct node
{
    bool is_word;
    struct node *children[27];
} node;

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word);

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

/**
 * djb2 hash algorithm by dan bernstein:
 * http://www.cse.yorku.ca/~oz/hash.html
 * 
 * with modified return based on this post:
 * http://stackoverflow.com/a/14409947/6049386
 */
uint32_t hash(const char *str);

#endif // DICTIONARY_H
