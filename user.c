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

int test_procedure (int results_array[2][6])
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
                    results_array[0][stage] = test(switch_config_A); // Configures switches for test
                    break;
                case 1:
                    results_array[0][stage] = test(switch_config_B); // Configures switches for test
                    break;
                case 2:
                    results_array[0][stage] = test(switch_config_C); // Configures switches for test
                    break;
                case 3:
                    results_array[0][stage] = test(switch_config_D); // Configures switches for test
                    break;
                case 4:
                    results_array[0][stage] = test(switch_config_E); // Configures switches for test
                    break;
                case 5:
                    results_array[0][stage] = test(switch_config_F); // Configures switches for test
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

void analyze_test (int tests[2][6])
{
    int i, j; //for counting
    int examine = 0; //generic container
    
    //Comparison_array contains the template values that the results SHOULD be
    int comparison_array[6] = {0x001F,0x002F,0x0037,0x003B,0x003D,0x003E}; //this is WITH extra diode
    //Foo is the test results
    int foo[2][6] = {{0x001F,0x000F,0x0007,0x0003,0x0001,0x0000}, //Row 0 = results from the test
                    {0, 0, 0, 0, 0, 0}};                          //Row 1 = negative polarity deduced from results
    
    //This set of loops deduces whether or not there is a break or if there is reverse polarity
    for (i=0; i<6;i++)
    {
        for (j=0; j<6;j++)
        {
            //led_out(j);
            //pause_flash();
            if (i == j) //skips loop if it is the loop to test itself
            {
                continue;
            }
            
            examine = (0x01 <<(5 - j));//these lines before the next if extract a bit to determine if there is a connection A->B
            //led_out(examine);
            //pause_flash();
            examine &= tests[0][i];
            //led_out(examine);
            //pause_flash();
            
            if (examine > 0)   //Is there A->B?
            {
                continue;
            }
            
            examine = (0x01 <<(5 - i)); //These lines determine if there is B->A
            //led_out(examine);
            //pause_flash();
            examine &= tests[0][j];
            //led_out(examine);
            //pause_flash();
            
            if (examine > 0)  //Is there B->A?
            {
            tests[1][i] |= (0x01 << (5 - j)); // Adds A bit to Neg Reg for C
            //led_out(tests[1][i]);
            //pause_flash();
            }
        }
    }
    
    //FIX B->A!!!!!!!!!!!!!!!!!!!!!!***************************************
    for (i=0;i<6;i++)
    {
        for(j=0;j<2;j++)
        {
            led_out(tests[j][i]);
            pause_flash();
        }
    }
}
