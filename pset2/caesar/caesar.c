#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char enc(char c, int key);
void show_usage();

int main(int argc, string argv[])
{
    // check if called with exactly one argument
    if (argc != 2)
    {
        show_usage();
        return(1);
    }
    
    // get the key from the first argument
    int k = atoi(argv[1]);
    
    // check if k is non-negative integer
    if (k < 0)
    {
        show_usage();
        return(1);
    }
    
    // ask for plaintext
    printf("plaintext:  ");
    string p = get_string();

    // calculate and print encrypted text
    printf("ciphertext: ");
    for (int i = 0, l = strlen(p); i < l; i++)
    {

        // encrypt and print alphabetic characters
        if (isalpha(p[i]))
        {
            
            printf( "%c", enc(p[i],k) );
        }
        
        // just print everything else
        else
        {
            printf( "%c", p[i]);
        }
    }

    // print newline for bettter formatting
    printf("\n");
}

// this function encrypts single character c with a key k and returns the encrypted char
char enc(char c, int k)
{
    // shift sets the distance to zero for the upper and the lower case letters
    char shift = 65;
    if (islower(c))
    {
        shift = 97;
    }
    return ( ((c - shift + k) % 26) + shift  );
}

// print usage
void show_usage()
{
    printf("Usage: ./caesar k\n");
}
