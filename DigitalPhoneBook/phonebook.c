/*
 * Author:      APS105H1 Teaching Team
 * Modified by: Marko Ciric
 * Student Number: 1006723967
 * 
 * Date: April 2021
 */

// #include "contactList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// IF IT'S EASIER, YOU NEED NOT BE OVERLY GENERAL/FLEXIBLE
// BUT CAN ASSUME THESE LIMITS
#define MAXCONTACTS 30 /* max # of contacts */
#define MAXPHONES 10 /* max # of phone numbers for each contact */

#define PHONE_NUMBER_MAX_LENGTH 12

/* Generates structures for storing names and phone numbers as pairs of 
data; 'next' is responsible for retrieving the following structure */
typedef struct contact {
   const char* contactName;
   const char* phoneNumber[PHONE_NUMBER_MAX_LENGTH];

   int phones; // Amount of phone numbers
   struct contact *next;
} contact;
contact *head;

// Prints out all contacts in the linked list
void ListAllContacts() {
   /* "for loop" that begins at the 'head' or initial name, then continues until the 
   last name (not NULL or outside list), in increments of a single name to the next */
   for (contact *ptr = head->next; ptr != NULL; ptr = ptr->next) {

      printf("Name: %s\n", ptr->contactName); // print the name at a given pointer
      printf("Phone(s):");

      // phones - 1 to make within bounds of linked list
      for (int i = ptr->phones - 1; i >= 0; i--) {
         printf(" %s", ptr->phoneNumber[i]); // prints all the phone numbers at a given pointer (for a name) BACKWARDS (from top to down)
      }
      printf("\n");
   }
}

/* When determining where a name fits in the list, this checks what name 
comes first in the alphabet then includes it where appropriate (using ASCII values). */ 

// x and y are two function names for the strings */
int StringComparison(char x[], char y[]) {

   int i = 0;
   
   // If the two strings are equal then returns 0
   if(strcmp(x, y) == 0) {
      return 0;
   }

   /* Checks every letter of the string (name) for whether the first difference (the change in letter) is 
   lower or higher, then returns a value 1 or -1 to indicate which one. */
   while (true) {
      if(x[i] > y[i]) {
         return 1;
      } else if(x[i] < y[i]) {
         return -1;
      }
      i++; // Goes through string
   }
   return 0;
}

// Inserts each new contact where it would be alphabetically in the linked list akin to the insertion sort algorithm and ASCII values
bool Insert(contact *ptr) {
   
   // Ignore ptr numbering as they are from previous debuggings (hence missing ptr1)
   contact *ptr4;
   contact *ptr3;
	contact *ptr5 = head;

   // Uses ptr2 to begin going through the entire linked list until end of boundary (last name)
   for (contact *ptr2 = head; ptr2->next != NULL; ptr2 = ptr2->next) {
      /* Locates the FIRST point that the passed pointers name comes earlier in the alphabet than the 
      next ptr already inserted in the linked list */
      if (StringComparison(ptr->contactName, ptr2->next->contactName) < 0) {
         // Ptr manipulations for insertion
         ptr3 = ptr2->next;
         ptr2->next = ptr;
         ptr->next = ptr3;
         ptr4 = ptr;

         /* Points the ptr at the final entry to NULL instead of where it was pointed 
         at the latest entry before insertion thus preventing cyclic linking */
         while(ptr4 != NULL) {
            if(ptr4->next == ptr) {
               ptr4->next = NULL;
			      break;
            }
            ptr4 = ptr4->next;
         }
		 break;
      }
   }
}

/*
 * convertInputToDataStructure
 * Takes the information in contactList.h and puts it into a data structure of your choosing, however: 1 mark of the lab will be
 * for using structs with pointers in your solution. You might also want to consider the outputs from the program (see the menu 
 * printed in printMenu) since some structures will make this easier than others.
 */
void convertInputToDataStructure() {

   // Performs memory allocation of pointers 
   head = (contact*) malloc(sizeof(contact));
   head->next = (contact*) malloc(sizeof(contact));
   contact* ptr = head->next;

   int counter = 0;

   // Goes through every entry of the contact link for making structs
   for (int i = 0; contactList[i] != NULL; i++) {
      // Makes the first name without insertion into list
      if (i == 0) {
         ptr->contactName = (contactList[i]);
         // Iterates the 'for loop' for future use for when leaving basecase it is at appropriate list entry
         i++;
         
         ptr->phones = 0; // Initializes amount of phone numbers

         // Finds the phone numbers out of next array entries
         while (contactList[i][0] <= '9') {
            ptr->phoneNumber[counter] = (contactList[i]);

            // Every time phone number is found it is added to counter and phone number is increased
            ptr->phones++;
            counter++;
            i++;
         }
         i--; // Accounts for the 'for loop' iterating once 'for loop' is repeated, thereby correcting 'i' positioning

      } // Adds and inserts a new struct every time a name is found
      else if (contactList[i][0] >= 'A') {
         counter = 0;

         while (ptr->next != NULL) {
            ptr = ptr->next;
         }

         ptr->next = (contact*) malloc(sizeof(contact));
         ptr = ptr->next;
         ptr->contactName = contactList[i];
         ptr->phones = 0;
         Insert(ptr); // Once new struct is made it inserts it into the linked list at sorted location
      } // Adds phone numbers for the new contact
      else if (contactList[i][0] <= '9') {
         ptr->phoneNumber[counter] = contactList[i];
         ptr->phones++;
         counter++;
      }
   }
}

