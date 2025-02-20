/* Program that counts consecutive 1's */

          .text                   // executable code follows
          .global _start                  
_start:     
          MOV     SP, #0x20000                        
          MOV     R4, #TEST_NUM   // loads the data words
          MOV     R5, #0          // R5 is initialized at 0 
          MOV     R6, #0          // R6 is initialized at 0 
          MOV     R7, #0          // R7 is initialized at 0 

LOOP_M:   LDR     R1, [R4]        // loads R1 from R4 (BUT does NOT add 4 to address like in previous part (keeps same address))
          CMP     R1, #0          
          BEQ     END             // if R1 is 0, end of TEST_NUM was reached branch to end of program

          BL      ONES
          CMP     R5, R0          // R0 holds the previous ONES number with longest sequence of 1s
          MOVLT   R5, R0          // R5 holds the resulting number with the longest sequence of 1s by copying
          LDR     R1, [R4]        

          BL      ZEROS
          CMP     R6, R0          // R0 holds the previous ZEROS number with longest sequence of 0s
          MOVLT   R6, R0          // R6 holds the resulting number with the longest sequence of 0s by copying
          LDR     R1, [R4]        /* loads R1 from R4 again (this is what allows us to use MVN since the list stays 
                                    same, only changing temp R1 in the ZEROS subroutine) */

          BL      ALTERNATE
          CMP     R7, R0          // R0 holds the previous ALTERNATE number with longest sequence of repeating 1s/0s
          MOVLT   R7, R0          // R7 holds the resulting number with the longest sequence of repeating 1s/0s by copying
          ADD     R4, #4          // here we finally go to the NEXT number in #TEST_NUM after evaluating each subroutine once
          B       LOOP_M            

END:      B       END

ONES:     MOV     R0, #0          // R0 is initialized at 0

LOOP_O:   CMP     R1, #0          // loop until the data contains no more 1's    
          BEQ     DONE_O          // branch to DONE if no more 1s found
          LSR     R2, R1, #1      // perform SHIFT, followed by AND
          AND     R1, R1, R2      // using AND logic operation if R1 and R1 both have 1 then result is 1, copy the result back into R1
          ADD     R0, #1          // count the string length so far
          B       LOOP_O

DONE_O:   MOV     PC, LR          // Return from subroutine  

ZEROS:    MOV     R0, #0          // R0 is reset to 0
          MVN     R1, R1          /* temporarily NOT-ing each number (0s->1s and 1s->0s) therefore allowing us to reuse ONES code
                                     to count the longest sequence of 0s in a number */

LOOP_Z:   CMP     R1, #0          // loop until the data contains no more 1's    
          BEQ     DONE_Z          // branch to DONE if no more 1s found
          LSR     R2, R1, #1      // perform SHIFT, followed by AND
          AND     R1, R1, R2      // using AND logic operation if R1 and R1 both have 1 then result is 1, copy the result back into R1
          ADD     R0, #1          // count the string length so far
          B       LOOP_Z

DONE_Z:   MOV     PC, LR          // Return from subroutine  

ALTERNATE: 
          LDR	  R8, =#0xaaaaaaaa  // 10101010... sequence loaded into R8
          MOV     R9, #0            // used for storing ONES
          MOV     R10, #0           // used for storing ZEROS
          
          EOR	  R1, R1, R8        // EOR == XOR logic expression which checks for sequence of 1s/0s that are sequential
          PUSH    {R1, LR}          // saving LR because we have nested subroutines that use R1 which need to be stored in stack
          BL      ONES              // runs ONES subroutine
          MOV     R9, R0            // copies the number with longest sequences of 1s into R9
          POP     {R1, LR}
          
          EOR	  R1, R1, R8        // same logic as above
          PUSH    {R1, LR}
          BL      ZEROS
          MOV     R10, R0
          POP     {R1, LR}
          
          MOV     R0, R9          // copying the number of 1s found into R0
          CMP     R10, R9         // comparing the number of 1s found with the number of 0s found
          MOVGT   R0, R10         // if the number of 0s found was greater than the number of 1s found replace R0 with the 0s
          MOV     PC, LR          // Return from subroutine   

TEST_NUM: .word   0x103fe00f    // longest 1s = 9   longest 0s = 9  alternating 1s0s = 2  
          .word   0x214ef12a    // longest 1s = 4   longest 0s = 4  alternating 1s0s = 7
          .word   0x417ff03c    // longest 1s = 11  longest 0s = 6  alternating 1s0s = 4
          .word   0x114ec20a    // longest 1s = 3   longest 0s = 5  alternating 1s0s = 5
          .word   0x811f000b    // longest 1s = 5   longest 0s = 12 alternating 1s0s = 4    [LONGEST 0s]
          .word   0x5f7ab11c    // longest 1s = 5   longest 0s = 3  alternating 1s0s = 7
          .word   0x312fe21d    // longest 1s = 7   longest 0s = 4  alternating 1s0s = 4
          .word   0x555555f0    // longest 1s = 4   longest 0s = 4  alternating 1s0s = 24   [LONGEST 1s0s]
          .word   0x795fff8b    // longest 1s = 14  longest 0s = 3  alternating 1s0s = 6    [LONGEST 1s]
          .word   0x00000000    // last word (loop stops since no 1s)

          .end

/*
TESTING NUMBERS:

TEST_NUM: .word   0x103fe00f   
          .word   0x55555555    longest alternate
          .word   0xaaaa0000    longest 0s
          .word   0xffffffff    longest 1s
          .word   0x00000000    

          .end
*/