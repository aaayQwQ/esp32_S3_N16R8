#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sntp.h"
#include "esp_log.h"

static const char *TAG = "HELLO_TIME";

static void set_timezone(void) {
    setenv("TZ", "CST-8", 1);
    tzset();
}

static void wait_for_time_sync(void) {
    time_t now = 0;
    struct tm timeinfo = {0};
    int retry = 0;
    while (timeinfo.tm_year < (2020 - 1900) && ++retry < 20) {
        ESP_LOGI(TAG, "等待时间同步... (%d/20)", retry);
        vTaskDelay(pdMS_TO_TICKS(1000));
        time(&now);
        localtime_r(&now, &timeinfo);
    }
}

void app_main(void) {
    printf("Hello, World! ESP32-S3 N16R8\n");
    set_timezone();

    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_init();

    wait_for_time_sync();

    while (1) {
        time_t now;
        struct tm timeinfo;
        char buf[64];
        time(&now);
        localtime_r(&now, &timeinfo);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
        printf("当前时间: %s\n", buf);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
