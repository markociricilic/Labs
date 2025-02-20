               .equ      EDGE_TRIGGERED,    0x1
               .equ      LEVEL_SENSITIVE,   0x0
               .equ      CPU0,              0x01    // bit-mask; bit 0 represents cpu0
               .equ      ENABLE,            0x1

               .equ      IRQ_MODE,          0b10010
               .equ      SVC_MODE,          0b10011

               .equ      INT_ENABLE,        0b01000000
               .equ      INT_DISABLE,       0b11000000
/*********************************************************************************
 * Initialize the exception vector table
 ********************************************************************************/
                .section .vectors, "ax"

                B        _start             // reset vector
                .word    0                  // undefined instruction vector
                .word    0                  // software interrrupt vector
                .word    0                  // aborted prefetch vector
                .word    0                  // aborted data vector
                .word    0                  // unused vector
                B        IRQ_HANDLER        // IRQ interrupt vector
                .word    0                  // FIQ interrupt vector

/*********************************************************************************
 * Main program
 ********************************************************************************/
                .text
                .global  _start
_start:        
                /* Set up stack pointers for IRQ and SVC processor modes */
                MSR		  CPSR_c, #0b11010010 		// interrupts masked (off), MODE = IRQ
				        LDR		  SP, =0x20000           	// set IRQ stack pointer
				
                MSR		  CPSR_c, #0b11010011			// interrupts masked, MODE = Supervisor (SVC)						
                LDR		  SP, =0x40000				    // set supervisor mode (SVC) stack 

                BL       CONFIG_GIC             // configure the ARM generic interrupt controller

                // Configure the KEY pushbutton port to generate interrupts
                LDR		  R0, =0xFF200050				  // pushbutton KEY base address
				        MOV		  R1, #0xF				        // set interrupt mask bits
				        STR		  R1, [R0, #0x8]				  // interrupt mask register is (base + 8)

                // enable IRQ interrupts in the processor
                MSR     CPSR_c, #0b01010011         // IRQ unmasked (enabled), MODE = SVC
IDLE:           
                B        IDLE                    // main program simply idles

IRQ_HANDLER:
                PUSH     {R0-R7, LR}
    
                /* Read the ICCIAR in the CPU interface */
                LDR      R4, =0xFFFEC100
                LDR      R5, [R4, #0x0C]         // read the interrupt ID

CHECK_KEYS:
                CMP      R5, #73
UNEXPECTED:     BNE      UNEXPECTED              // if not recognized, stop here
    
                BL       KEY_ISR
EXIT_IRQ:
                /* Write to the End of Interrupt Register (ICCEOIR) */
                STR      R5, [R4, #0x10]
    
                POP      {R0-R7, LR}
                SUBS     PC, LR, #4

/*****************************************************0xFF200050***********************************
 * Pushbutton - Interrupt Service Routine                                
 *                                                                          
 * This routine checks which KEY(s) have been pressed. It writes to HEX3-0
 ***************************************************************************************/
                  .global  EMPTY
EMPTY:            .word   0
                
                .global  KEY_ISR
KEY_ISR:
                PUSH    {R0-R12, LR}
                LDR     R5, =0xFF200020   // based address of HEX display 
                LDR     R0, =0xFF200050   // base address of pushbutton KEY port
                LDR     R1, [R0, #0xC]    // read edge capture register
                STR     R1, [R0, #0xC]    // clear the interrupt

                LDR     R0, =EMPTY        // sets KEY port address to 0
                LDR     R3, [R0]          // loads the value in the KEY port address into R3 (for holding the digit to be displayed)
                EOR     R1, R3            // changes display from showing digit to be blank by EORing the 7-seg code with the KEY port
                STR     R1, [R0]          // resets the key to the base address of the KEY port

                MOV     R3, #0            // R3 holds digit to be load to display 

// Show digit 0 on the HEX0 display when KEY0 is pressed, if pressed again display goes blank
KEY0:
                MOV     R2, #0x1          // copies the bit for KEY0 into R2
                ANDS    R2, R2, R1        // check for KEY0 out of all KEYs
                BEQ     KEY1              // if KEY0 not pressed, check for KEY1
                MOV     R3, #0b00111111   // if KEY0 pressed, have R3 hold 7-segment code for digit 0
              

// Show digit 1 on the HEX1 display when KEY1 is pressed, if pressed again display goes blank
KEY1:
                MOV     R2, #0x2            // copies the bit for KEY1 into R2
                ANDS    R2, R2, R1          // check for KEY1 
                BEQ     KEY2                
                MOV     R2, #0b00000110     // if KEY1 pressed, have R3 hold 7-segment code for digit 1
                LSL     R2, #8              // shift to HEX1 address
                ORR 	  R3, R2
              

// Show digit 2 on the HEX2 display when KEY2 is pressed, if pressed again display goes blank
KEY2:
                MOV     R2, #0x4            // copies the bit for KEY2 into R2
                ANDS    R2, R2, R1          // check for KEY2
                BEQ     KEY3
                MOV     R2, #0b01011011     // if KEY2 pressed, have R3 hold 7-segment code for digit 2
                LSL     R2, #16             // shift to HEX2 address
                ORR 	  R3, R2
          

// Show digit 3 on the HEX3 display when KEY3 is pressed, if pressed again display goes blank
KEY3:
                MOV     R2, #0x8            // copies the bit for KEY3 into R2
                ANDS    R2, R2, R1          // check for KEY3
                BEQ     END_KEY
                MOV     R2, #0b01001111     // if KEY3 pressed, have R3 hold 7-segment code for digit 3
                LSL     R2, #24             // shift to HEX3 address
                ORR 	  R3, R2

END_KEY:      
                STR   R3, [R5]              // show HEX displays
                POP   {R0-R12, LR}
                BX    LR
/*
 * Configure the Generic Interrupt Controller (GIC)
*/
                .global  CONFIG_GIC
CONFIG_GIC:
                PUSH     {LR}
                /* Enable the KEYs interrupts */
                MOV      R0, #73
                MOV      R1, #CPU0
                /* CONFIG_INTERRUPT (int_ID (R0), CPU_target (R1)); */
                BL       CONFIG_INTERRUPT

                /* configure the GIC CPU interface */
                LDR      R0, =0xFFFEC100        // base address of CPU interface
                /* Set Interrupt Priority Mask Register (ICCPMR) */
                LDR      R1, =0xFFFF            // enable interrupts of all priorities levels
                STR      R1, [R0, #0x04]
                /* Set the enable bit in the CPU Interface Control Register (ICCICR). This bit
                 * allows interrupts to be forwarded to the CPU(s) */
                MOV      R1, #1
                STR      R1, [R0]
    
                /* Set the enable bit in the Distributor Control Register (ICDDCR). This bit
                 * allows the distributor to forward interrupts to the CPU interface(s) */
                LDR      R0, =0xFFFED000
                STR      R1, [R0]    
    
                POP      {PC}
/* 
 * Configure registers in the GIC for an individual interrupt ID
 * We configure only the Interrupt Set Enable Registers (ICDISERn) and Interrupt 
 * Processor Target Registers (ICDIPTRn). The default (reset) values are used for 
 * other registers in the GIC
 * Arguments: R0 = interrupt ID, N
 *            R1 = CPU target
*/
CONFIG_INTERRUPT:
                PUSH     {R4-R5, LR}
    
                /* Configure Interrupt Set-Enable Registers (ICDISERn). 
                 * reg_offset = (integer_div(N / 32) * 4
                 * value = 1 << (N mod 32) */
                LSR      R4, R0, #3               // calculate reg_offset
                BIC      R4, R4, #3               // R4 = reg_offset
                LDR      R2, =0xFFFED100
                ADD      R4, R2, R4               // R4 = address of ICDISER
    
                AND      R2, R0, #0x1F            // N mod 32
                MOV      R5, #1                   // enable
                LSL      R2, R5, R2               // R2 = value

                /* now that we have the register address (R4) and value (R2), we need to set the
                 * correct bit in the GIC register */
                LDR      R3, [R4]                 // read current register value
                ORR      R3, R3, R2               // set the enable bit
                STR      R3, [R4]                 // store the new register value

                /* Configure Interrupt Processor Targets Register (ICDIPTRn)
                  * reg_offset = integer_div(N / 4) * 4
                  * index = N mod 4 */
                BIC      R4, R0, #3               // R4 = reg_offset
                LDR      R2, =0xFFFED800
                ADD      R4, R2, R4               // R4 = word address of ICDIPTR
                AND      R2, R0, #0x3             // N mod 4
                ADD      R4, R2, R4               // R4 = byte address in ICDIPTR

                /* now that we have the register address (R4) and value (R2), write to (only)
                 * the appropriate byte */
                STRB     R1, [R4]
    
                POP      {R4-R5, PC}

                .end   
