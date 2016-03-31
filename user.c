/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif

#include <stdint.h>          /* For uint16_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include "user.h"            /* variables/params used by user.c               */

void InitApp(void)
{
    //*******I/O Config*****
    AD1PCFGL = 0xFFFF; // Set all analog pins to digital
    //LEDs
    TRISA = 0x00; //Set Port A to output
    //SIGNAL & Switch Control
    TRISB = 0x00A0; // Set port B as output, except RB5 (signal in) and 7 (EXINT0)
    
    //External Interrupt 0
    INTCON2bits.INT0EP = 0; //EXT0 Rising edge
	IFS0bits.INT0IF = 0; 	//Ensure EXT0 flag is 0
	IEC0bits.INT0IE = 1; 	//Enable EXT0
    
    //*******TIMER 1*********
    T1CON = 0x0;  //timer Setup
    T1CONbits.TCKPS = 0x3; //set prescaler
    TMR1 = 0;            //Set timer value to 0
    PR1 = 2000;          //Period value for comparison
    IFS0bits.T1IF	= 0; //ensure flag is off
    IEC0bits.T1IE	= 1; //enable timer1 interrupts
    
    delay(); // Gives Switches and MCU time to settle
    
}

int test (int config[5]) //Performs test for given line
{
    int i = 0;
    int results = 0;
    
    //The following loop fills a variable with the single bit results
    //of the tests for a given line. It bit shifts the results of the old
    //test results left, creating a new bit slot to be filled with the latest
    //test result. Repeat until the least 5 significant bits of the variable
    //are the individual results of each test.
    for (i = 0; i < 5; i++)
        {
            LATB = config[i]; //pin config for desired test switch configuration
            delay(); // To settle switches
            results <<= 1; //Create new bit "slot" for test result
            results |= PORTBbits.RB5; //Store test result
        }
    return results;
}

void delay(void)
{
    int i = 0;
    for (i=0; i<25000; i++)
    {
        Nop();
    }
}

void pause_flash (void)
{
    test_stop = 1;
    while(test_stop == 1)
    {
        LATA ^=0x0F;
        delay();
    }
    delay();
}

void analyze_test (int test_1,int test_2,int test_3,int test_4,int test_5,int test_6)
{
    //Compares test results with correct template, if equal then blink LED
    if (test_1 == 0x0012 && test_2 == 0x0002)
    {
        LATAbits.LATA0 ^= 1;
        delay();
    }
    else
    {
        LATAbits.LATA4 ^= 1;
        delay();
        LATAbits.LATA4 ^= 1;
    }

    if (test_3 == 0x000A && test_4 == 0x000E)
    {
        LATAbits.LATA1 ^= 1;
            delay();
    }
    else
    {
        LATAbits.LATA4 ^= 1;
        delay();
        LATAbits.LATA4 ^= 1;
    }

    if (test_5 == 0x0000 && test_6 == 0x000F)
    {
        LATAbits.LATA2 ^= 1;
            delay();
    }
    else
    {
        LATBbits.LATB15 ^= 1;
        delay();
        LATBbits.LATB15 ^= 1;
    }
}
