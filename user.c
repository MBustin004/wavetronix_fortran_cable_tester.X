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
    
    //SIGNAL & Enables
    TRISB = 0x00A0; // Set port B as output, except RB5 (signal in) and 7 (EXINT0)
    
    //External interrupt 0
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
    
}

int test (int procedure[5]) // setup switches for AB test
{
    int i = 0;
    int results = 0;
    for (i = 0; i < 5; i++)
        {
            //Switch Setups
            LATB = procedure[i]; // TEST AB, turns off pin 11 signal
            //Signal Setup
            LATBbits.LATB4 = 1; //signal on pin 11
            delay();
            //pause_flash(); // flashes LEDS to show non-testing phase
            if (i>0)
            {
                results <<= 1;
            }
            results |= PORTBbits.RB5;
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
    //test_stop = 1;
    while(test_stop == 1)
    {
        LATA ^=0x1F;
        LATBbits.LATB15 ^= 1;
        delay();
    }
    delay();
}

void test_signal (void)
{
    //test_stop = 1;
    while (test_stop == 1) //Blinks light if input is correct
        {
            if (PORTBbits.RB5 == 0) //Pin 14
                {
                    T1CONbits.TON = 1; // turn on timer 1
                }
        }
}

void analyze_test (int test_1,int test_2,int test_3,int test_4,int test_5,int test_6)
{
        //test_stop =1;
        //while (test_stop == 1)
        //{
            if (test_1 == 0x0010 && test_2 == 0x0000)
            {
                LATBbits.LATB14 ^= 1;
                delay();
            }
            
            if (test_3 == 0x0000 && test_4 == 0x0004)
            {
                LATBbits.LATB13 ^= 1;
                delay();
            }
        //}
}