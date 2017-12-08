#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height = 0;
    do
    {
        if(height > 23 || height < 0)
        {
            printf("Please enter a height more than 0, and less than 23.\n");
        }
        printf("Enter half-pyramid's height (less than 23): ");
        height = get_int();
    }
    while(height > 23 || height < 0);

    for(int i = 0; i < height; i++)
    {
        int spaces = height - i - 1;
        int block = height - spaces;

        for(int n = 0; n < spaces; n++)
        {
            printf(" ");
        }
        for(int j = 0; j < block; j++)
        {
            printf("#");
        }
        printf("  ");
        for(int j = 0; j < block; j++)
        {
            printf("#");
        }
        printf("\n");
    }

}