#include <stdio.h>
#include <cs50.h>

int get_sec(long long i);
int sum_flip(long long i, bool flip, int m);
int sum_sum(long long  i);


int main(void)
{
    printf("Number: ");
    long long card = get_long_long();
    int sum = sum_flip(card,false,2) + sum_flip(card,true,1);
    if (sum % 10 == 0)
    {
        long long i = card;
        while (i>10)
        {
            i /=10;
        }
        int m = get_sec(card);
        switch (i)
        {
            case 3:
                if (m == 4 || m == 7)
                {
                    printf("AMEX\n");
                }
                else
                {
                    printf("INVALID\n");
                }
                break;
            case 4:
                printf("VISA\n");
                break;
            case 5:
                if (m > 0 && m < 6)
                {
                    printf("MASTERCARD\n");
                }
                else
                {
                    printf("INVALID\n");
                }
                break;
            default:
                printf("INVALID\n");
                break;
        }
    }
    else 
    {
        printf("INVALID\n");
    }
}

int get_sec(long long i)
{
    while (i>100)
    {
        i /= 10;
    }
    return (int) (i%10);
}

int sum_flip(long long i, bool flip, int m)
{
    int sum = 0;
    while (i != 0)
    {
        if (flip)
        {
            sum+= sum_sum((i%10)*m);
        }
        i /= 10;
        flip = !flip;
    }
    return sum;
}

int sum_sum(long long  i)
{
    int sum = 0;
    while (i != 0)
    {
        sum+= i%10;
        i /= 10;
    }
    return sum;
}