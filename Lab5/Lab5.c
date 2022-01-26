#include <stdio.h>
#include <math.h>

// Student Name: Marko Ciric
// Student Number: 1006723967

/* The purpose of this lab is to write a program that uses a function 'triange' to output Pascal's Triangle. The
number of rows is limited to between 0 to 13 inputted by the user. */

// Function for n!
int factorial(int n)
{
    if (n == 0 || n == 1) // The factorial of 0 or 1 is equal to 1
        return 1;
    else
        return n * factorial(n - 1); // Returns all other factorials
}

// Function for nCr
int choose(int n, int r)
{
    return factorial(n) / (factorial(r) * factorial(n - r)); // nCr equation for choosing r from n
}

// Function for spaces (may not be used)
void spaces(int space)
{
    if (space > 99)
        printf("   ");
    else if (space > 9)
        printf("    ");
    else 
        printf("     ");
}

// Function for Pascal's Triangle
void triangle(int rowInput)
{
    for (int rowNumber = 1; rowNumber <= rowInput; rowNumber++) {
    printf("%*s", ((rowInput - rowNumber) * 3), ""); // Indents (spaces) each line forming the triangle

// For columns (col)
        for (int col = 1; col <= rowNumber; col++) {
            printf("%-6d",choose(rowNumber - 1, col - 1)); // Triangle begins on row and column 0
        }
    printf("\n");
    }
}

int main(int argc, char **argv)
{
int initialValue = 1; // Random variable to start the while loop (user input not declared yet)
int rowInput; // User inputs number of rows

    while (initialValue == 1) {
        printf("Enter the number of rows: ");
        scanf("%d",&rowInput);

        if (rowInput < 0)
            break; // If user inputs negative integer then program stops

        triangle(rowInput); // calls and executes triangle function
    }
return 0;
}
