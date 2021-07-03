#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "thread.h"
#include "timex.h"
#include "xtimer.h"
#include "shell.h"

#include "mutex.h"

/* Add lps331ap related include here */
/*Atmospheric pressure and temperature – LPS331AP*/
#include "lps331ap.h"
#include "lps331ap_params.h"
#include "lpsxxx.h"
#include "lpsxxx_params.h"
/* Add lsm303dlhc related include here */
/*LSM303DLHC 3D accelerometer/magnetometer */
#include  "lsm303dlhc.h"
#include  "lsm303dlhc_params.h"

/* Declare the lps331ap device variable here */
/*static lps331ap_t lps331ap;*/

static lpsxxx_t lpsxxx;

/* Declare the lsm303dlhc device variable here */

static lsm303dlhc_t lsm303dlhc;

/* Declare and initialize the lsm303dlhc thread lock here */


/* stack memory allocated for the lsm303dlhc thread */
static char lsm303dlhc_stack[THREAD_STACKSIZE_MAIN];

static void *lsm303dlhc_thread(void *arg)
{
    (void)arg;

    while (1) {
        /* Acquire the mutex here */


        /* Read the accelerometer/magnetometer values here */
    int pres = lps331ap_read_pres(&dev);
    int temp = lps331ap_read_temp(&dev);

        /* Release the mutex here */


        xtimer_usleep(500 * US_PER_MS);
    }

    return 0;
}

static void _lsm303dlhc_usage(char *cmd)
{
    printf("usage: %s <start|stop>\n", cmd);
}

static int lsm303dlhc_handler(int argc, char *argv[])
{
    if (argc < 2) {
        _lsm303dlhc_usage(argv[0]);
        return -1;
    }

    /* Implement the lsm303dlhc start/stop subcommands here */


    return 0;
}

static void _lpsxxx_usage(char *cmd)
{
    printf("usage: %s <temperature|pressure>\n", cmd);
}

static int lpsxxx_handler(int argc, char *argv[])
{
    if (argc < 2) {
        _lpsxxx_usage(argv[0]);
        return -1;
    }

    /* Implement the lps331ap temperature/pressure subcommands here */

    if (!strcmp(argv[1], "temperature")) {
        int16_t temp = 0;
        lpsxxx_read_temp(&lpsxxx, &temp);
        printf("Temperature: %i.%u°C\n", (temp / 100), (temp % 100));
    }
    else if (!strcmp(argv[1], "pressure")) {
        uint16_t pres = 0;
        lpsxxx_read_pres(&lpsxxx, &pres);
        printf("Pressure: %uhPa\n", pres);
    }
    else {
        _lpsxxx_usage(argv[0]);
        return -1;
    }
    return 0;
}

static const shell_command_t commands[] = {
    /* lsm303dlhc shell command handler */
    { "lsm", "start/stop reading accelerometer values", lsm303dlhc_handler },

    /* Add the lps331ap command description here */

   { "lps", "read the lps331ap values", lpsxxx_handler },
    { NULL, NULL, NULL}
};

int main(void)
{
    /* Initialize the lps331ap sensor here */
    lpsxxx_init(&lpsxxx, &lpsxxx_params[0]);
    /*lps331ap_t dev1;
    puts("Initializing LPS331AP sensor");
    if (lps331ap_init(&dev1, &lps331ap_params[0]) == 0) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
    return 1;
    }
*/
    /* Initialize the lsm303dlhc sensor here */
    lsm303dhlc_t dev2;
    puts("Initializing LSM303DHLC sensor");
    if (lsm303dhlc_init(&dev2, &lsm303dhlc_params[0]) == 0) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
    return 1;
    }

    thread_create(lsm303dlhc_stack, sizeof(lsm303dlhc_stack), THREAD_PRIORITY_MAIN - 1,
                  0, lsm303dlhc_thread, NULL, "lsm303dlhc");

    /* Everything is ready, let's start the shell now */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
