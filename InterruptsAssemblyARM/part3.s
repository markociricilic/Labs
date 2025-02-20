               .equ      EDGE_TRIGGERED,    0x1
               .equ      LEVEL_SENSITIVE,   0x0
               .equ      CPU0,              0x01    // bit-mask; bit 0 represents cpu0
               .equ      ENABLE,            0x1

               .equ      KEY0,              0b0001
               .equ      KEY1,              0b0010
               .equ      KEY2,              0b0100
               .equ      KEY3,              0b1000

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

/* ********************************************************************************
 * This program demonstrates use of interrupts with assembly code. The program 
 * responds to interrupts from a timer and the pushbutton KEYs in the FPGA.
 *
 * The interrupt service routine for the timer increments a counter that is shown
 * on the red lights LEDR by the main program. The counter can be stopped/run by 
 * pressing any of the KEYs.
 ********************************************************************************/
                .text
                .global  _start
_start:                                         
/* Set up stack pointers for IRQ and SVC processor modes */
                  MSR	   CPSR_c, #0b11010010 		// interrupts masked (off), MODE = IRQ
				          LDR	   SP, =0x20000           // set IRQ stack pointer
				
                  MSR	   CPSR_c, #0b11010011	  // interrupts masked, MODE = Supervisor (SVC)						
                  LDR	   SP, =0x40000				    // set supervisor mode (SVC) stack 

                  BL       CONFIG_GIC           // configure the ARM generic interrupt controller
                  BL       CONFIG_PRIV_TIMER    // configure A9 Private Timer
                  BL       CONFIG_KEYS          // configure the pushbutton KEYs port

                  // enable IRQ interrupts in the processor
                  MSR     CPSR_c, #0b01010011         // IRQ unmasked (enabled), MODE = SVC

                  /* Enable IRQ interrupts in the ARM processor */
                  LDR      R5, =0xFF200000      // LEDR base address
LOOP:                                          
                  LDR      R3, COUNT            // global variable
                  STR      R3, [R5]             // write to the LEDR lights
                  B        LOOP                
          
/* Global variables */
                .global  COUNT
COUNT:          .word    0x0                  // used by timer
                .global  RUN
RUN:            .word    0x1                  // initial value to increment COUNT

