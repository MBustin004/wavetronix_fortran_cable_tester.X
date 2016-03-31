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
    int switch_config_A[5] = {0x0009,0x0101,0x0401,0x1001,0x4001};
    int switch_config_B[5] = {0x0006,0x0104,0x0404,0x1004,0x4004};
    int switch_config_C[5] = {0x0042,0x0048,0x0440,0x1040,0x4040};
    int switch_config_D[5] = {0x0202,0x0208,0x0300,0x1200,0x4020};
    int switch_config_E[5] = {0x0802,0x0808,0x0900,0x0C00,0x4800};
    int switch_config_F[5] = {0x2002,0x2008,0x2100,0x2400,0x3000};
    stage = 0;  //Selects which wire pair to test
    
    //INIT
    ConfigureOscillator();
    InitApp(); //Init MCU Interrupts and I/O
    
    while(1)
    {
        LATB = 0x0;
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
                break;
            case 3:
                results_D = test(switch_config_D); // Configures switches for test
                stage ++;
                break;
            case 4:
                results_E = test(switch_config_E); // Configures switches for test
                stage ++;
                break;
            case 5:
                results_F = test(switch_config_F); // Configures switches for test
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
