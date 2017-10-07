/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"


void split_merge(int b[], int a[], int l, int r);
void merge(int b[], int a[], int l, int m, int r);
void copy(int a[], int b[], int n);


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
    
    int buf[n];
    
    // copy the array in the buffer
    copy(values, buf, n);
    
    // start the recursion
    split_merge(buf, values, 0, n);
    
    return;
}

/**
 * Recursive split for the merge sort
 */
void split_merge(int b[], int a[], int l, int r)
{
    
    // one element in the array means sorted
    if ( (r - l) < 2 )
    {
        return;
    }
    
    // find the middle
    int m = (l + r)/2;
    
    // split both sides recursively
    split_merge(a, b, l, m);
    split_merge(a, b, m, r);
    
    // merge back to A
    merge(b, a, l, m, r);
}

/**
 * Merge fucntion for the merge sort
 */
void merge(int b[], int a[], int l, int m, int r)
{
    
    // index in each array
    int i = l;
    int j = m;
    
    // index in A
    int k = l;
    
    // while there are elements on each side
    while ( i < m || j < r )
    {
        
        // copy element from the left array
        if ( i<m && (b[i] < b[j] || j >= r) )
        {
            a[k] = b[i];
            i++;
        }

        // copy element from the right array
        else
        {
            a[k] = b[j];
            j++;
        }
        k++;
    }
}

/**
 * Copy array A into array B
 */
void copy(int a[], int b[], int n)
{
    for (int i = 0; i < n; i++)
    {
        b[i] = a[i];
    }
}
