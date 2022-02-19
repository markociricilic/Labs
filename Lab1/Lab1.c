#include <stdio.h>

// Student Name: Marko Ciric
// Student Number: 1006723967

/* The purpose of this lab is to have the user input 3 numbers, and the code output half the
sum, twice the product, and the mean (average) of these numbers. */

// Make sure to name the project EXACTLY "Lab1".
// Assume that the user enters valid numbers.
// Use double to make the outputs more accurate

int main(void)
{
    /* User inputs their choice of integer when given the command. The integer is
    displayed with a space after the prompt. (Procedure repeated for all 3 integers). */
    
    printf("Enter First Number: ");
    double inputNumber1;
    scanf("%lf",&inputNumber1);

    printf("Enter Second Number: ");
    double inputNumber2;
    scanf("%lf",&inputNumber2);

    printf("Enter Third Number: ");
    double inputNumber3;
    scanf("%lf",&inputNumber3);

    /* Functions are made using the previously inputted integers so that half the
    sum, twice the product, and the mean is calculated. They are outputted to 2 decimal places;
    hence '%.2lf'. */

    double halfSumOutput = (inputNumber1 + inputNumber2 + inputNumber3) / 2; // half the sum
    printf("Half the Sum: %.2lf\n",halfSumOutput);

    double twiceProductOutput = (inputNumber1 * inputNumber2 * inputNumber3) * 2; // twice the product
    printf("Twice the Product: %.2lf\n",twiceProductOutput);

    double averageOutput = (inputNumber1 + inputNumber2 + inputNumber3) / 3; // mean
    printf("Average: %.2lf\n",averageOutput);
    return 0;
}