/* Configure the A9 Private Timer to create interrupts at 0.25 second intervals */
CONFIG_PRIV_TIMER:                             
                LDR		  R0, =0xFFFEC600			// A9 Private Timer base address
                LDR     R1, =50000000       // loading in value to start decrementing initially (200M * 0.25 sec)
                STR     R1, [R0]            // storing the value 50M into the A9 Private Timer base address

                MOV     R1, #0b111          // setting interrupt, automatic, and enable to 1
                STR     R1, [R0, #0x8]      // storing the above bit code into the Timer's control register 
                MOV     PC, LR
            
/* Configure the pushbutton KEYS to generate interrupts */
CONFIG_KEYS:                          
                LDR		  R0, =0xFF200050				// pushbutton KEY base address
				        MOV		  R1, #0xF				      // set interrupt mask bits
				        STR		  R1, [R0, #0x8]				// interrupt mask register is (base + 8)       
                MOV     PC, LR 

/*--- IRQ ---------------------------------------------------------------------*/
IRQ_HANDLER:
                PUSH     {R0-R12, LR}
    
                /* Read the ICCIAR in the CPU interface */
                LDR      R4, =0xFFFEC100
                LDR      R5, [R4, #0x0C]         // read the interrupt ID

                CMP      R5, #73                 // checking if the interrupt was from KEYS and branching if true
                BLEQ     KEY_ISR      
                CMP      R5, #29                 // checking if the interrupt was from TIMER and branching if true
                BLEQ     PRIV_TIMER_ISR      

                /* Write to the End of Interrupt Register (ICCEOIR) */
                STR      R5, [R4, #0x10]
    
                POP      {R0-R12, LR}
                SUBS     PC, LR, #4

/****************************************************************************************
 * Pushbutton - Interrupt Service Routine                                
 *                                                                          
 * This routine toggles the RUN global variable.
 ***************************************************************************************/
                .global  KEY_ISR
KEY_ISR:        
                PUSH     {R0-R7, LR}
                LDR      R0, =0xFF200050   // base address of pushbutton KEY port
                LDR      R1, [R0, #0xC]    // read edge capture register
                
                MOV      R2, #0x1
                ANDS     R2, R2, R1          // check for KEY0
                BLNE     KEY_0  
                MOV      R2, #0x2
                ANDS     R2, R2, R1          // check for KEY1
                BLNE     KEY_1         
                MOV      R2, #0x4
                ANDS     R2, R2, R1          // check for KEY2
                BLNE     KEY_2
                                            // no need to check for KEY3 since it is not needed
                POP      {R0-R7, LR}
                MOV      PC, LR

KEY_0:
                PUSH     {R0-R7, LR}
                LDR      R2, RUN            // loading the value of RUN into R2 (0x1 initially)
                CMP      R2, #0             // comparing the current RUN with 0
                MOVEQ    R2, #1             // if it is equal to 0, move 1 into it
                MOVNE    R2, #0             // if it is not equal to 0, means must be 1 so move 0 into it

                LDR      R3, =RUN           // loading the address of RUN into R3
                STR      R2, [R3]           // storing the RUN value into the address of R3 stopping the increment of COUNT
                STR      R1, [R0, #0xC]     // and clearing the interrupt

                POP      {R0-R7, LR}
                MOV      PC, LR
KEY_1:
                PUSH     {R0-R7, LR}
                LDR      R1, =0xFFFEC600    // loading the memory mapped address of the A9 Private Timer into R1
                LDR      R3, [R1]           // loading the above into R3 as a "temp" register

                LSR      R3, #1             // logic shifting right by 1 bit, is equivalent to multiplying COUNT rate by 2
                STR      R3, [R1]           // storing this new rate into R1

                MOV      R1, #0b111         // Set interrupt, automatic, enable to 1
                STR      R1, [R0, #0x8]     // storing the above bit code into the Timer's control register 

                STR      R1, [R0, #0xC]    // clear the interrupt
                POP      {R0-R7, LR}
                MOV      PC, LR      
KEY_2:              
                // exactly identitical logic as with KEY_1, but logic shifting left by 1 bit to divide COUNT rate by 2
                PUSH     {R0-R7, LR}
                LDR      R1, =0xFFFEC600
                LDR      R3, [R1]

                LSL      R3, #1
                STR      R3, [R1]

                MOV      R1, #0b111           
                STR      R1, [R0, #0x8]

                STR      R1, [R0, #0xC]    
                POP      {R0-R7, LR}
                MOV      PC, LR  

/******************************************************************************
 * A9 Private Timer interrupt service routine
 *                                                                          
 * This code toggles performs the operation COUNT = COUNT + RUN
 *****************************************************************************/
                .global    TIMER_ISR
PRIV_TIMER_ISR:
                PUSH     {R0-R7, LR} 
                LDR      R0, COUNT          // loading the COUNT value into R0
                LDR      R1, RUN            // loading the RUN value into R1
                ADD      R2, R1, R0         // AND-ing the two values and storing in R2 (if both equal 1, store 1 into R2)

                LDR      R3, =COUNT         // loading the address of COUNT into R3
                STR      R2, [R3]           // storing the result of the AND operation into R3

                LDR      R0, =0xFFFEC600	// loading the memory mapped address of the A9 Private Timer into R1
                LDR      R1, [R0, #0xC]     // read edge capture register
                STR		   R1, [R0, #0xC]     // clear the interrupt

                POP      {R0-R7, LR}
                MOV      PC, LR
/* 
 * Configure the Generic Interrupt Controller (GIC)
*/
                .global  CONFIG_GIC
CONFIG_GIC:
                PUSH     {LR}
                MOV      R0, #29
                MOV      R1, #CPU0
                BL       CONFIG_INTERRUPT
                
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
