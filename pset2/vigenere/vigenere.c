/**
 * vigenere.c
 *
 * David Disch
 *
 * Takes an input keyword and plaintext and
 * encrypts it using the vigenére cipher
 */

#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void print_vigenere(char *plaintext, char *keyword);
int contains_non_alpha(char *string);

const char *USAGE = "Usage: ./vigenere <keyword>\n<keyword> - a string of any length with characters A-Z and a-z";

int main(int argc, char *argv[])
{
    // Confirm only 1 argument was passed
    if (argc != 2)
    {
        printf("%s\n", USAGE);
        return 1;
    }

    // Confirm key contains only alphabetic characters
    if (contains_non_alpha(argv[1]) != 0)
    {
        printf("%s\n", USAGE);
        return 1;
    }

    char *plaintext = get_string("plaintext:  ");
    print_vigenere(plaintext, argv[1]);
}

/**
 * Checks a string for non alphabetic characters
 *
 * Returns number of non alphabetic characters in string
 */
int contains_non_alpha(char *string)
{
    int count = 0;
    // Loop through each character in the string
    for (int i = 0; i < strlen(string); i++)
    {
        if (isalpha(string[i]) == 0)
        {
            count++;
        }
    }
    return count;
}

/**
 * Take plaintext and a keyword and print
 * the plaintext and vigenere ciphertext
 */
void print_vigenere(char *plaintext, char *keyword)
{
    printf("ciphertext: ");
    int plaintext_length = strlen(plaintext);
    int keyword_length = strlen(keyword);
    int offset;
    int zero_based_plaintext_char;
    int zero_based_ciphertext_char;
    int keyword_index = 0;
    char ciphertext_char;
    for (int i = 0; i < plaintext_length; i++)
    {
        ciphertext_char = plaintext[i];
        if (isalpha(plaintext[i]))
        {
            // This offset is based on where the letter
            // aA is in ASCII, upper A is 65 and lower a is 97
            // See: http://www.asciichart.com/
            offset = isupper(plaintext[i]) ? 65 : 97;
            // After we perform this operation we need to
            // reverse the offset so we save the offset in a variable
            zero_based_plaintext_char = plaintext[i] - offset;
            zero_based_ciphertext_char = toupper(keyword[keyword_index % keyword_length]) - 65;
            ciphertext_char = (zero_based_plaintext_char + zero_based_ciphertext_char) % 26 + offset;
            // Progress our index in the keyword
            keyword_index++;
        }
        printf("%c", ciphertext_char);
    }
    printf("\n");
}