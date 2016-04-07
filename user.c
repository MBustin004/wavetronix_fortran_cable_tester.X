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
#include "p33FJ64GP802.h"            /* variables/params used by user.c       */

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
    int switch_config_A[6] = {0x0000,0x0009,0x0101,0x0401,0x1001,0x4001};
    int switch_config_B[6] = {0x0006,0x0000,0x0104,0x0404,0x1004,0x4004};
    int switch_config_C[6] = {0x0042,0x0048,0x0000,0x0440,0x1040,0x4040};
    int switch_config_D[6] = {0x0202,0x0208,0x0300,0x0000,0x1200,0x4200};
    int switch_config_E[6] = {0x0802,0x0808,0x0900,0x0C00,0x0000,0x4800};
    int switch_config_F[6] = {0x2002,0x2008,0x2100,0x2400,0x3000,0x0000};
    
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

int test (int config[6]) //Performs test for given line
{
    int i = 0;
    int results = 0;
    
    //The following loop fills a variable with the single bit results
    //of the tests for a given line. It bit shifts the results of the old
    //test results left, creating a new bit slot to be filled with the latest
    //test result. Repeat until the least 5 significant bits of the variable
    //are the individual results of each test.
    for (i = 0; i < 6; i++)
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
    }
    delay();
}

void led_out(int out)
{
    LATBbits.LATB15 = (0x0001 & out);
    LATAbits.LATA4 = (0x0002 & out) >> 1;
    LATBbits.LATB4 = (0x0004 & out) >> 2;
    LATAbits.LATA2 = (0x0008 & out) >> 3;
    LATAbits.LATA1 = (0x0010 & out) >> 4;
    LATAbits.LATA0 = (0x0020 & out) >> 5;
}

void analyze_test (int tests[6])
{
    int i = 0; //for counting
    int j = 0; //for counting
    int examine[6] = {0}; //generic container
    int guilty_threshold = 1; //variable for determining guilty pins, starts at one to ignore 0 votes
    int guilty_threshold2= 1;
    int polls[6] = {0};
    
    //Comparison_array contains the template values that the results SHOULD be
    int comparison_array[6] = {0x0012,0x0002,0x0012,0x001A,0x0000,0x001E}; //this is WITH extra diode
    //int comparison_array[6] = {0x0012,0x002,0x0000,0x0008,0x0000,0x000C}; // this is with only 4 diodes
    //Foo is the test results
    //int foo[6] = {0x0002,0x0022,0x0022,0x002A,0x0000,0x002E};
    
    // This XORs the results and the templates, problems are high
    for (i = 0; i<6; i++) 
    {
        comparison_array[i] ^= tests[i];
        led_out(tests[i]);
        pause_flash();
    }
    
    //BEGIN POLLING ALGORITHM
    for (i=0; i<6; i++) // Loop for each set of tests
    {
        for (j=5; j>(-1); j--) // Loop to sum number of flagged errors
        {
            examine[0] = 0x0001;
            examine[0] <<= j;
            examine[0] &= comparison_array[i];
            examine[0] >>= j;
            polls[(5-j)] += examine[0];
        }
    }
    
    //Determine most votes
    examine[0] = 0;
    examine[1] = 0;
    guilty_threshold = 1; //variable for determining guilty pins, starts at one to ignore 0 votes
    guilty_threshold2= 1;
    for (i=0; i<6; i++)
    {
        if ((polls[i] >= guilty_threshold) || (polls[i] >= guilty_threshold2))
        {
            if (guilty_threshold >= guilty_threshold2)
            {
                examine[1] = examine[0];
                guilty_threshold2 = guilty_threshold;
            }
            examine[0] = (i+1); //must be i+1 because a 0 in examine means nothing is wrong (1=A, 2=B, etc.)
            guilty_threshold = polls[i];
        }
    }

    //turns on LATA based on analysis
    if ((examine[0] > 0) || (examine[1] > 0))
    {
        examine[2] = (0x0001 << (examine[0]-1));
        examine[3] = (0x0001 << (examine[1]-1));
        examine[2] |= examine[3];
        LATA = examine[2];
        //pause_flash();
        delay();
        delay();
        delay();
        delay();
    }
}
