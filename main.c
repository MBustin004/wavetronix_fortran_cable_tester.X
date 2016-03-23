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
    int results_A = 0;
    int results_B = 0;
    int results_C = 0;
    int results_D = 0;
    int results_E = 0;
    int results_F = 0;
    int switch_config_A[5] = {0x0009,0x0081,0x0201,0x0000,0x0000};
    int switch_config_B[5] = {0x0006,0x0084,0x0204,0x0000,0x0000};
    int switch_config_C[5] = {0x0042,0x0048,0x0440,0x0000,0x0000};
    int switch_config_D[5] = {0x0202,0x0208,0x0300,0x0000,0x0000};
    int switch_config_E[5] = {0x0000,0x0000,0x0000,0x0000,0x0000};
    int switch_config_F[5] = {0x0000,0x0000,0x0000,0x0000,0x0000};
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
                results_A = test(switch_config_A); // Configures switches for test
                stage ++;
                break;
            case 1:
                results_B = test(switch_config_B); // Configures switches for test
                stage ++;
                break;
            case 2:
                results_C = test(switch_config_C); // Configures switches for test
                stage ++;
            case 3:
                results_D = test(switch_config_D); // Configures switches for test
                stage ++;
                break;
            default:
                stage = 0;
                break;
        }
        //test_signal(); //Holds test procedure to visually confirm functioning switches
        delay();
        analyze_test(results_A, results_B, results_C, results_D, results_E, results_F); //Analyze results, RED LED blink if good
    }
}
