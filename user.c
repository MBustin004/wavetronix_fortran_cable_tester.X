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

void test_procedure (int results_array[2][6])
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
                    results_array[1][stage] = 0; //Clears negative polarity data from previous test procedure
                    break;
                case 1:
                    results_array[0][stage] = test(switch_config_B);
                    results_array[1][stage] = 0;
                    break;
                case 2:
                    results_array[0][stage] = test(switch_config_C);
                    results_array[1][stage] = 0;
                    break;
                case 3:
                    results_array[0][stage] = test(switch_config_D);
                    results_array[1][stage] = 0;
                    break;
                case 4:
                    results_array[0][stage] = test(switch_config_E);
                    results_array[1][stage] = 0;
                    break;
                case 5:
                    results_array[0][stage] = test(switch_config_F);
                    results_array[1][stage] = 0;
                    break;
                default:
                    stage = 0;
                    break;
            }
        }
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

void pause (void)
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
    int conclusion = 0;
    //Foo is a sample set of test results for testing algorithm
    int foo[2][6] = {{0x001F,0x000F,0x0007,0x0003,0x0001,0x0000}, //Row 0 = results from the test
                    {0, 0, 0, 0, 0, 0}};                         //Row 1 = negative polarity deduced from results
    
    led_out(0x3F); // Stop point for visual testing
    pause();
    
    //First we deduce negative polarity for conclusions
    prep_neg(tests);
    //Identifies any broken connections (no connections to anything)
    conclusion = detect_problems(tests);
    led_out(conclusion);
    pause();
}

void prep_neg(int samples[2][6])
{
    int i, j;
    int examine = 0;
    
    //This set of loops deduces if there is reverse polarity
    for (i=0; i<6;i++)
    {
        for (j=0; j<6;j++)
        {
            if (i == j) //skips loop if it is the loop to test itself
            {
                continue;
            }
            examine = (0x01 <<(5 - j));//extract a bit to determine if there is a connection A->B
            examine &= samples[0][i];
            if (examine > 0)   //Is there A->B?
            {
                continue;
            }
            examine = (0x01 <<(5 - i)); //determine if there is B->A
            examine &= samples[0][j];
            if (examine > 0)  //Is there B->A?
            {
            samples[1][i] |= (0x01 << (5 - j)); //Adds A bit to Neg Reg for C
            }
        }
    }
}

int detect_problems(int results[2][6])
{
    int i, j = 0; // for counting
    int pos_num, neg_num = 0; // stores number of connections (pos/neg polarity) when detecting position
    int num_breaks = 0; // The total number of breaks
    int failures = 0; // The final conclusion as to which connections have problems
    int connection = 0; // If there is a connection, this will go high
    int examine = 0; // variable for selecting a bit to examine
    int breaks = 0; // ID of which connections are broken
    
    for (i=0;i<6;i++) // This loop identifies which connections are broken
    {
        connection = 0;
        
        for(j=0;j<6;j++)
        {
            if (i == j) //skips loop if it is the loop to test itself
            {
                continue;
            }
            //these lines extract a bit from pos/neg registers to determine if there is a connection
            examine = (0x01 << (5 - i));
            connection |= (results[0][j] & examine);
            connection |= (results[1][j] & examine);
        }
        if (connection == 0)
        {
            breaks |= examine;
        }
    }
    
    ///THE LINES FROM HERE DOWN CHECK FOR SWAPS....I WANTED 2 FUNCTIONS BUT C SAID NOOOOOO
    for (i=0;i<6;i++) // This checks to see if there are NO connections, returns same results
    {
        num_breaks += ((breaks & (0x01 << (5-i))) >> (5-i));
        if (num_breaks > 4)
        {
            return breaks;
        }
    }
    
    for (i=0; i<6; i++) // THIS DETECTS THE SWAPS
    {
        pos_num = 0;
        neg_num = 0;
        
        for(j=0; j<6; j++)
        {
            if (i == j) // don't asses A -> A
            {
                continue;
            }
            else if (((breaks & (0x01 << (5-j))) >> (5-j)) == 1) //ignore broken connection, just add it
            {
                if (i < j) // this if/elseif is to increase the appropriate pos/neg connection
                {
                    pos_num += 1;
                }
                else if (i > j)
                {
                    neg_num += 1;
                }
                
                continue;
            }
            else
            {
                pos_num += (((results[0][i] & (0x01 << (5 - j)))) >> (5 - j)); //finds positive connections
                neg_num += ((results[1][i] & (0x01 << (5 - j))) >> (5-j)); //same with neg
            }
        }
        
        if (pos_num != (5-i)) // Are there the right number of positive connections?
        {
            failures |= (0x01 << (5 - i));
        }

        if (neg_num != i) // Are there the right number of negative connections?
        {
            failures |= (0x01 << (5 - i));
        }
    }
    return failures;
}
