/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

#define LIMIT 65536

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
 */
void sort(int values[], int n)
{
    int i;
    
    // initialize the buffer
    int buf[LIMIT];
    for (i = 0; i < LIMIT; i++)
    {
        buf[i] = 0;
    }
    
    // iterate through "values" and save the results in the buffer
    for (i = 0; i < n; i++)
    {
        buf[ values[i] ]++;
    }
    
    // put back the sorted elements in "values"
    int j = 0;
    for (i = 0; i < LIMIT; i++)
    {
        for (int k = 0; k < buf[i]; k++)
        {
            values[j] = i;
            j++;
        }
    }
}