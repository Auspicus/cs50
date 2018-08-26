/**
 * credit.c
 *
 * David Disch
 *
 * Checks credit card issuer and confirms number is valid
 */

#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int llong_digits(long long llong_value);
int llong_n_digit(long long number, int n);

bool hpl_card_number_validation(long long card_number);
int get_card_issuer(long long card_number);

const char *CARD_ISSUERS[] = {"INVALID", "AMEX", "VISA", "MASTERCARD"};

int main(void)
{
    const char *result = CARD_ISSUERS[0];
    long long card_number;

    // Largest number will be 16 digits long, which a regular long cannot store
    // Long max digits is 10 and long long is 19
    // We need to use a long long but get_long_long is deprecated
    // See: https://github.com/cs50/libcs50/blob/72dbc412f47f7d94b4907f143a39d2e8b9932c19/README.md#usage
    do
    {
        card_number = get_long("Enter a credit or debit card number: ");
    }
    while (card_number < 0);

    const int card_number_length = llong_digits(card_number);

    // Check that card number length is between 13 and 16
    if (card_number_length >= 13 && card_number_length <= 16)
    {
        // Determine whether credit card number is from one of the issuers
        int card_issuer_index = get_card_issuer(card_number);

        // Run extra validation if the card issuer is not invalid
        if (card_issuer_index > 0)
        {
            if (hpl_card_number_validation(card_number))
            {
                result = CARD_ISSUERS[card_issuer_index];
            }
        }
    }

    printf("%s\n", result);
    return 0;
}

/**
 * Runs Hans Peter Luhn validation on card number
 */
bool hpl_card_number_validation(long long card_number)
{
    int checksum = 0;

    for (int i = 1; card_number; card_number /= 10, ++i)
    {
        int right_most_digit = card_number % 10;

        if (i % 2 == 0)
        {
            right_most_digit *= 2;

            if (right_most_digit < 10)
            {
                // Right most digit was <= 4
                // So we can add the doubled value
                // directly to the checksum (0-8)
                checksum += right_most_digit;
            }
            else
            {
                // Right most digit was >= 5
                // So we add the two digits
                // of the new doubled value (10-18)
                checksum += (right_most_digit / 10);
                checksum += (right_most_digit % 10);
            }
        }
        else
        {
            checksum += right_most_digit;
        }
    }

    return (checksum % 10 == 0);
}

/**
 * Gets card issuer based on card number
 *
 * Returns index of card issuer if valid, or 0 if invalid
 */
int get_card_issuer(long long card_number)
{
    int first_digit = llong_n_digit(card_number, 0);
    int second_digit = llong_n_digit(card_number, 1);

    switch (first_digit)
    {
        case 3:
            switch (second_digit)
            {
                case 4:
                case 7:
                    return 1;
            }
            break;
        case 4:
            return 2;
        case 5:
            switch (second_digit)
            {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    return 3;
            }
    }

    return 0;
}

/**
 * Returns number of digits in a long long number
 */
int llong_digits(long long llong_value)
{
    return (int) log10(llong_value) + 1;
}

/**
 * Returns n digit in a positive long long number
 *
 * n is from 0 to number length - 1
 */
int llong_n_digit(long long number, int n)
{
    if (number < 0)
    {
        return -1;
    }
    int digits = llong_digits(number);
    if (n < 0 || n > digits - 1)
    {
        return -1;
    }
    return (int)(number / (long long) pow(10, digits - (n + 1))) % 10;
}