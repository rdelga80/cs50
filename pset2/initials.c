#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

void letter(string passed_name);

int main(void)
{
    // User enters name
    string name = get_string();

    // Call letter function, and pass name
    letter(name);

}

void letter(string passed_name)
{
    // If name is not null
    if(passed_name != NULL)
    {
        // Assign length name to variable
        int passed_name_len = strlen(passed_name);

        // Print each letter entered
        for(int i = 0; i < passed_name_len; i++)
        {
            // If i'th letter isn't a space, and the i'th - 1 letter is a space, or if
            // The iteration is the first letter of the name
            if( (passed_name[i] != ' ' && passed_name[i - 1] == ' ' ) || ( passed_name[0] != ' ' && i == 0 ))
            {
                // Print i'th letter as uppercase
                printf("%c", toupper(passed_name[i]));
            }
        }

        // Print line break
        printf("\n");

    }
}