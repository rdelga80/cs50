#include <stdio.h>
#include <cs50.h>
#include <string.h>

int is_triangle(int sides[3]);

int main(void)
{
    int side[3];

    // Function to ask customer's for length input for three sides
    for(int i = 0; i < 3; i++)
    {
        // Run do loop is side isn't positive
        do
        {
            // Prompt user entry based on side
            printf("Length of side %i: ", i + 1);
            side[i] = get_int();

            // If Side is less than 0 - negative
            if(side[i] <= 0)
            {
                // Print error
                printf("Please enter a positive length.\n");
            }
        }
        while(side[i] <= 0);
    }

    // Call Is Triangle Function
    int tri_answer = is_triangle(side);
    if(tri_answer)
    {
        // Two sides were greater than the third.
        printf("This is a triangle.");
    }
    else
    {
        // If return is a 1, true, then it didn't match the qualifier
        // of adding two sides together and greater than the third.
        printf("That cannot be a triangle.");
    }
    printf("\n");

}

int is_triangle(int sides[3])
{
    // Run if statements if any two sides are greater than the last side
    if(sides[0] + sides[1] > sides[2])
    {
        // Returns 1 to show that there was a match, true
        return 1;
    }
    if(sides[1] + sides[2] > sides[0])
    {
        return 1;
    }
    if(sides[2] + sides[0] > sides[1])
    {
        return 1;
    }
    else
    {
        // Returns 0 to show there wasn't a match, false
        return 0;
    }

}