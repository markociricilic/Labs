/* Subroutine to convert the digits from 0 to 9 to be shown on a HEX display.
 *    Parameters: R0 = the decimal value of the digit to be displayed
 *    Returns: R0 = bit patterm to be written to the HEX display
 */     

           .text           
           .global _start

SEG7_CODE:  MOV     R1, #BIT_CODES  
            ADD     R1, R0         // index into the BIT_CODES "array"
            LDRB    R0, [R1]       // load the bit pattern (to be returned)
            MOV     PC, LR              

BIT_CODES:  .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
            .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
            .skip   2      // pad with 2 bytes to maintain word alignment

_start:     
          MOV     SP, #0x20000                        
          MOV     R4, #TEST_NUM   // loads the data words
          MOV     R5, #0          // R5 is initialized at 0 
          MOV     R6, #0          // R6 is initialized at 0 
          MOV     R7, #0          // R7 is initialized at 0 

LOOP_M:   LDR     R1, [R4]        // loads R1 from R4 (BUT does NOT add 4 to address like in previous part (keeps same address))
          CMP     R1, #0          
          BEQ     END_M             // if R1 is 0, end of TEST_NUM was reached branch to end of program

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

END_M:    B       DISPLAY         // branch to display, once all the registers are complete

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
          .word   0xaaaaaaf0    // longest 1s = 4   longest 0s = 4  alternating 1s0s = 24   [LONGEST 1s0s]
          .word   0x795fff8b    // longest 1s = 14  longest 0s = 3  alternating 1s0s = 6    [LONGEST 1s]
          .word   0x00000000    // last word (loop stops since no 1s)

/* Display R5 on HEX1-0, R6 on HEX3-2 and R7 on HEX5-4 */
DISPLAY:    LDR     R8, =0xFF200020 // loading the memory mapped address address base for HEX3-0
            MOV     R0, R5          // displaing R5 on HEX1-0 so copying the value of the number into R0
            BL      DIVIDE          // ones digit will be in R0; tens digit in R1
            MOV     R9, R1          // saving the tens digit into R9

HEX0:
            BL      SEG7_CODE       
            MOV     R4, R0          // save the ones bit code in R4
            MOV     R0, R9          // retrieve the tens digit, get bit code in R0, displaying the ones digit of R5 in HEX0

HEX1:                       
            BL      SEG7_CODE       
            LSL     R0, #8          // logic shifts left by 8 bits to display the tens digit of R5 in HEX1
            ORR     R4, R0          // R4 will hold both HEX0 and HEX1 digits
            
            // Setting up for HEX2 with R6
            MOV     R0, R6          // HEX3-2 will display with R6

            BL      DIVIDE
            MOV     R9, R1          // tens digit is held in R9 just as before

HEX2:
            BL      SEG7_CODE
            LSL     R0, #16         // logic shifts left by 16 bits to display the ones digit of R6 in HEX2
            ORR     R4, R0          // R4 will hold all HEX2-0 digits

            MOV     R0, R9          // copying the tens digit into R0 to use for HEX3

HEX3:
            BL      SEG7_CODE
            LSL     R0, #24         // logic shifts left by 24 bits to display the tens digit of R6 in HEX3
            ORR     R4, R0          // R4 will hold all HEX3-0 digits

			STR     R4, [R8]        // displays the number from R5 and R6
            
            // Setting up for HEX4 with R7
            LDR     R8, =0xFF200030 // loading the memory mapped address address base for HEX5-4
            MOV     R0, R7          // HEX5-4 will display with R7

            BL      DIVIDE          // R1 holds tens, R0 holds ones
            MOV     R9, R1          // R9 holds tens digit

HEX4:
            BL      SEG7_CODE       
            MOV     R4, R0          // R4 holds lower bit code
            MOV     R0, R9          // R0 holds tens digit code, displaying the ones digit of R7 in HEX4

HEX5:
            BL      SEG7_CODE       
            LSL     R0, #8          // logic shifts left by 8 bits to display the tens digit of R7 in HEX5
            ORR     R4, R0          // R4 will hold all HEX5-4 digits
			
            STR     R4, [R8]        // displays the number from R7

END:		B	   END

/* Program that converts a binary number to decimal <-- copied from Lab 1*/

// R1 holds tens, R0 holds ones
DIVIDE:     MOV    R2, #0

CONT:       CMP    R0, #10
            BLT    DIV_END
            SUB    R0, #10
            ADD    R2, #1
            B      CONT

DIV_END:    MOV    R1, R2     // quotient in R1 (remainder in R0)
            MOV    PC, LR

            .end
