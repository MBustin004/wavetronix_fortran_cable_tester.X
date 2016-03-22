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
    int results_A[5]; //Results of test AB
    int results_B[5]; //Results of test BA
    int results_C[5];
    int results_D[5];
    stage = 0;  //Selects which wire pair to test
    
    //INIT
    ConfigureOscillator();
    InitApp(); //Init MCU Interrupts and I/O
    
    //LED Activation
    LATA = 0x1F;  //Port A high for LED output Pins 1-5
    LATBbits.LATB15 = 1; // RB15 LED output, pin 26
    
    while(1)
    {
        switch (stage) //Selects what test to perform
        {
            case 0:
                testA(results_A); // Configures switches for test
                stage ++;
                break;
            case 1:
                testB(results_B); // Configures switches for test
                stage ++;
                break;
            case 2:
                testC(results_C); // Configures switches for test
                stage ++;
            case 3:
                testD(results_D); // Configures switches for test
                stage ++;
                break;
            default:
                stage = 0;
                break;
        }
        
        //test_signal(); //Holds test procedure to visually confirm functioning switches
        
        analyze_test(results_A, results_B, results_C, results_D); //Analyze results, RED LED blink if good
    }
}
