#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "net/ipv6/addr.h"
#include "thread.h"
#include "timex.h"
#include "xtimer.h"
#include "shell.h"
#include "net/emcute.h" 
#include "msg.h"
#include "mutex.h"
#include <time.h>

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

#include "isl29020.h"
#include "isl29020_params.h"

/* Add l3g44200 related include here*/
/*l3g44200 gyroscope  sensor*/

#include "l3g4200d.h"
#include "l3g4200d_params.h"


#ifndef EMCUTE_ID
#define EMCUTE_ID           ("gertrud")
#endif
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define CONFIG_EMCUTE_DEFAULT_PORT   (1883U)

#define DELAY              (100LU * US_PER_MS) /* 1000 ms */
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)
#define _IPV6_DEFAULT_PREFIX_LEN        (64U)


static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];



/* [Emcute - MQTT] Stack and  vars */
static char emcute_stack[THREAD_STACKSIZE_DEFAULT];

/* Declare the lpsxxx device variable here */
 
static lpsxxx_t lpsxxx;

/* Declare the lsm303dlhc device variable here */

static lsm303dlhc_t lsm303dlhc;

/* Declare the isl29020 device variable here*/

static isl29020_t isl29020;

/*Declare the l3g4200d device variable here*/

static l3g4200d_t l3g4200d;


/* Declare and initialize the lsm303dlhc thread lock here */
static mutex_t lsm_lock = MUTEX_INIT_LOCKED;

/* Declare and initialize the lpsxxx thread lock here */
static mutex_t lps_lock = MUTEX_INIT_LOCKED;

/* Declare and initialize the isl29020 thread lock here */
static mutex_t isl_lock = MUTEX_INIT_LOCKED;

/* Declare and initialize the l3g4200d thread lock here */
static mutex_t l3g_lock = MUTEX_INIT_LOCKED;


//static char stack[THREAD_STACKSIZE_MAIN];

/* stack memory allocated for the lsm303dlhc, l3g4200d, isl29020, lpsxxx thread */

static char lsm303dlhc_stack[THREAD_STACKSIZE_MAIN];
static char l3g4200d_stack[THREAD_STACKSIZE_MAIN];
static char isl29020_stack[THREAD_STACKSIZE_MAIN];
static char lpsxxx_stack[THREAD_STACKSIZE_MAIN];

static char stack[THREAD_STACKSIZE_DEFAULT];

static emcute_topic_t  topic;
static unsigned  flags = EMCUTE_QOS_0;

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, EMCUTE_ID);
    return NULL;    /* should never be reached */
}

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;

    printf("### got publication for topic '%s' [%i] ###\n",
           topic->name, (int)topic->id);
    for (size_t i = 0; i < len; i++) {
        printf("%c", in[i]);
    }
    puts("");
}



 static unsigned get_qos(const char *str)
{
    int qos = atoi(str);
    switch (qos) {
        case 1:     return EMCUTE_QOS_1;
        case 2:     return EMCUTE_QOS_2;
        default:    return EMCUTE_QOS_0;
    }
}

  

static void *lsm303dlhc_thread(void *arg)
{
    (void)arg;
    
/* Add the lsm303dlhc accelerometer/magnetoeter sensor polling endless loop here */
    
    while (1) {
        
        /* Acquire the mutex here */
      
        mutex_lock(&lsm_lock);
    
 
        /* Read the accelerometer/magnetometer values here */
      
        lsm303dlhc_3d_data_t mag_value;
        lsm303dlhc_3d_data_t acc_value;
        lsm303dlhc_read_acc(&lsm303dlhc, &acc_value);
        lsm303dlhc_read_mag(&lsm303dlhc, &mag_value);

        
        printf("Accelerometer x: %i y: %i z: %i\n",
           acc_value.x_axis, acc_value.y_axis, acc_value.z_axis);
        printf("Magnetometer x: %i y: %i z: %i\n",
           mag_value.x_axis, mag_value.y_axis, mag_value.z_axis);
      
        xtimer_usleep(500 * US_PER_MS);

        xtimer_ticks32_t last = xtimer_now(); 

        /*json structure*/
        char json[128];  

        /*structure of time*/
        char datetime[20];
        time_t current;
   
     
        // takes the current date and time
        time(&current);
 
        struct tm* tt = localtime(&current);
        int c = strftime(datetime, sizeof(datetime), "%Y-%m-%d %T", tt);
        if(c == 0) {
          printf("Error! Invalid format\n");
          return 0;
        } 
    
        // takes the sample sensor data

        char value[1] = "1";
        
   
        /* step 1:  fills the json document */

        sprintf(json, "{\"id\": \"%d\", \"datetime\": \"%s\", \"X\": %6i, \"Y\": %6i,\"Z\": %6i}",
                  atoi(value), datetime, acc_value.x_axis, acc_value.y_axis, acc_value.z_axis);                  
        // argv[2] = json;  
         
 
    /* step 2: publish data */
      if (emcute_pub(&topic, json, strlen(json), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s [%i]'\n",
                topic.name, (int)topic.id);
        return 0;
      }

      printf("Published %i bytes to topic '%s  [%i]'\n",
            (int)strlen(json), topic.name, topic.id);

      xtimer_periodic_wakeup(&last, DELAY);

  
        /* Release the mutex here */
        
        mutex_unlock(&lsm_lock);
  
      
        xtimer_usleep(500 * US_PER_MS);
    }

    return 0;
}

