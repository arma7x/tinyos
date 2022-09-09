#ifndef HEALTH_MONITOR_H
#define HEALTH_MONITOR_H

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include "freertos/task.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#ifdef __cplusplus
 extern "C" {
#endif

char mem_str[10];
float mem;

void TaskHealthMonitor(void *pvParameters) {
  for (;;) {
    mem = (float) ((ESP.getFreeHeap() - esp_get_free_heap_size()) * 100) / (float) ESP.getFreeHeap();
    // Serial.printf("%d => %.2f%%\n", ESP.getFreeHeap(), mem);
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(1, 1);
    display.printf("CPU: %.2f%%  ", 0.0);
    display.setCursor(1, 10);
    display.printf("RAM: %.2f%%  ", mem);
    display.display();
    vTaskDelay(5000);
  }
}

void watch() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(1, 1);
  display.println(F("CPU:"));
  display.setCursor(1, 10);
  display.println(F("RAM:"));
  display.display();
  xTaskCreatePinnedToCore(
    TaskHealthMonitor,  
    "TaskHealthMonitor",   // A name just for humans
    2048,  // This stack size can be checked & adjusted by reading the Stack Highwater
    NULL,
    3,  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    NULL, 
    ARDUINO_RUNNING_CORE
  );
}

#ifdef __cplusplus
}
#endif

#endif