/*
 * printMenu - Prints out the user menu
 */
void printMenu() {
   printf("\n\nEnter a command by number\n");
   printf("1. List all contacts in alphabetical order\n");
   printf("2. Print phone(s) for a contact\n");
   printf("3. Find duplicate entries\n");
   printf("4. Delete contact\n");
   printf("5. Exit the program\n");
   printf("Your input: ");
}

//eliminate trailing \n
char* rtrim(char* str) {
   int i;
   for (i = strlen(str) - 1; i >= 0 && str[i] == '\n'; i--)
      str[i] = '\0';
   return str;
}

// Displays the chosen contact
void PrintContact() {

   int located = 0;
   char userInput[128];

   // Takes the user input strings
   printf("Enter name: ");
   char* sInput = fgets(userInput, 128, stdin);
   sInput = rtrim(sInput); // "fgets" generates \n which rtrim removes

   // Searches in the linked list for the contact that was inputed and prints their name and phone numbers
   for (contact *ptr = head->next; ptr != NULL; ptr = ptr->next) {
      if (strcmp(userInput, ptr->contactName) == 0) {
         printf("Name: %s\nPhone(s):", ptr->contactName);
         for (int i = ptr->phones - 1; i >= 0; i--) {
            printf(" %s", ptr->phoneNumber[i]);
            located = 1;
         }
         printf("\n");
      }
   }

   // If no match was located then indicates such
   if (located == 0) {
      printf("Contact not found\n");
   }
}

/*
 * DeleteContact deletes a contact from the list
 * The user must enter the name of the contact to delete
 */
void DeleteContact() {

   printf("Enter name: ");

   int located = 0;
   char userInput[128];
   char* sInput = fgets(userInput, 128, stdin);
   sInput = rtrim(sInput); // "fgets" generates \n which rtrim removes

   contact *ptr2;
   
   // Searches in the linked list for the contact that was inputed and removes the contact through ptr manipulation
   for (contact *ptr1 = head; ptr1->next != NULL; ptr1 = ptr1->next) {
      if (strcmp(userInput, ptr1->next->contactName) == 0) {
         located = 1;
         ptr2 = ptr1->next;
         ptr1->next = ptr1->next->next;
         free(ptr2); // Removes the struct from memory when deleted
      }
   }

   if (located == 0) {
      printf("Contact not found\n");
   }
}

/*
 * findDuplicates finds the names of contacts with a phone number in common.
 */
void FindDuplicates() {

   int duplicate = 0;
   
   // Iterates through the entire linked list 
   for (contact *ptr1 = head; ptr1 != NULL; ptr1 = ptr1->next) {
      // Iterates through the linked list beginning at where ptr1 is
      for (contact *ptr2 = ptr1->next; ptr2 != NULL; ptr2 = ptr2->next) {
         // Iterates through each phone number for ptr1
         for (int i = 0; i < ptr1->phones; i++) {
            // Iterates through each phone number for ptr2
            for (int j = 0; j < ptr2->phones; j++) {
               // Indicates a duplicate was found between the two present contacts
               if (strcmp(ptr1->phoneNumber[i], ptr2->phoneNumber[j]) == 0) {
                  duplicate = 1;
               }
            }
         }
         // If a duplicate was found outputs the contact names that have a number in common
         if (duplicate == 1) {
            printf("%s and %s have a phone number in common\n", ptr1->contactName, ptr2->contactName);
         }
         duplicate = 0;
      }
   }
}

int main() {
   // first move data from contactList.h to a data structure
   convertInputToDataStructure();
   
   char notDone = 1;
   while (notDone) {
      printMenu();
      char userChoice[100];
      char* s = fgets(userChoice, sizeof(userChoice), stdin);
      if (!s)
      {
         printf("Wrong input\n");
         continue;
      }
      s = rtrim(s); /* eliminate trailing \n */
      //printf("selection = %s\n", s);
      int userSelection = atoi(s);

      switch (userSelection) {
         case 1: ListAllContacts(); break;
         case 2: PrintContact(); break;
         case 3: FindDuplicates(); break;
         case 4: DeleteContact(); break;
         case 5: notDone = 0; break;
         default: printf("Bad command...\n"); break;
         }
   }
   return 0;
}
