#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include <crypt.h>
#include <unistd.h>
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#define ALPHA 26
#define LETTERA 65
#define LETTERALOWER 97
#define TOTAL_LOOPS 123
#define SWITCH_CASE_POINT 91

bool check_arg_length(int arg_length);
bool run_letter_loops(string arg_hash, string salt);
string crypt_attempt(char a, string salt);
string multi_crypt(char a, char b, char c, char d, string salt);

int main(int argc, string argv[])
{
    // If check_arg_length returns true, there is only one argument
    if(check_arg_length(argc))
    {
        // Assign entered has to a variable
        const string arg_hash = argv[1];

        // Declare a variable to hold first to letters of hash for salt
        char salt_first = argv[1][0];
        char salt_second = argv[1][1];

        // Sprintf lwo letters into salt
        char salt[3];
        sprintf(salt, "%c%c", salt_first, salt_second);
        string salt_con = salt;

        // Print return decrypted password and end loop
        return run_letter_loops(arg_hash, salt_con);
    }
    else
    {
        // If there is anything different from 1 argument in argv.
        return 1;
    }
}

bool check_arg_length(int arg_length)
{
    // Check if there are only two arguments.
    if(arg_length == 2)
    {
        return true;
    }
    else
    {
        // If the length is anything but 2, then print "Usage..." and return false, exiting main.
        printf("Usage: ./crack hash\n");
        return false;
    }

}


bool run_letter_loops(string arg_hash, string salt)
{
    string answer;
    // Initiate loop for first letter in possible four
    for(int a = LETTERA; a < TOTAL_LOOPS; a++)
    {
        // if int d == "Z", switch to lowercase "a"
        if(a == SWITCH_CASE_POINT) { a = LETTERALOWER; }

        // Pass letter a, and salt to crpyt_attempt
        // Assign the returned value from the encrypted passed letter to crypt_val
        string crypt_val = crypt_attempt(a, salt);

        // If the hash and the return crypt value match
        if(strcmp(arg_hash, crypt_val) == 0)
        {
            // Create string for letter, and print answer.
            char a_letter[2];
            sprintf(a_letter, "%c%c", a, '\0');
            answer = a_letter;
            printf("%s\n", answer);

            // Return 0 to end loop.
            return 0;
        }

        // Initiate loop for second letter in possible four
        for(int b = LETTERA; b < TOTAL_LOOPS; b++)
        {
            // if int d == "Z", switch to lowercase "a"
            if(b == SWITCH_CASE_POINT) { b = LETTERALOWER; }

            // Pass letter a, and b to mutli_crypt, and a 0 in the int c portion to inform mutli_crypt
            // to only use letters a and b
            // Assign returned value to two_crypt_res
            string two_crypt_res = multi_crypt(a, b, 0, 1, salt);

            // If the crypt_val_double matches the entered hash
            if(strcmp(arg_hash, two_crypt_res) == 0)
            {
                // Create string for letter, and print answer.
                char ab_letter[3];
                sprintf(ab_letter, "%c%c", a, b);
                answer = ab_letter;
                printf("%s\n", answer);

                // Return 0 to end loop.
                return 0;
            }

            // Initiate loop for third letter
            for(int c = LETTERA; c < TOTAL_LOOPS; c++)
            {
                // if int d == "Z", switch to lowercase "a"
                if(c == SWITCH_CASE_POINT) { c = LETTERALOWER; }

                // Pass letter a, b, and c to mutli_crypt, and a 0 in the int d portion to inform mutli_crypt
                // to only use letters a, b, and c
                // Assign returned value to three_crypt_res
                string three_crypt_res = multi_crypt(a, b, c, 0, salt);

                // If the three_crypt_res matches the entered hash
                if(strcmp(arg_hash, three_crypt_res) == 0)
                {
                    // Create string for letter, and print answer.
                    char abc_letter[4];
                    sprintf(abc_letter, "%c%c%c", a, b, c);
                    answer = abc_letter;
                    printf("%s\n", answer);

                    // Return 0 to end loop.
                    return 0;
                }

                // Initiate loop for fourth letter
                for(int d = LETTERA; d < TOTAL_LOOPS; d++)
                {
                    // if int d == "Z", switch to lowercase "a"
                    if(d == SWITCH_CASE_POINT) { d = LETTERALOWER; }

                    // Pass letter a, b, c, and d to mutli_crypt
                    // Assign returned value to four_crypt_res
                    string four_crypt_res = multi_crypt(a, b, c, d, salt);

                    // If the four_crypt_res matches the entered hash
                    if(strcmp(arg_hash, four_crypt_res) == 0)
                    {
                        // Create string for letter, and print answer.
                        char abcd_letter[5];
                        sprintf(abcd_letter, "%c%c%c%c", a, b, c, d);
                        answer = abcd_letter;
                        printf("%s\n", answer);

                        // returns 0 to end the loop.
                        return 0;
                    }

                }

            }

        }
    }

    return "No answer";
}

// Function used to create encrypted value if there is only one character
string crypt_attempt(char a, string salt)
{
    // Assign passed character into sprintf created new_letter, and assign to const char *pword (for crypt)
    char new_letter[2];
    sprintf(new_letter, "%c", a);
    const char *pword = new_letter;

    // Return encrypted passed pword character
    return crypt(pword, salt);
}

// Function used to create encrypted value if there is more than one character
string multi_crypt(char a, char b, char c, char d, string salt)
{
    // If c is equal to 0, then use char a, and b, to create the returned crypt value
    if(c == 0)
    {
        // create string from a, and b
        char combine_char[3];
        sprintf(combine_char, "%c%c", a, b);

        // Return encrypted value based on a and b string
        return crypt(combine_char, salt);
    }
    // If d is equal to 0, then use char a, b, and c to create the returned crypt value
    else if(d == 0)
    {
        // create string from a, b, and c
        char combine_char[4];
        sprintf(combine_char, "%c%c%c", a, b, c);

        // Return encrypted value based on a, b, and c string
        return crypt(combine_char, salt);
    }
    // If a, b, c, and d are all passed to create encrypted value
    else
    {
        // create string from a, b, c, and d
        char combine_char[5];
        sprintf(combine_char, "%c%c%c%c", a, b, c, d);

        // Return encrypted value based on a, b, c, and d string
        return crypt(combine_char, salt);
    }

}