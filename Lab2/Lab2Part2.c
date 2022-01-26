#include <stdio.h>
#include <math.h>

// Student Name: Marko Ciric
// Student Number: 1006723967

/* The purpose of this lab is to round the price to be payed to the closest nickel. */

// Assume that the user enters valid numbers.
// Use double to make the outputs more accurate.

int main(void)
{
    double inputPrice1;
    double outputPriceRounded = round(inputPrice1 / 0.05) * 0.05;
    /* Divide the value inputted by 0.05 to make it easier to round, then round the value from Line 18 to the
    nearest integer, and then multiply by 0.05 to 'return' it back to the original form inputted by the user. */

    printf("Enter the price as a float with two decimals: ");
    scanf("%lf",&inputPrice1); // User inputs the price before rounding

    printf("The total charge is: %.2lf\n",outputPriceRounded);
    return 0;
}