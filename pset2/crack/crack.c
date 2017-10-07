/**
 * crack.c from pset2 
 * 
 * rewritten with pointers
 * and dynamic depth
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>

typedef struct level
{
    int lvl;
    int pos;
    struct level *next;
} level;

void clearList (level *index);
void unload();
void show_usage();
char compare(char *s, char *c);

level *root = NULL;
level *tmp = NULL;
char *p = NULL;

int main(int argc, char **argv)
{
    // how deep
    const char DEEP = 4;

    // hash can be only so much long
    const char HASH_LENGTH = 13;

    // all possible characters
    const char *aA = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // alphabet length
    const char aA_ = 52;
    
    // check if called with exactly one argument
    if (argc != 2)
    {
        show_usage();
        return(1);
    }
    
    char *h = argv[1];

    // check the hash length
    if (strlen(h) != HASH_LENGTH)
    {
        show_usage();
        return(1);
    }
    
    // read salt from hash
    char salt[2];
    salt[0] = h[0];
    salt[1] = h[1];
    
    // initialize the password holder
    p = calloc(sizeof(char), DEEP);
    
    // linked list of (all - 1) levels
    for (int i = (DEEP - 1); i > 0; i--)
    {
        tmp = malloc(sizeof(level));
        tmp -> lvl = i;
        tmp -> pos = -1;
        tmp -> next = root;
        root = tmp;
    }
    
    while (1)
    {
        for (int i = 0; i < aA_; i ++)
        {
            p[0] = aA[i];
            if (compare(h, crypt(p, salt)))
            {
                printf("%s\n",p);
                unload();
                return(0);
            }
        }
        tmp = root;
        tmp -> pos += 1;
        while (tmp -> pos >= aA_)
        {
            if (tmp -> lvl == (DEEP - 1))
            {
                printf("All variations tested - none matched!\n");
                unload();
                return(1);
            }
            tmp -> pos = 0;
            p[tmp -> lvl] = aA[tmp -> pos];
            tmp = tmp -> next;
            tmp -> pos += 1;
            p[tmp -> lvl] = aA[tmp -> pos];
        }
        p[tmp -> lvl] = aA[tmp -> pos];
    }
}

// clears memory
void unload()
{
    clearList(root);
    free(p);
}

// recursively clear the linked list
void clearList (level *index)
{
    if (index == NULL)
        return;
    clearList(index -> next);
    free(index);
}


// compares two hashes
char compare(char *s, char *c)
{
    // the first two letters are always the same (salt)
    for (int i = 2; i < 13; i++)
    {
        if (s[i] != c[i])
        {
            return 0;
        }
    }
    return 1;
}

// print usage
void show_usage()
{
    printf("Usage: ./crack hash\n");
}
