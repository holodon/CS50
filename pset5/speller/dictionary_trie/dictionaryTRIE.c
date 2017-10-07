/**
 * Implements a dictionary's functionality.
 * 
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "dictionary.h"

uint8_t shift(const char c);
void clear_trie(node *n);

// pointer to the start of the trie
node *root;

// word count
uint32_t words = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // char buffer
    uint8_t c;
    
    // pointer buffer
    node *tmp = root;

    // itterate through the whole word
    while (( c = tolower(*word++) ))
    {
        c  = shift(c);
        if (tmp -> children[c] == NULL)
            return false;
        tmp = tmp -> children[c];
    }
    
    // all chars found - now check the boolean
    if (tmp -> is_word)
        return true;
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 * 
 */
bool load(const char *dictionary)
{
    
    // create the first element of the trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        fprintf(stderr, "Not enough memory.\n");
        return false;
    }
    
    // initialize its fields
    root -> is_word = false;
    for (uint8_t i = 0; i < 27; i++)
    {
        root -> children[i] = NULL;
    }

    // single word buffer
    char *w;
    w = malloc( (LENGTH + 1)* sizeof(char));    // LENGTH + '\0'
    if (w == NULL)
    {
        free(root);
        fprintf(stderr, "Not enough memory.\n");
        return false;
    }
    
    // open dictionary file 
    FILE *dfile = fopen(dictionary, "r");
    if (dfile == NULL)
    {
        free(w);
        free(root);
        return false;
    }
    
    // temporary variables
    node *tmp, *tmp2;
    
    // read the dictionary word by word and save each word's hash in the table
    while ( fscanf(dfile, "%s\n", w) != EOF )
    {
        // count the words
        words++;
        
        // buffer variables
        uint8_t len = strlen(w);
        uint8_t c;
        tmp = root;
        
        // create a trie path for the current word
        for (uint8_t i = 0; i < len; i++)
        {
            c = shift(w[i]);

            // position doesnt exists - create it
            if (tmp -> children[c] == NULL)
            {

                // create new node
                tmp2 = malloc(sizeof(node));
                if (tmp2 == NULL)
                {
                    unload();
                    free(w);
                    fprintf(stderr, "Not enough memory.\n");
                    return false;
                }
                
                // initialize its fields
                for (uint8_t j = 0; j < 27; j++)
                {
                    tmp2 -> children[j] = NULL;
                }
                tmp -> children[c] = tmp2;
            }
            
            // point to the next child
            tmp = tmp -> children[c];
        }
        
        // set the boolean flag at the end
        tmp -> is_word = true;
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
    return words;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    clear_trie(root);
    return true;
}

/**
 * Recursively clear the trie
 */
void clear_trie(node *n)
{
    for (uint8_t i = 0; i < 27; i++)
    {
        if (n -> children[i] != NULL)
        {
            clear_trie(n -> children[i]);
        }
    }
    free(n);
}

/**
 * This aligns the char to array indexes, using 0 for \', 1 for 'a' etc. to 26 for 'z'
 * case-insensitive
 */
uint8_t shift(const char c)
{
    if (c == '\'')
        return 0;
    else
        return (c - 96);
}
