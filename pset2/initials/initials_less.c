#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // ask for user input
    string name = get_string();
    
    // print the first initial in upper case
    printf("%c",toupper(name[0]) );
    int len = strlen(name);
    
    // search the string for spaces
    for (int i=1; i<len; i++)
    {
        
        // delimiter (space) found
        if (name[i]==' ')
        {
            
            // print the char (in upper case) after the space
            printf("%c", toupper(name[i+1]) );
        }
    }
    
    // print newline at the end for better formatting
    printf("\n");
}