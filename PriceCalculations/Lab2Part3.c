#include <stdio.h>
#include <math.h>

// Student Name: Marko Ciric
// Student Number: 1006723967

/* The purpose of this lab is to write a program that calculates the 'future value of the investment after t number
of years, including interest". The function used is the formula for compound interest using values inputted by the
user. */

// Assume that the user enters valid numbers.
// Use double to make the outputs more accurate.

int main(void)
{
    // Variables inputted by user

    double inputInitialDeposit; // Initial Deposit Amount (Principal)
    float inputInterestRate; // Annual interest rate (Up to 2 decimal places)
    double inputCompoundRate; // Number of times that interest is compounded per year
    int inputTime; // Number of (whole) years the money is invested

    double outputFutureValue; // Expected future value of the investment (To 2 decimal places)

    printf("The amount deposited P: ");
    scanf("%lf",&inputInitialDeposit);

    printf("The interest rate r: ");
    scanf("%.2f",&inputInterestRate);

    printf("The number of times the interest is compounded n: ");
    scanf("%lf",&inputCompoundRate);

    printf("The period of time t the money is invested (in years): ");
    scanf("%d",&inputTime);

    outputFutureValue = inputInitialDeposit * pow((1 + (inputInterestRate / inputCompoundRate)),(inputCompoundRate * inputTime));

    printf("The future value of the investment A: %.2lf\n",outputFutureValue);
    return 0;
}