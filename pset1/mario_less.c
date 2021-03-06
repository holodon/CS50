#include <stdio.h>
#include <cs50.h>

bool input_okay(int i);
void draw_pyramid(int i);
void draw_symbols (char c, int count);

int main(void)
{
    int i;
    do
    {
        printf("Height: ");
        i = get_int();
    }
    while (!input_okay(i));
    draw_pyramid(i);
}

bool input_okay(int i)
{
    if ( (i<0) || (i>23) ) {
        return false;
    }
    return true;
}

void draw_pyramid(int i)
{
    int rows    = i;
    int cells   = i +1;
    int hashes  = 2;
    for (int n = 0; n < rows; n++) 
    {
        draw_symbols(' ',cells - hashes);
        draw_symbols('#',hashes);
        printf("\n");
        hashes++;
    }
}

void draw_symbols (char c, int count)
{
    for (int i = 0; i < count; i ++)
    {
        printf("%c",c);
    }
}
