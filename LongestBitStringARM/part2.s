/* Program that counts consecutive 1's */

          .text                   // executable code follows
          .global _start                  
_start:                             
          MOV     R4, #TEST_NUM   // loads the data words
          MOV     R5, #0          // R5 is initialized at 0 (IT IS THE NUMBER WITH THE LONGEST CURRENT SEQUENCE OF 1s)

LOOP_M:   LDR     R1, [R4], #4    // loads R1 from R4 then adds 4 to address 
          CMP     R1, #0          
          BEQ     END             // if R1 is 0, end of TEST_NUM was reached branch to end of program
          BL      ONES
          CMP     R5, R0          // R0 holds the previous ONES number with longest sequence of 1s
          MOVLT   R5, R0          /* R5 holds the resulting number with the longest sequence of 1s by copying
                                    the value in R0 into R5 if it is greater than it otherwise keeping R5 as is */
          B       LOOP_M

END:      B       END

ONES:     MOV     R0, #0          // R0 is initialized at zero 1s (IT IS THE CURRENT NUMBER)

LOOP:     CMP     R1, #0          // loop until the data contains no more 1's    
          BEQ     DONE            // branch to DONE if no more 1s found
          LSR     R2, R1, #1      // perform SHIFT, followed by AND
          AND     R1, R1, R2      // using AND logic operation if R1 and R1 both have 1 then result is 1, copy the result back into R1
          ADD     R0, #1          // count the string length so far
          B       LOOP

DONE:     MOV     PC, LR          // Return from subroutine ONES                       

TEST_NUM: .word   0x103fe00f    // longest = 9
          .word   0x214ef12a    // longest = 4
          .word   0x417ff03c    // longest = 11
          .word   0x114ec20a    // longest = 3
          .word   0x5f7ab11c    // longest = 5
          .word   0x312fe21d    // longest = 7
          .word   0x116fb23b    // longest = 5
          .word   0x111af14a    // longest = 4
          .word   0x795fff8b    // longest = 14 --> this should be the word with the longest sequence of 1s 
          .word   0x00000000    // last word (loop stops since no 1s)

          .end                               