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

#include "lpsxxx.h"
#include "lpsxxx_params.h"

/* Add lsm303dlhc related include here */
/*LSM303DLHC 3D accelerometer/magnetometer */

#include  "lsm303dlhc.h"
#include  "lsm303dlhc_params.h"

/* Add isl29020 related include here*/
/*ISL29020 light sensor*/

#include “isl29020.h”
#include “isl29020_params.h”

/* Add l3g44200 related include here*/
/*l3g44200 gyroscope  sensor*/

#include “l3g4200d.h”
#include “l3g4200d_params.h”

/* Declare the lps331ap device variable here */
/*static lps331ap_t lps331ap;*/

static lpsxxx_t lpsxxx;

/* Declare the lsm303dlhc device variable here */

static lsm303dlhc_t lsm303dlhc;

/* Declare the isl29020 device variable here*/

static isl29020_t isl29020;

/*Declare the l3g4200d device variable here*/

static l3g4200d_t l3g4200d;


/* Declare and initialize the lsm303dlhc thread lock here */
mutex mu;

static char stack[THREAD_STACKSIZE_MAIN];

/* stack memory allocated for the lsm303dlhc, l3g4200d, isl29020,  lpsxxx thread */

static char lsm303dlhc_stack[THREAD_STACKSIZE_MAIN];
static char l3g4200d_stack[THREAD_STACKSIZE_MAIN];
static char isl29020_stack[THREAD_STACKSIZE_MAIN];
static char lpsxxx_stack[THREAD_STACKSIZE_MAIN];


static void *lsm303dlhc_thread(void *arg)
{
    (void)arg;
    
/* Add the lsm303dlhc sensor polling endless loop here */
    
    while (1) {
        
        /* Acquire the mutex here */
    
        mu.lock();

        /* Read the accelerometer/magnetometer values here */
      
        lsm303dlhc_3d_data_t mag_value;
        lsm303dlhc_3d_data_t acc_value;
        lsm303dlhc_read_acc(&lsm303dlhc, &acc_value);
        lsm303dlhc_read_mag(&lsm303dlhc, &mag_value);
    
        /*int acc = lsm303dlhc_read_acc*/ 
        
        printf("Accelerometer x: %i y: %i z: %i\n",
           acc_value.x_axis, acc_value.y_axis, acc_value.z_axis);
        printf("Magnetometer x: %i y: %i z: %i\n",
           mag_value.x_axis, mag_value.y_axis, mag_value.z_axis);
      
        xtimer_usleep(500 * US_PER_MS);
  
        /*  int pres = lpsxxx_read_pres(&dev1);
    int temp = lpsxxx_read_temp(&dev1);

*/        /* Release the mutex here */

        mu.unlock();
      
        xtimer_usleep(500 * US_PER_MS);
    }

    return 0;
}

static void *l3g4200d_thread(void *arg)
{
    (void)arg;
    
/* Add the lsm303dlhc sensor polling endless loop here */
    
    while (1) {
        
        /* Acquire the mutex here */
    
        mu.lock();

        /* Read the accelerometer/magnetometer values here */
      
        lsm303dlhc_3d_data_t mag_value;
        lsm303dlhc_3d_data_t acc_value;
        lsm303dlhc_read_acc(&lsm303dlhc, &acc_value);
        lsm303dlhc_read_mag(&lsm303dlhc, &mag_value);
    
        /*int acc = lsm303dlhc_read_acc*/ 
        
        printf("Accelerometer x: %i y: %i z: %i\n",
           acc_value.x_axis, acc_value.y_axis, acc_value.z_axis);
        printf("Magnetometer x: %i y: %i z: %i\n",
           mag_value.x_axis, mag_value.y_axis, mag_value.z_axis);
      
        xtimer_usleep(500 * US_PER_MS);
  
        /*  int pres = lpsxxx_read_pres(&dev1);
    int temp = lpsxxx_read_temp(&dev1);

*/        /* Release the mutex here */

        mu.unlock();
      
        xtimer_usleep(500 * US_PER_MS);
    }

    return 0;
}

