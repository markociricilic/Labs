          .text                   // executable code follows
          .global _start        

_start:     LDR     R3, =0xFF200020    // for HEX
            LDR     R4, =0xFF200000    // for KEY
            LDR     R7, =0xFFFEC600    // ARM A9 Private Timer
            LDR     R10, =2000000     // value to be loaded (200 MHz * 0.25 = 50 MHz)

			STR		R10, [R7], #8		// post increment to control
			LDR 	R10, =0b11			// starting the timer
			STR		R10, [R7], #4		// post increment to interrupt status to check

            MOV     R5, #0             // holds counter to be displayed  
            MOV     R9, #0             // detecting the button press
			MOV		R11, #0

DISPLAY:    MOV     R0, R5          
            BL      DIVIDE         
            MOV     R6, R1

HEX0:       BL      SEG7_CODE       
            MOV     R12, R0          
            MOV     R0, R6

HEX1:       BL      SEG7_CODE       
            LSL     R0, #8          
            ORR     R12, R0  
			
			MOV		R0, R11
			BL 		DIVIDE
			MOV		R6, R1
			
HEX2:		BL		SEG7_CODE
			LSL		R0, #16
			ORR		R2, R0
			MOV		R0, R6
			
HEX3:		BL		SEG7_CODE
			LSL		R0, #24
			ORR		R2, R0
			
			ADD		R12, R2, R12
			STR		R12, [R3]
			
			MOV		R6, #0

POLL:		LDR		R10, [R7]		        // update the end of the timer
			CMP		R10, #1				    // checking if the timer has ended
			BEQ		COUNTER
			
			LDR     R8, [R4, #0x5C]
            AND     R8, #0xF
            CMP     R8, #0
			BEQ		POLL
			
			MOV     R8, #0xF
            STR     R8, [R4, #0x5C]
            EOR     R9, #1

COUNTER:    STR		R10, [R7]
			
			CMP		R9, #1
			BEQ		POLL
			
            CMP     R5, #99
            ADDLT   R5, #1
            MOVGE   R5, #0
			ADDGE	R11, #1
			CMP		R11, #59
			MOVGE	R11, #0
			
            B       DISPLAY

// Program that converts a binary number to decimal
DIVIDE:     MOV     R2, #0
CONT:       CMP     R0, #10
            BLT     DIV_END
            SUB     R0, #10
            ADD     R2, #1
            B       CONT

DIV_END:    MOV     R1, R2     // tens in R1 (ones in R0)
			MOV		R2, #0
            MOV     PC, LR

SEG7_CODE:  MOV     R1, #BIT_CODES  
            ADD     R1, R0         // index into the BIT_CODES "array"
            LDRB    R0, [R1]       // load the bit pattern (to be returned)
            MOV     PC, LR              

BIT_CODES:  .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
            .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
            .skip   2      // pad with 2 bytes to maintain word alignment

            .end