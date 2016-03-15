/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */
void testAB (void);
void testBA (void);
void testCD (void);
void testDC (void);
void delay (void);
void pause_flash (void);
void test_signal (void);
int analyze_test (int test_1,int test_2,int test_3,int test_4);


extern int test_stop;