static void *l3g4200d_thread(void *arg)
{
    (void)arg;
    
/* Add the l3g4200d gyroscope sensor polling endless loop here */
    
    while (1) {
        
        /* Acquire the mutex here */
    
        mutex_lock(&l3g_lock);

        /* Read the gyroscope  values here */
      
       l3g4200d_t l3g4200d;
       l3g4200d_data_t acc_data;
       l3g4200d_read(&l3g4200d, &acc_data);
       printf("Gyroscope data [dps] - X: %6i   Y: %6i   Z: %6i\n",
           acc_data.acc_x, acc_data.acc_y, acc_data.acc_z);
       xtimer_usleep(500 * US_PER_MS);

       xtimer_ticks32_t last = xtimer_now();

       /*json structure*/
       char json[128];  

       /*structure of time*/
       char datetime[20];
       time_t current;
   
     
        // takes the current date and time
    
        time(&current);
        struct tm* tt = localtime(&current);
        int c = strftime(datetime, sizeof(datetime), "%Y-%m-%d %T", tt);
        if(c == 0) {
          printf("Error! Invalid format\n");
          return 0;
        } 
    
        // takes the sample sensor data

         char value[1] = "1";
   
        /* step 1:  fills the json document */

        sprintf(json, "{\"id\": \"%d\", \"datetime\": \"%s\", \"X\": %6i, \"Y\": %6i,\"Z\": %6i}",
                  atoi(value), datetime, acc_data.acc_x, acc_data.acc_y, acc_data.acc_z);                  
      //argv[2] = json;  
         
 
    /* step 2: publish data */
      if (emcute_pub(&topic, json, strlen(json), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s [%i]'\n",
                topic.name, (int)topic.id);
        return 0;
      }

      printf("Published %i bytes to topic '%s  [%i]'\n",
            (int)strlen(json), topic.name, topic.id);

      xtimer_periodic_wakeup(&last, DELAY);


        /* Release the mutex here */
    
       mutex_unlock(&l3g_lock);
      
       xtimer_usleep(500 * US_PER_MS);
    }

    return 0;
}

static void *isl29020_thread(void *arg)
{
    (void)arg;
    
/* Add the isl29020 light sensor polling endless loop here */
    
    while (1) {
        
        /* Acquire the mutex here */
    
        mutex_lock(&isl_lock);

        /* Read the light sensor  values here */
      
       isl29020_t lumin;
       printf("Intensity of luminescence: %5i LUX\n", isl29020_read(&lumin));
       xtimer_usleep(500 * US_PER_MS);
       xtimer_ticks32_t last = xtimer_now();

        /*json structure*/
        char json[128];  

        /*structure of time*/
        char datetime[20];
        time_t current;

       // takes the current date and time
    
        time(&current);
        struct tm* tt = localtime(&current);
        int c = strftime(datetime, sizeof(datetime), "%Y-%m-%d %T", tt);
        if(c == 0) {
          printf("Error! Invalid format\n");
          return 0;
        } 

    // takes the sample sensor data

      char value[1] = "1";
   
    /* step 1:  fills the json document */

    sprintf(json, "{\"id\": \"%d\", \"datetime\": \"%s\", \"LUMINESCENCE\": %5i}",
                  atoi(value), datetime, isl29020_read(&lumin));                  
      //argv[2] = json;  
         
 
    /* step 2: publish data */
      if (emcute_pub(&topic, json, strlen(json), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s [%i]'\n",
                topic.name, (int)topic.id);
        return 0;
      }

      printf("Published %i bytes to topic '%s  [%i]'\n",
            (int)strlen(json), topic.name, topic.id);

       xtimer_periodic_wakeup(&last, DELAY);

       
       /* Release the mutex here */

       mutex_unlock(&isl_lock);


       xtimer_usleep(500 * US_PER_MS);
    }

    return 0;
}


