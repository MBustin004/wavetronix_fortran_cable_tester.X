/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */
int test (int procedure[5]);
void delay (void);
void pause_flash (void);
void analyze_test (int test_1,int test_2,int test_3,int test_4,int test_5,int test_6);


extern int test_stop;