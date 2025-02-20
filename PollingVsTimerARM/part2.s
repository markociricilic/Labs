          .text                   // executable code follows
          .global _start        

_start:     LDR     R3, =0xFF200020     // for HEX
            LDR     R4, =0xFF200050    // for KEY
            MOV     R5, #0             // holds counter to be displayed  
            MOV     R9, #0             // detecting the button press

DISPLAY:    MOV     R0, R5          
            BL      DIVIDE         
            MOV     R6, R1

HEX0:       BL      SEG7_CODE       
            MOV     R2, R0          
            MOV     R0, R6

HEX1:       BL      SEG7_CODE       
            LSL     R0, #8          
            ORR     R2, R0       
            STR     R2, [R3]

POLL:       LDR     R8, [R4, #0xC]
            AND     R8, #0xF
            CMP     R8, #0
            BEQ     DO_DELAY

            MOV     R8, #0xF
            STR     R8, [R4, #0xC]
            EOR     R9, #1

DO_DELAY:   LDR     R7, =500000         // for hardware use =200000000
SUB_LOOP:   SUBS    R7, R7, #1
            BNE     SUB_LOOP

            CMP     R9, #0
            BNE     POLL

COUNTER:    CMP     R5, #99
            ADDLT   R5, #1
            MOVGE   R5, #0
            B       DISPLAY

// Program that converts a binary number to decimal
DIVIDE:     MOV     R2, #0
CONT:       CMP     R0, #10
            BLT     DIV_END
            SUB     R0, #10
            ADD     R2, #1
            B       CONT

DIV_END:    MOV     R1, R2     // tens in R1 (ones in R0)
            MOV     PC, LR

SEG7_CODE:  MOV     R1, #BIT_CODES  
            ADD     R1, R0         // index into the BIT_CODES "array"
            LDRB    R0, [R1]       // load the bit pattern (to be returned)
            MOV     PC, LR              

BIT_CODES:  .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
            .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
            .skip   2      // pad with 2 bytes to maintain word alignment

            .end