static void *isl29020_thread(void *arg)
{
    (void)arg;
    
/* Add the lsm303dlhc sensor polling endless loop here */
    
    while (1) {
        
        /* Acquire the mutex here */
    
        mu.lock();

        /* Read the accelerometer/magnetometer values here */
      
        lsm303dlhc_3d_data_t mag_value;
        lsm303dlhc_3d_data_t acc_value;
        lsm303dlhc_read_acc(&lsm303dlhc, &acc_value);
        lsm303dlhc_read_mag(&lsm303dlhc, &mag_value);
    
        /*int acc = lsm303dlhc_read_acc*/ 
        
        printf("Accelerometer x: %i y: %i z: %i\n",
           acc_value.x_axis, acc_value.y_axis, acc_value.z_axis);
        printf("Magnetometer x: %i y: %i z: %i\n",
           mag_value.x_axis, mag_value.y_axis, mag_value.z_axis);
      
        xtimer_usleep(500 * US_PER_MS);
  
        /*  int pres = lpsxxx_read_pres(&dev1);
    int temp = lpsxxx_read_temp(&dev1);

*/        /* Release the mutex here */

        mu.unlock();
      
        xtimer_usleep(500 * US_PER_MS);
    }

    return 0;
}
static void *lpsxxx_thread(void *arg)
{
    (void)arg;
    
/* Add the lsm303dlhc sensor polling endless loop here */
    
    while (1) {
        
        /* Acquire the mutex here */
    
        mu.lock();

        /* Read the accelerometer/magnetometer values here */
      
        lsm303dlhc_3d_data_t mag_value;
        lsm303dlhc_3d_data_t acc_value;
        lsm303dlhc_read_acc(&lsm303dlhc, &acc_value);
        lsm303dlhc_read_mag(&lsm303dlhc, &mag_value);
    
        /*int acc = lsm303dlhc_read_acc*/ 
        
        printf("Accelerometer x: %i y: %i z: %i\n",
           acc_value.x_axis, acc_value.y_axis, acc_value.z_axis);
        printf("Magnetometer x: %i y: %i z: %i\n",
           mag_value.x_axis, mag_value.y_axis, mag_value.z_axis);
      
        xtimer_usleep(500 * US_PER_MS);
  
        /*  int pres = lpsxxx_read_pres(&dev1);
    int temp = lpsxxx_read_temp(&dev1);

*/        /* Release the mutex here */

        mu.unlock();
      
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
    
     if (!strcmp(argv[1], "temperature")) {
        int16_t temp = 0;
        lsm303dlhc_read_temp(&lsm303dlhc, &temp);
        printf("Temperature: %i.%u°C\n", (temp / 100), (temp % 100));
    }
    else if (!strcmp(argv[1], "magnetometer")) {
        uint16_t mag = 0;
        lsm303dlhc_read_mag(&lsm303dlhc, &mag);
        printf("Magnetometer: %uhPa\n", mag);
    }
    else if (!strcmp(argv[1], "accelerometer")) {
        uint16_t acc = 0;
        lsm303dlhc_read_acc(&lsm303dlhc, &acc);
        printf("Accelerometer: %uhPa\n", acc);
    }
    else {
        _lsm303dlhc_usage(argv[0]);
        return -1;
    }

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

static void _isl29020_usage(char *cmd)
{
    printf("usage: %s <start|stop>\n", cmd);
}

static int isl29020_handler(int argc, char *argv[])
{
    if (argc < 2) {
        _isl29020_usage(argv[0]);
        return -1;
    }

    /* Implement the isl29020 start/stop subcommands here */
    
     if (!strcmp(argv[1], "temperature")) {
        int16_t temp = 0;
        isl29020_read_temp(&isl29020, &temp);
        printf("Temperature: %i.%u°C\n", (temp / 100), (temp % 100));
    }
    else if (!strcmp(argv[1], "magnetometer")) {
        uint16_t mag = 0;
        isl29020_read_mag(&isl29020, &mag);
        printf("Magnetometer: %uhPa\n", mag);
    }
    else if (!strcmp(argv[1], "accelerometer")) {
        uint16_t acc = 0;
        isl29020_read_acc(&isl29020, &acc);
        printf("Accelerometer: %uhPa\n", acc);
    }
    else {
        _isl29020_usage(argv[0]);
        return -1;
    }

    return 0;
}

static void _l3g4200d_usage(char *cmd)
{
    printf("usage: %s <start|stop>\n", cmd);
}

static int l3g4200d_handler(int argc, char *argv[])
{
    if (argc < 2) {
        _l3g4200d_usage(argv[0]);
        return -1;
    }

    /* Implement the l3g4200d start/stop subcommands here */
    
     if (!strcmp(argv[1], "temperature")) {
        int16_t temp = 0;
        l3g4200d_read_temp(&l3g4200d, &temp);
        printf("Temperature: %i.%u°C\n", (temp / 100), (temp % 100));
    }
    else if (!strcmp(argv[1], "magnetometer")) {
        uint16_t mag = 0;
        l3g4200d_read_mag(&l3g4200d, &mag);
        printf("Magnetometer: %uhPa\n", mag);
    }
    else if (!strcmp(argv[1], "accelerometer")) {
        uint16_t acc = 0;
        l3g4200d_read_acc(&l3g4200d, &acc);
        printf("Accelerometer: %uhPa\n", acc);
    }
    else {
        _l3g4200d_usage(argv[0]);
        return -1;
    }

    return 0;
}

static const shell_command_t commands[] = {
    /* lsm303dlhc shell command handler */
    { "lsm", "  reading accelerometer values", lsm303dlhc_handler },

    /* lpsxxx shell command handler */
   { "lps", "reading the  pressure and temperature  values", lpsxxx_handler },
    
    /* isl29020 shell command handler */
    { "isl", "reading light sensor values", isl29020_handler },

    /* l3g4200d shell command handler */
   { "l3g", "read the l3g4200d values", l3g4200d_handler },
    
    { NULL, NULL, NULL}
};

int main(void)
{
    /* Initialize the lps331ap sensor here */
    puts("Initializing LPS331AP sensor");
    lpsxxx_init(&lpsxxx, &lpsxxx_params[0]);
    if (lpsxxx_init(&lpsxxx, &lpsxxx_params[0]) == 0) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
    return 1;
    }
   
    /* Initialize the lsm303dlhc sensor here */
    puts("Initializing LSM303DLHC sensor");
    lsm303dlhc_init(&lsm303dlhc, &lsm303dlhc_params[0])
    if (lsm303dlhc_init(&lsm303dlhc, &lsm303dlhc_params[0]) == 0) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
    return 1;
    }
    
    /* Initialize the isl29020 sensor here */
    puts("Initializing ISL29020 sensor");
    isl29020_init(&isl29020, &isl29020_params[0])
    if (isl29020_init(&isl29020, &isl29020_params[0]) == 0) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
    return 1;
    }
    
    /*Initialize the l3g4200d sensor here*/
    puts("Initializing L3G4200D sensor");
    l3g4200d_init(&l3g4200d, &l3g4200d_params[0]);
    if (l3g4200d_init(&l3g4200d, &l3g4200d_params[0]) == 0) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
    return 1;
    }
    
   /* Perform sensor readings lsm303dlhc on a separate thread in order to host a shell on the main thread*/  
    thread_create(lsm303dlhc_stack, sizeof(lsm303dlhc_stack), THREAD_PRIORITY_MAIN - 1,
                  0, lsm303dlhc_thread, NULL, "lsm303dlhc");
    /* Perform sensor readings l3g4200d on a separate thread in order to host a shell on the main thread*/  
    thread_create(l3g4200d_stack, sizeof(l3g4200d_stack), THREAD_PRIORITY_MAIN - 2,
                  0, l3g4200d_thread, NULL, "l3g4200d");
    /* Perform sensor readings isl29020 on a separate thread in order to host a shell on the main thread*/  
    thread_create(isl29020_stack, sizeof(isl29020_stack), THREAD_PRIORITY_MAIN - 3,
                  0, isl29020_thread, NULL, "isl29020");
    /* Perform sensor readings lpsxxx on a separate thread in order to host a shell on the main thread*/  
    thread_create(lpsxxx_stack, sizeof(lpsxxx_stack), THREAD_PRIORITY_MAIN - 4,
                  0, lpsxxx_thread, NULL, "lpsxxx");

    /* Everything is ready, let's start the shell now */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
