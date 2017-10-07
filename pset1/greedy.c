#include <stdio.h>
#include <cs50.h>
#include <math.h>

int get_cents();

int main(void)
{
    int change = get_cents();
    int coins  = 0;
    int coin_type[] = {25, 10, 5, 1};

    for (int i = 0; i < 4; i++)
    {
        while ( change >= coin_type[i] )
        {
            coins++;
            change-= coin_type[i];
        }
    }
    printf("%i\n",coins);
}

int get_cents()
{
    float f;
    do
    {
        printf("O hai! How much change is owed?\n");
        f = get_float();
    }
    while (f < 0);
    return ((int) round(f*100) );
}