/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */
int test_procedure (int results_array [6]);
int test (int config[6]);
void delay (void);
void pause_flash (void);
void led_out(int out);
void analyze_test (int tests[6]);

extern int test_stop;