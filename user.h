/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */
void testA (int results[5]);
void testB (int results[5]);
void testC (int results[5]);
void testD (int results[5]);
void delay (void);
void pause_flash (void);
void test_signal (void);
void analyze_test (int test_1[5],int test_2[5],int test_3[5],int test_4[5]);


extern int test_stop;