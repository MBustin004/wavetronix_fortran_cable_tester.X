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
int test_stop = 0; //Global for manual advancement through program
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{
    int data [2][6] = {{0}};
    //*****INIT*****
    ConfigureOscillator(); // Generic MCU setup
    InitApp(); //Init MCU Interrupts and I/O
    
    //*****Main Loop*****
    while(1) 
    {
        LATB = 0x0; // Turns off switches and LEDS for a fresh start
        LATA = 0x0;
        
        //*****Selects what test to perform*****
        test_procedure(data);
        delay(); //Gives MCU and switches time to settle
        
        //*****Analyzing the results of the tests performed*****
        analyze_test(data);
    }
}
