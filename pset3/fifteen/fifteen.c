/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// all the numbers and size of the board
int ints, size;

// position of the "space" character
int space[2];

const int SPACE = 0;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
int * pos(int e);
void swap(int a, int b, int c, int d);



int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    ints = d*d - 1;
    size = ints + 1;
    space[0] = space[1] = d-1;

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int c,k;
    c = k = 0;
    
    // fill the board with d to 1 backwards
    for (int i = ints; i > 0; i--)
    {
        board[k][c] = i;
        c++;
        if (c==d)
        {
            c = 0;
            k++;
        }
    }
    
    // check for odd d and flip "1" and "2" accordingly
    if (d % 2 == 0)
    {
        board[d-1][d-3] = 1;
        board[d-1][d-2] = 2;
    }
    
    // the "space" character
    board[c][k] = SPACE;
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    int c,k;
    c = k = 0;
    printf("\n  ");
    
    for (int i = 1; i <= size; i++)
    {
        if (board[k][c] == SPACE)
        {
            printf(" _");
        }
        else
        {
            if (board[k][c] == i)
            {
                printf("\033[%d;%dm", 34,1);
            }
            printf("%2i",board[k][c]);
            printf("\033[%d;%dm", 39,0);
            
        }
        c++;
        if (c==d)
        {
            c = 0;
            k++;
            printf("\n\n  ");
        }
        else
        {
            printf(" ");
        }
    }
    printf("\b\b");
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // find the position of the requested tile
    int *p = pos(tile);
    
    // tile is not  in the array
    if (p[0] == -1)
    {
        return false;
    }
    
    // check if movable (touching the "space") and swap
    if      (p[1] + 1 <  d && board[ p[0] ][ p[1] + 1 ] == SPACE)
    {
        swap(p[0], p[1] + 1, p[0], p[1]);
        return true;
    }
    else if (p[1] - 1 >= 0 && board[ p[0] ][ p[1] - 1 ] == SPACE)
    {
        swap(p[0], p[1] - 1, p[0], p[1]);
        return true;
    }
    else if (p[0] + 1 <  d && board[ p[0] + 1 ][ p[1] ] == SPACE)
    {
        swap(p[0] + 1, p[1], p[0], p[1]);
        return true;
    }
    else if (p[0] - 1 >= 0 && board[ p[0] - 1 ][ p[1] ] == SPACE)
    {
        swap(p[0] - 1, p[1], p[0], p[1]);
        return true;
    }
    
    return false;
}

/**
 * Swaps an element with the space in the "board" array.
 * [a][b] is position of "space"
 * [c][d] - position of the element
 */
void swap(int a, int b, int c, int d)
{
    board[a][b] = board[c][d];
    board[c][d] = SPACE;
}

/**
 * Returns the position of a element in the "board" array
 */
int * pos(int e)
{
    static int res[] = {-1,-1};
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == e)
            {
                res[0] = i;
                res[1] = j;
                return res;
            }
        }
    }
    return res;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int c,k;
    c = k = 0;
    
    // check one element at a time
    for (int i = 1; i <= ints; i++)
    {
        if (board[k][c] != i)
        {
            return false;
        }
        c++;
        if (c==d)
        {
            c = 0;
            k++;
        }
    }
    
    // no error found means all ok
    return true;
}
