          .text                   // executable code follows
          .global _start        

_start:     LDR     R11, =0xFF200020    // for HEX 
            MOV     R0, #0              // digit to be displayed       

POLL:       LDR     R12, =0xFF200050    // for KEY
            LDR     R12, [R12]
            CMP     R12, #0
            MOVNE   R10, R12
            BNE     POLL              

KEY0:       CMP     R10, #1
            MOVEQ   R0, #0
        
KEY1:       CMP     R10, #2
            BEQ     INCREASE

KEY2:       CMP     R10, #4
            BEQ     DECREASE

KEY3:       CMP     R10, #8             
            BEQ     CLEAR
            BLT		SEG7_CODE
            B       POLL   		 

INCREASE:   CMP     R0, #9
            ADDLT   R0, #1
            B       KEY2

DECREASE:   CMP     R0, #0
            SUBGT   R0, #1
            B       KEY3

CLEAR:      MOV     R0, #0
            STR		R0, [R11]
            MOV     R10, #0

            LDR     R12, =0xFF200050    // for KEY
            LDR     R12, [R12]
            CMP		R12, #0
            BEQ		CLEAR

WAIT:		LDR     R12, =0xFF200050    // for KEY
            LDR     R12, [R12]
            MOV		R0, #0
            CMP		R12, #0
            BNE		WAIT

SEG7_CODE:  MOV     R9, #BIT_CODES  
            ADD     R9, R0
            LDRB    R9, [R9]       // load the bit pattern (to be returned)
            STR     R9, [R11]
            MOV     R10, #0
            B       POLL         
                            
BIT_CODES:  .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
            .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
            .skip   2      // pad with 2 bytes to maintain word alignment

            .end