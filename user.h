/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */
//This structure is for convenience of handling the conclusions that we will deduce
struct BadStuff {int no_connection; int swapped_wires;};

void InitApp(void);         /* I/O and Peripheral Initialization */
void test_procedure (int results_array[2][6]);
int test (int config[6]);
void prep_neg(int samples[2][6]);
void delay (void);
void pause (void); // this is to pause code for debugging
void pause_and_show (struct BadStuff lights); // this is to pause code to show the breaks/swaps
void led_out(int out);
void analyze_test (int tests[2][6]);
struct BadStuff detect_problems(int results[2][6]);

extern int test_stop;