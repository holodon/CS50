/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // array size cannot be negative
    if (n < 0)
    {
        return false;
    }
    
    // set right, left and middle bounds
    int r = n -1;
    int l = 0;
    int m;
    while (l <= r)
    {
        
        // calculate middle
        m = (l + r)/2;
        if (values[m] == value)
        {
            
            // value found
            return true;
        }
        else
        {
            if (value < values[m])
            {
                
                // search to the left (move the right boundary)
                r = m - 1;
            }
            else // value > values[m]
            {
                
                // search to the right (move the left boundary)
                l = m + 1;
            }
        }
    }

    // nothing found
    return false;
}

/**
 * Sorts array of n values.
 * (insertion sort)
 */
void sort(int values[], int n)
{
	// buffer for swaping
	int b;
	
	// backwards counter for shifting
	int j;
	
	for (int i = 1; i < n; i++)
	{
		
		// save the i-th element in the buffer
		b = values[i];
		j = i-1;
		
		// shift all bigger elements to the right
		while ( j>=0 && values[j] > b )
		{
			values[j+1] = values[j];
			j = j-1;
		}
		values[j+1] = b;
	}
}