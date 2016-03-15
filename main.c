/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

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


#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <p33FJ64GP802.h>
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/
int stage = 0; //for selecting wire pair to test
int test_stop = 0; //Global for manual advancement through program
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{
    //Variable Setup
    int AB = 0; //Results of test AB
    int BA = 1; //Results of test BA
    int CD = 1;
    int DC = 0;
    int result = 0;
    stage = 0;  //Selects which wire pair to test
    
    //INIT
    ConfigureOscillator();
    InitApp(); //Init MCU Interrupts and I/O
    
    //LED Activation
    LATA = 0x1F;  //Port A high for LED output Pins 1-5
    LATBbits.LATB15 = 1; // RB15 LED output, pin 26
    
    //Signal Setup
    LATBbits.LATB4 = 1; //signal on pin 11
    
    while(1)
    {
        switch (stage) //Selects what test to perform
        {
            case 0:
                testAB(); // Configures switches for test
                pause_flash(); // flashes LEDS to show non-testing phase
                AB = PORTBbits.RB5;
                stage ++;
                break;
            case 1:
                testBA(); // Configures switches for test
                pause_flash(); // flashes LEDS to show non-testing phase
                BA = PORTBbits.RB5;
                stage ++;
                break;
            case 2:
                testCD(); // Configures switches for test
                pause_flash(); // flashes LEDS to show non-testing phase
                CD = PORTBbits.RB5;
                stage ++;
                break;
            case 3:
                testDC(); // Configures switches for test
                pause_flash(); // flashes LEDS to show non-testing phase
                DC = PORTBbits.RB5;
                stage ++;
                break;
            default:
                stage = 0;
                break;
        }
        
        test_signal(); //Holds test procedure to visually confirm functioning switches
        
        result = analyze_test(AB, BA, CD, DC); //Analyze results, RED LED blink if good
    }
}