static void *lpsxxx_thread(void *arg)
{
    (void)arg;
    
    /* Add the lpsxxx atmospheric pressure and temperature sensor polling endless loop here */
    
    while (1) {
        
        /* Acquire the mutex here */
    
        mutex_lock(&lps_lock);

        /* Read the atmospheric pressure and temperature  values here */
 
        uint16_t pres = 0;
        int16_t temp = 0;
        lpsxxx_read_pres(&lpsxxx, &pres);
        lpsxxx_read_temp(&lpsxxx, &temp);
       
        printf("Pressure: %uhPa, Temperature: %u.%u°C\n",
           pres, (temp / 100), (temp % 100));
        xtimer_sleep(2);
        xtimer_usleep(500 * US_PER_MS);
  
        xtimer_ticks32_t last = xtimer_now(); 
   

        /*json structure*/
        char json[128];  

        /*structure of time*/
        char datetime[20];
        time_t current;
     
        // takes the current date and time
    
        time(&current);
        struct tm* tt = localtime(&current);
        int c = strftime(datetime, sizeof(datetime), "%Y-%m-%d %T", tt);
        if(c == 0) {
          printf("Error! Invalid format\n");
          return 0;
        } 
    
    // takes the sample sensor data

     char value[1] = "1"; 
   
    /* step 1:  fills the json document */

    sprintf(json, "{\"id\": \"%d\", \"datetime\": \"%s\", \"PRESSURE\": %i, \"TEMPERATURE\": %i}",
                  atoi(value), datetime, pres, temp);                  
     // argv[2] = json;  
         
 
    /* step 2: publish data */
      if (emcute_pub(&topic, json, strlen(json), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s [%i]'\n",
                topic.name, (int)topic.id);
        return 0;
      }

      printf("Published %i bytes to topic '%s  [%i]'\n",
            (int)strlen(json), topic.name, topic.id);

      xtimer_periodic_wakeup(&last, DELAY);
  
    /*   gpio_toggle(pin);*/   
  
  
    // it sleeps for five seconds
      xtimer_sleep(5);            




        /* Release the mutex here */

        mutex_unlock(&lps_lock);
      
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

    /* Implement the lsm303dlhc start/stop temperature|magnetometer|accelerometer subcommands here */
    
    if (!strcmp(argv[1], "start")) {
        mutex_unlock(&lsm_lock);
    }
    else if (!strcmp(argv[1], "stop")) {
        mutex_trylock(&lsm_lock);
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

    /* Implement the lpsxxx temperature/pressure subcommands here */

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
   
    if (!strcmp(argv[1], "start")) {
        mutex_unlock(&lsm_lock);
    }
    else if (!strcmp(argv[1], "stop")) {
        mutex_trylock(&lsm_lock);
    }
    else {
        _lsm303dlhc_usage(argv[0]);
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

    /* Implement the gyroscope l3g4200d start/stop subcommands here */
    
     if (!strcmp(argv[1], "start")) {
        mutex_unlock(&lsm_lock);
    }
    else if (!strcmp(argv[1], "stop")) {
        mutex_trylock(&lsm_lock);
    }
    else {
        _lsm303dlhc_usage(argv[0]);
        return -1;
    }


    return 0;
}

static int cmd_con(int argc, char **argv)
{
    sock_udp_ep_t gw = { .family = AF_INET6, .port = CONFIG_EMCUTE_DEFAULT_PORT };
    char *topic = NULL;
    char *message = NULL;
    size_t len = 0;

    if (argc < 2) {
        printf("usage: %s <ipv6 addr> [port] [<will topic> <will message>]\n",
                argv[0]);
        return 1;
    }

    /* parse address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, argv[1]) == NULL) {
        printf("error parsing IPv6 address\n");
        return 1;
    }

    if (argc >= 3) {
        gw.port = atoi(argv[2]);
    }
    if (argc >= 5) {
        topic = argv[3];
        message = argv[4];
        len = strlen(message);
    }

    if (emcute_con(&gw, true, topic, message, len, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", argv[1], (int)gw.port);
        return 1;
    }
    printf("Successfully connected to gateway at [%s]:%i\n",
           argv[1], (int)gw.port);

    return 0;
}

 
static int cmd_discon(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    int res = emcute_discon();
    if (res == EMCUTE_NOGW) {
        puts("error: not connected to any broker");
        return 1;
    }
    else if (res != EMCUTE_OK) {
        puts("error: unable to disconnect");
        return 1;
    }
    puts("Disconnect successful");
    return 0;
}

static int cmd_pub(int argc, char **argv)
{
    emcute_topic_t topic;
    unsigned flags = EMCUTE_QOS_0;

    if (argc < 2) {
        printf("usage: %s <topic name>  [QoS level]\n", argv[0]);
        return 1;
    }

    /* parse QoS level */
    if (argc >= 4) {
        flags |= get_qos(argv[3]);
    }

    printf("pub with topic: %s and name %s and flags 0x%02x\n", argv[1], argv[2], (int)flags);

    /* step 1: get topic id */
    topic.name = argv[1];
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID");
        return 1;
    }

    ;
    /*ACTUATOR STATUS ON/OF*/
   // char pump[2] = "OF"; 
    /*SENSOR VARIABLES*/
       
  
    /*This four threads provide with continuous readings. while the shell, which is a separate thread (the main)
    provide with start/stop readings*/
    
 
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

 
// it sleeps for five seconds
      xtimer_sleep(5);            

    return 0;
}

static int cmd_sub(int argc, char **argv)
{
    unsigned flags = EMCUTE_QOS_0;

    if (argc < 2) {
        printf("usage: %s <topic name> [QoS level]\n", argv[0]);
        return 1;
    }

    if (strlen(argv[1]) > TOPIC_MAXLEN) {
        puts("error: topic name exceeds maximum possible size");
        return 1;
    }
    if (argc >= 3) {
        flags |= get_qos(argv[2]);
    }

    /* find empty subscription slot */
    unsigned i = 0;
    for (; (i < NUMOFSUBS) && (subscriptions[i].topic.id != 0); i++) {}
    if (i == NUMOFSUBS) {
        puts("error: no memory to store new subscriptions");
        return 1;
    }

    subscriptions[i].cb = on_pub;
    strcpy(topics[i], argv[1]);
    subscriptions[i].topic.name = topics[i];
    if (emcute_sub(&subscriptions[i], flags) != EMCUTE_OK) {
        printf("error: unable to subscribe to %s\n", argv[1]);
        return 1;
    }

    printf("Now subscribed to %s\n", argv[1]);
    return 0;
}

static int cmd_unsub(int argc, char **argv)
{
    if (argc < 2) {
        printf("usage %s <topic name>\n", argv[0]);
        return 1;
    }

    /* find subscriptions entry */
    for (unsigned i = 0; i < NUMOFSUBS; i++) {
        if (subscriptions[i].topic.name &&
            (strcmp(subscriptions[i].topic.name, argv[1]) == 0)) {
            if (emcute_unsub(&subscriptions[i]) == EMCUTE_OK) {
                memset(&subscriptions[i], 0, sizeof(emcute_sub_t));
                printf("Unsubscribed from '%s'\n", argv[1]);
            }
            else {
                printf("Unsubscription form '%s' failed\n", argv[1]);
            }
            return 0;
        }
    }

    printf("error: no subscription for topic '%s' found\n", argv[1]);
    return 1;
}

static int cmd_will(int argc, char **argv)
{
    if (argc < 3) {
        printf("usage %s <will topic name> <will message content>\n", argv[0]);
        return 1;
    }

    if (emcute_willupd_topic(argv[1], 0) != EMCUTE_OK) {
        puts("error: unable to update the last will topic");
        return 1;
    }
    if (emcute_willupd_msg(argv[2], strlen(argv[2])) != EMCUTE_OK) {
        puts("error: unable to update the last will message");
        return 1;
    }

    puts("Successfully updated last will topic and message");
    return 0;
}



static const shell_command_t commands[] = {
 
    /* shell command connect  */  
    { "con", "Start the station", cmd_con},

    /* shell command connect  */ 
    { "discon", "disconnect from the current broker", cmd_discon },

    /* shell command connect  */ 
    { "pub", "publish something", cmd_pub },
    
    /* shell command connect  */ 
    { "sub", "subscribe topic", cmd_sub },
    
    /* shell command connect  */ 
    { "unsub", "unsubscribe from topic", cmd_unsub },
    
    /* shell command connect  */ 
    { "will", "register a last will", cmd_will },
 
    /* lsm303dlhc shell command handler */
    { "lsm", "  reading lsm303dlhc accelerometer values", lsm303dlhc_handler },

    /* lpsxxx shell command handler */
    { "lps", "reading the lpsxxx  pressure and temperature  values", lpsxxx_handler },
    
    /* isl29020 shell command handler */
    { "isl", "reading isl29020 light sensor values", isl29020_handler },

    /* l3g4200d shell command handler */
    { "l3g", "read the gyroscope l3g4200d values", l3g4200d_handler },
    
    { NULL, NULL, NULL}
};

int main(void)
{

    puts("Riot Application");
    /* Initialize the lps331ap sensor here */
    puts("Initializing LPSxxx sensor");
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
    lsm303dlhc_init(&lsm303dlhc, &lsm303dlhc_params[0]);
    if (lsm303dlhc_init(&lsm303dlhc, &lsm303dlhc_params[0]) == 0) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
    return 1;
    }
    
    /* Initialize the isl29020 sensor here */
    puts("Initializing ISL29020 sensor");
    isl29020_init(&isl29020, &isl29020_params[0]);
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


    /* the main thread needs a msg queue to be able to run `ping6`*/
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* initialize our subscription buffers */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));

    /* start the emcute thread */
    thread_create(emcute_stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");


     

    /* Everything is ready, let's start the shell now */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
