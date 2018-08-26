/**
 * mario.c
 *
 * David Disch
 *
 * Creates a two sided pyramid split down the center
 */

#include <cs50.h>
#include <stdio.h>

// Amount of characters between the pyramid halves
const int GAP_SIZE = 2;

// Characters for blocks and gaps
const char GAP[] = " ";
const char BLOCK[] = "#";
const char NEW_LINE[] = "\n";

int implementation_1(int height);
int implementation_2(int height);

int main(void)
{
    // Prompt user for integer until
    // we have an integer between 0 and 23
    int height;
    do
    {
        height = get_int("Let's build a pyramid!\nTell me how tall you want it (any integer from 0 to 23): ");
    }
    while (height < 0 || height > 23);

    // Chose to use implementation_1 because it
    // was simpler to read and satisfied tests
    implementation_1(height);

    return 0;
}

/**
 * Implementation #1
 *
 * Simple algorith to print x number of spaces,
 * followed by x number of blocks, 2 spaces and
 * then another x number of blocks depending on row #.
 */
int implementation_1(int height)
{
    int row;
    int gaps;
    int blocks;

    // Execute this block for each row of the pyramid
    for (row = 1; row <= height; row++)
    {
        for (gaps = height - row; gaps > 0; gaps--)
        {
            printf(GAP);
        }

        for (blocks = row; blocks > 0; blocks--)
        {
            printf(BLOCK);
        }

        for (gaps = GAP_SIZE; gaps > 0; gaps--)
        {
            printf(GAP);
        }

        for (blocks = row; blocks > 0; blocks--)
        {
            printf(BLOCK);
        }

        printf("\n");
    }

    return 0;
}

/**
 * Implementation #2
 *
 * More complex but allows for spaces to be appended
 * at the end of each row to maintain right alignment.
 * There might be use cases for this implementation over
 * the other depending on what the end goal of the app is.
 *
 * This implementation does not satisfy the tests as each
 * row has exactly 2 * height + 3 characters whereas the
 * test case only allows for \n characters right after the
 * end of the right pyramid half.
 */
int implementation_2(int height)
{
    // We need to add 1 to the line length because of the newline char
    const int LINE_LENGTH = 2 * height + GAP_SIZE + 1;

    int left_start;
    int left_end;
    int right_start;
    int right_end;
    int row;

    // Execute this block for each row of the pyramid
    for (row = 0; row < height; row++)
    {
        /**
         * Character index visualization of blocks assuming height of 4:
         *
         * "   3  6   "
         * "  23  67  "
         * " 123  678 "
         * "0123  6789"
         */

        /**
         * Character index visualization of gaps assuming height of 4:
         *
         * "012 45 789"
         * "01  45  89"
         * "0   45   9"
         * "    45    "
         */

        // We can determine whether to print a block
        // or a gap based on the following parameters:
        left_start       = -1 + height - row;
        left_end         = -1 + height;
        right_start      = height + GAP_SIZE;
        right_end        = height + GAP_SIZE + row;

        for (int x = 0; x < LINE_LENGTH; x++)
        {
            if ((x >= left_start && x <= left_end) || (x >= right_start && x <= right_end))
            {
                printf(BLOCK);
            }
            else if (x < LINE_LENGTH - 1)
            {
                printf(GAP);
            }
            else
            {
                printf(NEW_LINE);
            }
        }
    }

    return 0;
}