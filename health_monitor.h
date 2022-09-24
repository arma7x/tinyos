#ifndef HEALTH_MONITOR_H
#define HEALTH_MONITOR_H

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include "env.h"
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

uint8_t temprature_sens_read();

void TaskHealthMonitor(void *pvParameters) {
  float value;
  for (;;) {
    value = (temprature_sens_read() - 32) / 1.8;
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(1, 1);
    display.printf("TEMP: %.2fC  ", value);
    display.setCursor(1, 10);
    value = ((ESP.getHeapSize() - esp_get_free_heap_size()) * 100) / (float) ESP.getHeapSize();
    display.printf("RAM : %.2f%% ", value);
    display.display();
    vTaskDelay(2000);
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
  display.println(F("TEMP:"));
  display.setCursor(1, 10);
  display.println(F("RAM :"));
  display.display();
  xTaskCreatePinnedToCore(TaskHealthMonitor, "TaskHealthMonitor", 2048, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
}

#ifdef __cplusplus
}
#endif

#endif
