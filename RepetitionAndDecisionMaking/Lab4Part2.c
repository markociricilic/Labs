#include <stdio.h>

// Student Name: Marko Ciric
// Student Number: 1006723967

/* The purpose of this lab is to write a program that uses '*' to draw a triangle with the number of rows determined
   by the user. */

int main(int argc, char **argv)
{
    // Defining user variable

    int rowInput;

    printf("Enter the number of rows in the triangle: "); // Prompts user to input a number of rows for the triangle
    scanf("%d",&rowInput);

    // The first 'for loop' is for defining the row '*'.
    // The second 'for loop' within the first is for the column (col) '*'.
    // The '*' prints for 3 sections of the triangle (left side, right side, and bottom); script in 'if statement'

    for (int rowNumber = 1; rowNumber <= rowInput; rowNumber++) {
        for (int col = 1; col < rowInput * 2; col++) {
            if (rowNumber == rowInput || col + rowNumber - 1 == rowInput || col - rowNumber + 1 == rowInput)
                printf("*");
            else
                printf(" ");
        }
        printf("\n");
    }
    return 0;
}