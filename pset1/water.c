#include <stdio.h>
#include <cs50.h>

bool input_okay(int i);

int main(void)
{
    // the text description says we do not need to worry about checking the user input, 
    //      but Zamyla in the video said otherwise. So to be sure I implemented it.
    // It also looks that the demo version is compiled with another cs50.h, 
    //      because the get_int now checks for integer overflow.
    int i;
    do
    {
        printf("Minutes: ");
        i = get_int();
    }
    while (!input_okay(i));
    printf("Bottles: %i\n", i*12);
}

bool input_okay(int i)
{
    if (i<0) {
        printf("Really ?? Come on - give me a real answer!\n");
        return false;
    }
    return true;
}
