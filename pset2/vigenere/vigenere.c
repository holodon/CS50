#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

char enc(char c, char key);
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
    string k = argv[1];
    int k_len = strlen(k);
    
    // check if k consist only of alphabeticals
    for (int i = 0; i < k_len; i++)
    {
        if (! isalpha(k[i]) )
        {
            show_usage();
        return(1);
        }
    }
    
    // ask for plaintext
    printf("plaintext:  ");
    string p = get_string();

    // calculate and print encrypted text
    printf("ciphertext: ");
    
    // this variable counts through the key
    int c = 0;
    for (int i = 0, l = strlen(p); i < l; i++)
    {

        // encrypt and print alphabetic characters
        if (isalpha(p[i]))
        {
            printf( "%c", enc(p[i],k[c]) );
            
            // cycle through the key
            c++;
            if (c == k_len)
            {
                c = 0;
            }
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

// this function encrypts single character c with a key k
char enc(char c, char k)
{
    // shift sets the distance to zero for the upper and the lower case letters
    char shift = 65;
    if (islower(c))
    {
        shift = 97;
    }
    
    // convert 'a' and 'A' to 0, 'b' and 'B' to 2 and so on
    k = toupper(k) - 65;
    
    // calculate and return result
    return ( ((c - shift + k) % 26) + shift  );
}

// print usage
void show_usage()
{
    printf("Usage: ./vigenere k\n");
}
