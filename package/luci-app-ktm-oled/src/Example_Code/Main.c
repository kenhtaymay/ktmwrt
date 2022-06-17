/*
 * Main.c
 *
 *  Created on  : Sep 6, 2017
 *  Author      : Vinay Divakar
 *  Description : Example usage of the SSD1306 Driver API's
 *  Website     : www.deeplyembedded.org
 */

/* Lib Includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include <time.h>

/* Header Files */
#include "I2C.h"
#include "SSD1306_OLED.h"
#include "example_app.h"

/* Oh Compiler-Please leave me as is */
volatile unsigned char flag = 0;

/* Alarm Signal Handler */
void ALARMhandler(int sig)
{
    /* Set flag */
    flag = 5;
}

void BreakDeal(int sig)
{
    clearDisplay();
    usleep(1000000);
    Display();
    exit(0);
}

FILE *fp;

int main(int argc, char *argv[])
{

    char *i2cdevice=argv[1];   
    int needinit=atoi(argv[2]);

    /* Initialize I2C bus and connect to the I2C Device */
    if (init_i2c_dev(i2cdevice, SSD1306_OLED_ADDR) == 0)
    {
        printf("(Main)i2c-2: Bus Connected to SSD1306\r\n");
    }
    else
    {
        printf("(Main)i2c-2: OOPS! Something Went Wrong\r\n");
        exit(1);
    }

    /* Register the Alarm Handler */
    signal(SIGALRM, ALARMhandler);
    signal(SIGINT, BreakDeal);
    // signal(SIGTERM, BreakDeal);
    /* Run SDD1306 Initialization Sequence */
    if (needinit==1) {
        display_Init_seq();
    /* Clear display */
        clearDisplay();

        setTextColor(WHITE);
        setTextSize(2);
        setCursor(24, 20);
        print_strln("KtmWrt");
        setTextSize(1);
        setCursor(72, 36);
        print_strln("v2.0");
        Display();
        sleep(2);
    }
    clearDisplay();

    char last_speed_test_time[16];
    int i = 0;
    char test[8];
    while (1)
    {

        time_t rawtime;
        // time_t curtime;
        uint8_t timebuff[32];
        // curtime = time(NULL);
        time(&rawtime);
        strftime(timebuff, 32, "%H:%M:%S %d-%m-%Y", localtime(&rawtime));

        fillRect(0, 57, 128, 7, BLACK);
        setTextColor(WHITE);
        setTextSize(1);
        setCursor(7, 57);
        print_strln(timebuff);

        if ((rawtime % 3 == 0))
        {

            if ((fp = popen("cat /sys/class/thermal/thermal_zone0/temp | cut -c -2 | awk '{printf \"%s%c\", $1, 0xF7}'", "r")) != NULL)
            {
                char temperature_buf[8];
                fgets(temperature_buf, 8, fp);
                fclose(fp);

                fillRect(5, 7, 36, 16, BLACK);
                setTextColor(WHITE);
                setTextSize(2);
                setCursor(5, 7);
                print_strln(temperature_buf);
            }

            if ((fp = fopen("/tmp/netspeed", "r")) != NULL)
            {
                char current_speed_test_time[16];
                char ping_buf[16];
                char download_buf[16];
                fgets(current_speed_test_time, 16, fp);
                fgets(ping_buf, 16, fp);
                fgets(download_buf, 16, fp);

                if (memcmp(last_speed_test_time, current_speed_test_time, 16) != 0 && !feof(fp))
                {
                    memcpy(last_speed_test_time, current_speed_test_time, 16);

                    fillRect(50, 0, 78, 8, BLACK);
                    setTextColor(WHITE);
                    setTextSize(1);
                    setCursor(50, 0);
                    oled_write(0x12);
                    print_strln(ping_buf);

                    fillRect(50, 10, 78, 8, BLACK);
                    setTextColor(WHITE);
                    setTextSize(1);
                    setCursor(50, 10);
                    oled_write(0x18);
                    print_strln(download_buf);

                    char upload_buf[16];
                    fgets(upload_buf, 16, fp);

                    fillRect(50, 20, 78, 8, BLACK);
                    setTextColor(WHITE);
                    setTextSize(1);
                    setCursor(50, 20);
                    oled_write(0x19);
                    print_strln(upload_buf);
                }
                fclose(fp);
            }

            if ((fp = popen("top -bn 1 | grep CPU | awk 'NR==1{print $2}' | sed 's/\%//'", "r")) != NULL)
            {
                char cpu_load_avg[4];
                fgets(cpu_load_avg, 4, fp);
                fillRect(0, 33, 128, 8, BLACK);
                setTextColor(WHITE);
                setTextSize(1);
                setCursor(0, 33);
                print_strln("CPU");
                drawRoundRect(24, 33, 104, 6, 3, WHITE);
                fillRoundRect(26, 35, (short)(atoi(cpu_load_avg)), 2, 1, WHITE);
                fclose(fp);
            }

            if ((fp = popen("free -m | awk 'NR==2{printf \"%d\", ($2-$4)*100/$2 }'", "r")) != NULL)
            {
                char mem_load_avg[4];
                fgets(mem_load_avg, 4, fp);
                fillRect(0, 45, 128, 8, BLACK);
                setTextColor(WHITE);
                setTextSize(1);
                setCursor(0, 45);
                print_strln("MEM");
                drawRoundRect(24, 45, 104, 6, 3, WHITE);
                fillRoundRect(26, 47, (short)(atoi(mem_load_avg)), 2, 1, WHITE);
                fclose(fp);
            }
        }

        // sprintf(test, "(%03d) %c  %c  %c  %c  %c", i, i, i+1, i+2, i+3, i+4);
        // i = i == 255 ? 0 : ++i;
        // fillRect(0, 40, 128, 8, BLACK);
        // setTextColor(WHITE);
        // setTextSize(1);
        // setCursor(0, 40);
        // print_strln(test);

        Display();
        sleep(1);
    }

    // pthread_t thread_id[5];
    // pthread_create(&thread_id[0], NULL, thread_run_queue, NULL);
    // pthread_create(&thread_id[1], NULL, date_time_thread, NULL);
    // pthread_create(&thread_id[2], NULL, temperature_thread, NULL);
    // pthread_create(&thread_id[3], NULL, cpu_mem_load_thread, NULL);
    // pthread_create(&thread_id[4], NULL, network_info_thread, NULL);
    // pthread_join(thread_id[0], NULL);
    // pthread_exit(NULL);
}
