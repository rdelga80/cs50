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

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool change_tile(int blank_cell[], int tile);
bool won(void);

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
    // Calculate num of tiles
    int count_tile = (d * d) - 1;

    // two-dim array based on d as max sizes
    // int board[d][d];

    // two-dim array to populate array
    for(int row = 0; row < d; row++)
    {
        for(int col = 0; col < d; col++)
        {

            // assign int 0 to last tile, later to be substituted with underscore in print
            if(row == (d - 1) && col == (d - 1))
            {
                board[row][col] = 0;
            }
            else
            {
                // if board dim is even number
                if(d % 2 == 0 && count_tile == 2)
                {
                    // assign 1 to second to last tile, and 2 to last tile, before 0/blank
                    board[row][col] = 1;
                    board[row][col + 1] = 2;
                    col++;
                }
                else
                {
                    // assign count tile to array
                    board[row][col] = count_tile;

                    // subtrack count tiles to create desc order
                    count_tile--;
                }
            }
        }
    }

}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // Prints top line of holding box
    for(int a = 0; a < d; a++) printf("+----");
    printf("+\n");

    // Display 2-dim array by row, then col
    for(int row = 0; row < d; row++)
    {
        // divider
        printf("| ");
        for(int col = 0; col < d; col++)
        {
            // if value is 0 placeholder, display underscore
            if(board[row][col] == 0)
            {
                // empty space, green box
                printf("\033[42m__");
                printf("\033[49m | ");
            }
            // print value, single digits with extra space
            else
            {
                printf("%2i | ", board[row][col]);
            }
        }
        // between rows for binding box
        printf("\n");
        for(int a = 0; a < d; a++) printf("+----");
        printf("+\n");
    }
    printf("\n");
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{
    // check if move is legal

    // holder for row and col of 0/blank tile
    int blank_cell[2];

    // find tile location
    for(int row = 0; row < d; row++)
    {
        for(int col = 0; col < d; col++)
        {
            // if cell at current iteration is 0/blank
            if(board[row][col] == 0)
            {
                // store row and cell in blank_cell int array
                blank_cell[0] = row;
                blank_cell[1] = col;

                // pass coord and tile number to change_tile function, returns bool
                return change_tile(blank_cell, tile);

            }
        }
    }

    // default false if illegal move
    return false;
}

bool change_tile(int blank_cell[], int tile)
{

    int new_cell[2];
    int above_sel;
    int right_sel;
    int below_sel;
    int left_sel;

    // redestribute coords by row and cell
    int row = blank_cell[0];
    int col = blank_cell[1];

    // find legal values of surrounding tiles
    above_sel = board[row][col - 1];
    right_sel = board[row + 1][col];
    below_sel = board[row][col + 1];
    left_sel = board[row - 1][col];

    // find tile with matching surrounding tile and assign to new_cell int array
    if(tile == above_sel)
    {
        new_cell[0] = row;
        new_cell[1] = col - 1;
    }
    else if(tile == right_sel)
    {
        new_cell[0] = row + 1;
        new_cell[1] = col;
    }
    else if(tile == below_sel)
    {
        new_cell[0] = row;
        new_cell[1] = col + 1;
    }
    else if(tile == left_sel)
    {
        new_cell[0] = row - 1;
        new_cell[1] = col;
    }
    else
    {
        // return false if no matching tile, ie illegal
        return false;
    }

    // change tile into 0 tile
    board[row][col] = tile;
    board[new_cell[0]][new_cell[1]] = 0;

    // return true for legal move
    return true;

}


/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
    // determine number of non-blank tiles
    int check_nums[d * d];

    // iterate through tiles
    int counter = 0;
    for(int row = 0; row < d; row++)
    {
        for(int col = 0; col < d; col++)
        {
            // assign tile numbers to check_nums array in order
            check_nums[counter] = board[row][col];
            counter++;
        }
    }

    // iterate through check_nums array, one less so as not to calculate final number against next to final
    for(int a = 0; a < counter - 2; a++)
    {
        // if next b'th number isn't one greater than a'th number
        int b = a + 1;
        if(check_nums[a] + 1 != check_nums[b])
        {
            return false;
        }
    }
    printf("\n");

    return true;
}

