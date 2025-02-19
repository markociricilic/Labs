#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Student Name: Marko Ciric
// Student Number: 1006723967

// The purpose of this lab is to write a program that plays a simple game of Blackjack.

int randomCard() {
    return rand() % 13 + 1; // Generates a random card between 1 - 13, which is given to player/dealer
}

int main(int argc, char **argv)
{
    // Seed Random Number (required for marking process)
    if(argc == 1)
        srand(time(NULL));
    else
        srand(atoi(argv[1]));

    // Initial Variables defined for player and dealer (taken from function outside of main function)

    int initialValueDealer = randomCard();
    int initialValuePlayer = randomCard();
    int playerDecision;
    int newCard;

    printf("First cards: player %d, dealer %d \n",initialValuePlayer,initialValueDealer);

    int playerTotal = (initialValuePlayer >= 10) ? 10 : initialValuePlayer;
    int dealerTotal = (initialValueDealer >= 10) ? 10 : initialValueDealer;

    // Player Script
    while (true) {
        printf("Type 1 for Hit, 0 to Stay:");
        scanf("%d",&playerDecision); // Player inputs whether to hit (1) or stay (0)

        if (playerDecision == 1) {
            newCard = randomCard();

            if (newCard >= 10) {
                playerTotal += 10; // Any card 10 or greater gets calculated as equal to 10 and added to previous total
            } else {
                playerTotal += newCard; // Card is calculated as its value and added to previous total
            }
            printf("Player gets a %d, worth is %d\n",newCard,playerTotal); // Player hits and receives card

            if (playerTotal > 21) {
                printf("Player over 21, Dealer wins\n");
                break;
            }
        } else if (playerDecision == 0) {
            // Dealer Script below
            printf("Dealer gets: ");

            while (dealerTotal < 17) {

                newCard = randomCard();

                if (newCard >= 10) {
                    dealerTotal += 10; // See line 43
                } else {
                    dealerTotal += newCard; // See line 45
                }
                printf("%d ",newCard); // Dealer's cards are listed
            }
            printf("\n");

            if (dealerTotal > 21) {
                printf("Dealer over 21, Player wins\n");
                    break;
            } else if (dealerTotal > playerTotal && dealerTotal <= 21) {
                printf("Dealer better than Player, Dealer wins\n");
                    break; // Game ends when condition is met (&& = both conditions must be true)
            } else if (playerTotal > dealerTotal && playerTotal <= 21) {
                printf("Player better than Dealer, Player wins"); // Same as line 74
                    break;  
            } else {
                    printf("Tie!");
            }
        }
    }
    return 0;
}
