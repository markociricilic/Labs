#include <stdio.h>

// Student Name: Marko Ciric
// Student Number: 1006723967

/* The purpose of this lab is to write a program that makes change for amounts inputted by the user that are
less than one dollar. The program will output the minimum number of set of coins that make up the user input. */

int main(int argc, char **argv) 
{
    // Defining Variables

    int userCentInput, totalQuarter, totalDime, totalNickel, totalCents;

    printf("Please give an amount in cents less than 100: "); // Prompts user to input response
    scanf("%d",&userCentInput);

    // Beginning of while loop, user must input value greater than 0 and less than 100 to be valid otherwise breaks

    while (userCentInput > 0) {
        if (userCentInput >= 100)
            break;
        else if (userCentInput != 1) // If user inputs 1 then 'else', if not then 'else if' statement
            printf("%d cents: ",userCentInput);
        else
            printf("%d cent: ",userCentInput);

        /* Variables redefined below: user's input is modded (the remainder is reassigned as the new user's input)
        every time to reduce mathematical expressions throughout the script. */

        // userCentInput = userCentInput % X (redefines the user's input as the result of the remainder from / X)

        totalQuarter = userCentInput / 25;
        userCentInput = userCentInput % 25;

        totalDime = userCentInput / 10;
        userCentInput = userCentInput % 10;

        totalNickel = userCentInput / 5;

        totalCents = userCentInput % 5;

        // There following are all the 'if statements' that are necessary to output the minimum # of coin types

        /* The following 'if statements' detail if a type of coin is 0, then it does not print (not displayed)
        and if there exists a number of a type of coin to print it. */

        // This if statement is for when there ARE quarters
        if (totalQuarter != 0) {
            if (totalQuarter == 1)
                printf("%d quarter",totalQuarter); // If only 1 quarter print 'quarter', this is repeated for all types
            else if (totalQuarter > 1)
                printf("%d quarters",totalQuarter); // If more than 1 quarter print 'quarters', again for all types

            if (totalDime == 1)
                printf(", %d dime",totalDime);
            else if (totalDime > 1)
                printf(", %d dimes",totalDime);

            if (totalNickel == 1)
                printf(", %d nickel",totalNickel);
            else if (totalNickel > 1)
                printf(", %d nickels",totalNickel);

            if (totalCents == 1)
                printf(", %d cent.\n",totalCents);
            else if (totalCents != 0)
                printf(", %d cents.\n",totalCents);
            else
                printf(".\n"); // This is to cover '.' in case no cents are needed.
        } // Same as above, except for when there are NO quarters, but there ARE dimes
        else if (totalDime != 0) {
            if (totalDime == 1)
                printf("%d dime",totalDime);
            else if (totalDime > 1)
                printf("%d dimes",totalDime);

            if (totalNickel == 1)
                printf(", %d nickel",totalNickel);
            else if (totalNickel > 1)
                printf(", %d nickels",totalNickel);

            if (totalCents == 1)
                printf(", %d cent.\n",totalCents);
            else if (totalCents != 0)
                printf(", %d cents.\n",totalCents);
            else
                printf(".\n");
        } // For when there are NO quarters OR dimes, but there ARE nickels
        else if (totalNickel != 0) {
            if (totalNickel == 1)
                printf("%d nickel",totalNickel);
            else if (totalNickel > 1)
                printf("%d nickels",totalNickel);

            if (totalCents == 1)
                printf(", %d cent.\n",totalCents);
            else if (totalCents != 0)
                printf(", %d cents.\n",totalCents);
            else
                printf(".\n");
        } // If NO quarters, dimes, or nickels, but there ARE cents
        else if (totalCents != 0) {
            if (totalCents == 1)
                printf("%d cent.\n",totalCents);
            else if (totalCents != 0)
                printf("%d cents.\n",totalCents);
            else
                printf(".\n");
        }
        printf("Please give an amount in cents less than 100: "); // Prompts user to input new response
        scanf("%d",&userCentInput);
    }
    printf("Goodbye \n"); // If while loop breaks, then program prints Goodbye
    return 0;
}
