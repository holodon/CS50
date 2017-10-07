#include <stdio.h>
#include <cs50.h>

void collatz(int n, int *i);

int main(int argc, string argv[])
{
   int i = 0;

   // check if called with the proper number of arguments
   if (argc != 2)
   {
     printf("Usage: ./collatz n\n");
     return 1;
   }
   
   // convert the first argument to integer
   int n = atoi(argv[1]);
   
   collatz(n, &i);
   printf("Steps: %i \n", i);
}

void collatz(int n, int *i)
{
   if (n == 1)
      return;
   else
   {
      *i += 1;
      if ( (n % 2) == 0 )
         collatz( n/2, i);
      else
         collatz( (3*n + 1), i);
   }
}
