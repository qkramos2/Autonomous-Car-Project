/* 
   --------------------------------------------------------------
 * Name:    Quentin Ramos II    
 * File:    uart.h
 * Purpose: Provide function prototypes
   --------------------------------------------------------------
*/
 
// Include Statements:

#include "Common.h"
 
// ----- Function Prototypes ----- // 

// UART0 Prototypes:

void uart0_init(int clock_speed);
char uart0_getchar(void);
void uart0_putchar(char ch);
void uart0_put(char *ptr_str);
BOOLEAN uart0_dataAvailable(void);

// UART2 Prototypes:

void uart2_init(int clock_speed);
char uart2_getchar(void);
void uart2_putchar(char ch);
void uart2_put(char *ptr_str);
BOOLEAN uart2_dataAvailable(void);
