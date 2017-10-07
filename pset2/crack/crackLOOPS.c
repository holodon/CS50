#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

#define _XOPEN_SOURCE
#include <unistd.h>
#include <crypt.h>

void show_usage();
bool compare(string, char[]);

int main(int argc, string argv[])
{
    // all possible characters
    char aA[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // alphabet length
    char aA_ = 52;
    
    // check if called with exactly one argument
    if (argc != 2)
    {
        show_usage();
        return(1);
    }
    
    string h = argv[1];

    // check the hash length
    if (strlen(h) != 13)
    {
        show_usage();
        return(1);
    }
    
    // read salt from hash
    char salt[2];
    salt[0] = h[0];
    salt[1] = h[1];
    
    // pass buffer
    char p[] = "aaaa";
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;

    // depth 1..3 counters
    int i_ = -1;
    int i__ = -1;
    int i___ = -1;
    
    while (true)
    {
        
        // depth 0
        for (int i = 0; i < aA_; i++)
        {
           p[0] = aA[i];
            if (compare(h, crypt(p, salt)))
            {
                printf("%s\n",p);
                return(0);
            }
        }
        
        // depth 1
        if ( (i_ + 1) < aA_ )
        {
            i_++;
            p[1] = aA[i_]; 
        }
        else
        {
            i_ = 0;
            p[1] = aA[i_]; 

            // depth 2
            if ( (i__ + 1) < aA_ )
            {
                i__++;
                p[2] = aA[i__]; 
            }
            else
            {
                i__ = 0;
                p[2] = aA[i__]; 

                // depth 3
                if ( (i___ + 1) < aA_ )
                {
                    i___++;
                    p[3] = aA[i___];
                }
                else
                {
                    printf("All variations tested - none matched!\n");
                    return(1);
                }
            }
        }
                
    }

}

// compares two hashes
bool compare(string s, char c[])
{
    // the first two letters are always the same (salt)
    for (int i = 2; i < 13; i++)
    {
        if (s[i] != c[i])
        {
            return false;
        }
    }
    return true;
}

// print usage
void show_usage()
{
    printf("Usage: ./crack hash\n");
}
