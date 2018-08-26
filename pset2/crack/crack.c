/**
 * crack.c
 *
 * David Disch
 *
 * Takes a password hashed in C's DES based
 * crypt function and returns it plaintext
 */

#define _XOPEN_SOURCE
#define PBSTR "##########"
#define PBWIDTH 10
#define USAGE "Usage: ./crack <hashed_password>\n<hashed_password> - a password hashed in C's DES based crypt function"
#define EMPTY ""
#define PROGRESS_RESOLUTION 1000

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

void reverse(char *t);
void n_to_base(long n, char *pattern, char *output);
int cdes_encrypted_to_plaintext(char *encrypted_password, int min_guess_length, int max_guess_length, char *pattern, char *output);
void print_progress(long i, long m);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("%s\n", USAGE);
        return 1;
    }

    /**
     * Assumptions:
     *  - passwords can be any length
     *  - password length is less than 6
     *  - plaintext password (prior to hashing) only contained characters A-Z and a-z
     *  - that's 52^5 or 380,204,032 combinations!
     *
     * You can adjust the pattern to only
     * include characters which were permitted
     * in the original plaintext password.
     */
    int min_password_length = 0;
    int max_password_length = 5;
    char *pattern = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Output buffer
    char plaintext[max_password_length + 1];
    int found_plaintext = cdes_encrypted_to_plaintext(argv[1], min_password_length, max_password_length, pattern, plaintext);

    if (found_plaintext == 1)
    {
        printf("%s\n", plaintext);
    }
}

/**
 * Sets output buffer to the plaintext version
 * of ciphertext from C's DES based crypt function
 *
 * This implementation looks at the problem
 * as a number system where each combination
 * represents a different number in the number
 * system. The base of the number system is
 * strlen(pattern) and each number is represented
 * by the corresponding index in the pattern string.
 *
 * @var encrypted_password  ciphertext from C's DES crypt function to decrypt
 * @var min_guess_length    character length to start from
 * @var max_guess_length    character length to end at
 * @var pattern             the range of possible characters are
 * @var output              plaintext decrypted
 *
 * @returns int 1 if a plaintext was found, 0 if no plaintext was found
 */
int cdes_encrypted_to_plaintext(char *encrypted_password, int min_guess_length, int max_guess_length, char *pattern, char *output)
{
    /**
     * Examples:
     *
     * 50xcIMJ0y.RXo = YES
     * 50mjprEcqC/ts = CA
     * 50GApilQSG3E2 = UPenn
     * 50n0AAUD.pL8g = lloyd
     * 50CcfIk1QrPr6 = maybe
     * 509nVI8B9VfuA = TF
     * 50JIIyhDORqMU = nope
     * 50JGnXUgaafgc = ROFL
     * 51u8F0dkeDSbY = NO
     * 50cI2vYkF0YU2 = LOL
     */

    // Copy the first two characters
    // of encrypted_password into salt
    char salt[3];
    strncpy(salt, encrypted_password, 2);
    salt[2] = '\0';

    // The base of the number system we are using
    int base = strlen(pattern);

    // Which combination number to start from
    // We need to -1 from this because x^0 = 1
    long min = pow(base, min_guess_length) - 1;

    // The max number of combinations for this password
    long max = pow(base, max_guess_length) + 1;

    // Guess string buffer
    char guess[max_guess_length + 1];

    if (strcmp(crypt(EMPTY, salt), encrypted_password) == 0)
    {
        strcpy(output, EMPTY);
        return 1;
    }

    for (long i = min; i < max; i++)
    {
        // Convert number to alphanumeric
        n_to_base(i, pattern, guess);

        // Check guess
        if (strcmp(crypt(guess, salt), encrypted_password) == 0)
        {
            strcpy(output, guess);
            printf("\n");
            return 1;
        }

        // Clear the guess
        memset(guess, 0, sizeof(guess));

        // Print progress every 1%
        if ((i + 1) % (max / PROGRESS_RESOLUTION) == 0)
        {
            print_progress(i, max);
        }
    }

    printf("\n");

    return 0;
}

void n_to_base(long n, char *pattern, char *output)
{
    if (n < 0)
    {
        return;
    }

    int i = 0;
    int base = strlen(pattern);

    do
    {
        // Select the character from the
        // pattern which corresponds with
        // this number and add it to the
        // output
        output[i++] = pattern[n % base];
        n /= base;
    }
    while (n > 0);

    // Reverse the order
    reverse(output);

    // Terminate the string
    output[i] = '\0';
}

void reverse(char *string)
{
    int length, i;
    char *end, start;

    length = strlen(string);
    end = string + length - 1;

    for (i = 0; i < length / 2; i++)
    {
        start  = *string;
        *string = *end;
        *end = start;
        string++;
        end--;
    }
}

/**
 * Thanks to JeffV @ StackOverflow
 *
 * https://stackoverflow.com/a/36315819
 */
void print_progress(long i, long m)
{
    double percentage = (double)i / (double)m;
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s] %ld/%ld", val, lpad, PBSTR, rpad, "", i, m);
    fflush(stdout);
}