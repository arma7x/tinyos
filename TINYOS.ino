#include <WiFi.h>
#include "health_monitor.h"
#include "env.h"
#include "driver.h"
#include "module.h"
#include "register_module.h"
#include "notification_bar.h"

void setup()
{
  Serial.begin(115200);

  // Serial.print(ICMP_TTL);
  pinMode(RST, INPUT_PULLUP);
  pinMode(SET, INPUT_PULLUP);
  pinMode(MID, INPUT_PULLUP);
  pinMode(RHT, INPUT_PULLUP);
  pinMode(LFT, INPUT_PULLUP);
  pinMode(DWN, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);

  initLCD();

  initNotificationBar();

  ModuleSwitcher(GetModuleHomescreen());
  GetActiveModule().Init(0);

  WiFi.mode(WIFI_MODE_STA);
  WiFi.mode(WIFI_MODE_NULL);
  // WiFi.begin(WIFI_SSID, WIFI_PASS);
  // updateWifiStatus();
  Serial.println(F("RUNNING TINYOS"));
  watch();
}

void loop()
{
  if(digitalRead(RST) == LOW) {
    delay(250);
    // Serial.println(F("Reset Pin Is Pressed."));
    GetActiveModule().KeyReset();
    while(digitalRead(RST) == LOW);
  }

  if(digitalRead(SET) == LOW) {
    delay(250);
    // Serial.println(F("Set Pin Is Pressed."));
    GetActiveModule().KeySet();
    while(digitalRead(SET) == LOW);
  }

  if(digitalRead(MID) == LOW) {
    delay(250);
    // Serial.println(F("Middle Pin Is Pressed."));
    GetActiveModule().KeyMid();
    while(digitalRead(MID) == LOW);
  }

  if(digitalRead(RHT) == LOW) {
    delay(250);
    // Serial.println(F("Right Pin Is Pressed."));
    GetActiveModule().KeyRight();
    while(digitalRead(RHT) == LOW);
  }

  if(digitalRead(LFT) == LOW) {
    delay(250);
    // Serial.println(F("Left Pin Is Pressed."));
    GetActiveModule().KeyLeft();
    while(digitalRead(LFT) == LOW);
  }

  if(digitalRead(DWN) == LOW) {
    delay(250);
    // Serial.println(F("Down Pin Is Pressed."));
    GetActiveModule().KeyDown();
    while(digitalRead(DWN) == LOW);
  }

  if(digitalRead(UP) == LOW) {
    delay(250);
    // Serial.println(F("Up Pin Is Pressed."));
    GetActiveModule().KeyUp();
    while(digitalRead(UP) == LOW);
  }
}
