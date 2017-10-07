#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // ask for user input
    string name = get_string();
    
    // check if the input starts with a char and print it
    if (name[0] != ' ')
    {
        printf("%c",toupper(name[0]) );
        
    }
    int len = strlen(name);

    // search the string for spaces
    for (int i=0; i<len; i++)
    {
        
        // delimiter (space) found
        if (name[i]==' ')
        {
            int k = i+1;
            
            // print the first char (in upper case) if such exists after the space(s)
            if (k < len && name[k] != ' ')
            {
                printf("%c", toupper(name[i+1]) );
            }
        }
    }
    
    // print newline at the end for better formatting
    printf("\n");
}
