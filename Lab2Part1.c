#include <stdio.h>
#include <math.h>

// Student Name: Marko Ciric
// Student Number: 1006723967

/* The purpose of this lab is to calculate the total amount the user will pay, if "for every 3 dollars
spent on produce in the store, they get 10 cents cash back". The total charge and savings are outputted. */

// Assume that the user enters valid numbers.
// Use double to make the outputs more accurate.

int main(void)
{
    // Variables

    double inputPricePound;
    double inputWeight;

    double totalBeforeSaving = (inputPricePound * inputWeight); // The total charge before saving

    double moneySaved = floor(totalBeforeSaving / 3) * 0.10;
    /* The amount saved - the total before saving is divided by 3 and rounded to the largest integer not larger than
    itself (floor function), then that is multiplied by 0.10 (10 cents) to determine how many cents are saved. */

    printf("Enter the price per pound: ");
    scanf("%lf",&inputPricePound);  

    printf("Enter the total weight: ");
    scanf("%lf",&inputWeight);

    printf("The total charge is: %.2lf\n", totalBeforeSaving - moneySaved); // The total charge after saving

    printf("You saved: %.2lf\n",moneySaved); // The total saved is displayed
    return 0;
}
