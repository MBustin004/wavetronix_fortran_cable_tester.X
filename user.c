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
#include "user.h"
#include "p33FJ64GP802.h"            /* variables/params used by user.c               */

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

int test_procedure (int results_array[6])
{
    int stage = 0; //for selecting wire pair to test
    
    //switch_config_* each contains the I/O pin configurations for each test
    int switch_config_A[5] = {0x0009,0x0101,0x0401,0x1001,0x4001};
    int switch_config_B[5] = {0x0006,0x0104,0x0404,0x1004,0x4004};
    int switch_config_C[5] = {0x0042,0x0048,0x0440,0x1040,0x4040};
    int switch_config_D[5] = {0x0202,0x0208,0x0300,0x1200,0x4020};
    int switch_config_E[5] = {0x0802,0x0808,0x0900,0x0C00,0x4800};
    int switch_config_F[5] = {0x2002,0x2008,0x2100,0x2400,0x3000};
    
    for (stage = 0; stage < 6; stage ++)
        {
            switch (stage) 
            {
                case 0:
                    results_array[stage] = test(switch_config_A); // Configures switches for test
                    break;
                case 1:
                    results_array[stage] = test(switch_config_B); // Configures switches for test
                    break;
                case 2:
                    results_array[stage] = test(switch_config_C); // Configures switches for test
                    break;
                case 3:
                    results_array[stage] = test(switch_config_D); // Configures switches for test
                    break;
                case 4:
                    results_array[stage] = test(switch_config_E); // Configures switches for test
                    break;
                case 5:
                    results_array[stage] = test(switch_config_F); // Configures switches for test
                    break;
                default:
                    stage = 0;
                    break;
            }
        }
    return results_array;
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

void analyze_test (int tests[6])
{
    int i = 0;
    int examine = 0;
    //Comparison_array contains the template values that the results SHOULD be
    int comparison_array[6] = {0x0012,0x0002,0x000A,0x000E,0x0000,0x000F};
    int foo[6] = {0x0012,0x0002,0x000A,0x000E,0x0000,0x000F};
    int foo_results[6] = {0x0002,0x0012,0x0012,0x0016,0x0000,0x0017};
    
    for (i = 0; i<6; i++)
    {
        foo[i] ^= foo_results[i];
    }
    
    //Is A correct?
    examine = foo[1];
    examine &= 0x0010;
    if (examine == 1)
    {
        LATAbits.LATA4 ^= 1;
    }
    
    /*//This loop determines if there are any bits that are out of order by clearing out the matching pairs
    for (i = 0; i<6; i++)
    {
        comparison_array[i] ^= tests[i];
    }
    
    //Compares test results with correct template, if equal then blink LED
    if (comparison_array[0] == 0 && comparison_array[1] == 0)
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

    if (comparison_array[2] == 0 && comparison_array[3] == 0)
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

    if (comparison_array[4] == 0 && comparison_array[5] == 0)
    {
        LATAbits.LATA2 ^= 1;
            delay();
    }
    else
    {
        LATBbits.LATB15 ^= 1;
        delay();
        LATBbits.LATB15 ^= 1;
    }*/
}
