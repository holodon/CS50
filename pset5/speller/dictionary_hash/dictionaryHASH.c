/**
 * Implements a dictionary's functionality.
 * 
 */

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include "dictionary.h"

void clearList (node *index);
uint32_t hash(const char *str);
uint32_t hash_check(const char *str);

// size of the hash table
//#define HASH_SIZE 4096  // 2^12
#define HASH_SIZE 262144  // 2^18

// pointer to an array of pointers to "node
node** ht = NULL;

// word count
uint32_t words = 0;

// collisions counter
//uint32_t collisions = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    
    // hash buffer
    const uint32_t i = hash_check(word);

    // check if entry even exists
    if (ht[i] == NULL)
        return false;

    // temporary pointer
    node *tmp = ht[i];
    
    // keep searching for match in the linked list
    do
    {
        if ( strcasecmp( (tmp -> c), word) == 0 )
            return true;
        tmp = tmp -> next;
        
    } while (tmp);

    // no match found
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 * 
 * Populates a hash table (of HASH_SIZE) as array of *node pointers 
 * passing each word from the dictionary through the "hash" function
 * Collisions get inserted as linked list
 */
bool load(const char *dictionary)
{
    node *tmp = NULL;

    // the hash table - is a pointer to an array of node* pointers
    //      all elements initiated to NULL (calloc)
    ht = (node**) malloc( sizeof(node*) * HASH_SIZE );
    if (ht == NULL)
    {
        fprintf(stderr, "Not enough memory.\n");
        return false;
    }
    
    // initialize memory to NULL
    for (uint32_t i = 0; i < HASH_SIZE; i++)
        ht[i] = NULL;

    // single word buffer
    char *w;
    w = malloc( (LENGTH + 1)* sizeof(char));    // LENGTH + '\0'
    if (w == NULL)
    {
        free(ht);
        fprintf(stderr, "Not enough memory.\n");
        return false;
    }
    
    // open dictionary file 
    FILE *dfile = fopen(dictionary, "r");
    if (dfile == NULL)
    {
        free(w);
        free(ht);
        return false;
    }
    
    // current index in the hash table
    uint32_t index = 0;
    
    // hash buffer
    uint32_t h;
    
    // read the dictionary word by word and save each word's hash in the table
    while ( fscanf(dfile, "%s\n", w) > 0 )
    {
        words++;
        h = hash(w);

        // first element at the position
        if (ht[h] == NULL)
        {
            // create a node
            ht[h] = malloc(sizeof(node));
            if (ht[h] == NULL)
            {
                free(w);
                free(ht);
                unload();
                fclose(dfile);
                fprintf(stderr, "Not enough memory.\n");
                return false;
            }

            // allocate memory for the current string
            ht[h] -> c = malloc(sizeof(char) * strlen(w) + 1);
            if (ht[h] -> c == NULL)
            {
                free(w);
                free(ht);
                unload();
                fclose(dfile);
                fprintf(stderr, "Not enough memory.\n");
                return false;
            }
            
            ht[h] -> next = NULL;
            
            // save the current string
            strcpy(ht[h] -> c, w);
        }

        // pointer exists (collision) - insert as linked list
        else
        {
            //collisions++;
            
            // get memory for a new node
            tmp = malloc(sizeof(node));
            if (tmp == NULL)
            {
                free(w);
                free(ht);
                unload();
                fclose(dfile);
                fprintf(stderr, "Not enough memory.\n");
                return false;
            }

            // get a memory for a string
            tmp -> c = malloc(sizeof(char) * strlen(w) + 1);
            if (tmp -> c == NULL)
            {
                free(w);
                free(ht);
                free(tmp);
                unload();
                fclose(dfile);
                fprintf(stderr, "Not enough memory.\n");
                return false;
            }
            
            tmp -> next = NULL;
            
            // save the current string
            strcpy(tmp -> c, w);

            // insert the new node
            tmp -> next = ht[h] -> next;
            ht[h] -> next = tmp;
            
        }

        // advance index
        index++;
    }
    free(w);
    fclose(dfile);
    return true;   
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
//    printf("\n\nCollisions: %i\n", collisions);
    return words;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{

    // free the linked lists
    for (uint32_t z = 0; z < HASH_SIZE; z++)
        if (ht[z] != NULL)
            clearList(ht[z]);

    // clear the table index
    free(ht);
    
    return true;
}

/**
 * Recursive delete for linked list
 */
void clearList (node *index)
{
    if (index == NULL)
        return;
    clearList(index -> next);
    free(index -> c);
    free(index);
}

/**
 * djb2 hash algorithm by dan bernstein:
 * http://www.cse.yorku.ca/~oz/hash.html
 * 
 * with modified "return" based on this post:
 * http://stackoverflow.com/a/14409947/6049386
 */
uint32_t hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return (hash & (HASH_SIZE - 1));
}

/**
 * case-ignoring version of "hash"
 */
uint32_t hash_check(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + tolower(c);

    return (hash & (HASH_SIZE - 1));
}
