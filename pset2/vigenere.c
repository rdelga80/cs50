#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

#define LETTERALOWER 97
#define LETTERAUPPER 65
#define ALPHA 26

bool cipher_character(string key);
void cipher_text(string plaintext, string cipher_key);

int main(int argc, string argv[])
{

    // If argument count has no arugments or more than one argument
    if(argc > 2 || argc == 1)
    {
        printf("Usage: ./vigenere k\n");
        return 1;
    }
    else
    {
        // Asign argument to charcter, and check if is A-Za-z
        return cipher_character(argv[1]);
    }
}

bool cipher_character(string key)
{
    // Determine length of entered key to be used as length of loop
    int key_len = strlen(key);

    for(int i = 0; i < key_len; i++)
    {
        // If any letter in the key is not an alpha then return an error
        if(!isalpha(key[i]))
        {
            printf("Usage: ./vigenere k\n");
            return 1;
        }
    }
    // Assign argument key letter to arg_letter
    string cipher_key = key;

    // Prompt user to enter plaintext, then assign to plaintext string
    printf("plaintext: ");
    string plaintext = get_string();

    // Pass plaintext and cipher_key to cipher_text function
    printf("ciphertext: ");
    cipher_text(plaintext, cipher_key);

    // Return 0 to signify no errors
    return 0;
}

void cipher_text(string plaintext, string cipher_key)
{
    // Determine length of cipher_key string and use as break point in wrap around for determining cipher.
    int cipher_len = strlen(cipher_key);

    // Determine length of passed plaintext, then use as for loop length
    int plain_len = strlen(plaintext);

    // Create loop that applies key difference to entered plaintext
    for(int i = 0, a = 0; a < plain_len; a++)
    {
        // If var i equals the key letter, then restart are 0, "rolling over key value" to start over again for next letter.
        if(i == cipher_len) { i = 0; }

        // Create val based on cipher_key letter as uppercase value and moded to 65, uppercase letter base.
        int cipher_val = toupper(cipher_key[i]) % LETTERAUPPER;

        // If value in plaintext is not A-Za-z, then do not apply key value.
        char each_letter;
        if(!isalpha(plaintext[a]))
        {
            // Assign non-alpha letter direction, and do not advance i'th letter in cipher_key
            each_letter = plaintext[a];
        }
        else
        {

            // If letter is lowercase handle differently than uppercase
            if(islower(plaintext[a]))
            {
                // Add the plaintext letter value with the ciper letter value, subtract base of lowercase letter int val
                // The mod to find corresponding letter in alphabet, then add 97
                each_letter = ((((plaintext[a] + cipher_val) - LETTERALOWER) % ALPHA) + LETTERALOWER);
            }
            else
            {
                // Handling uppercase the same way, except with 65 base.
                each_letter = ((((plaintext[a] + cipher_val) % LETTERAUPPER) % ALPHA) + LETTERAUPPER);
            }

            // Continue to next i'th of cipher key
            i++;

        }

        // Display ciphered character
        printf("%c", each_letter);

    }
    printf("\n");
}