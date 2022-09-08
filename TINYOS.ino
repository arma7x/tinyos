
// #include <WiFi.h>
#include "env.h"
#include "constant.h"
#include "driver.h"
#include "module.h"
#include "register_module.h"
#include "notification_bar.h"



#define BLK 16
#define RST 33
#define SET 25
#define MID 26
#define RHT 27
#define LFT 14
#define DWN 12
#define UP  13

void setup()
{
  Serial.begin(115200);

  uint8_t lcd_b = getPreferences().getUChar("lcd_b", 15);
  ledcSetup(0, 5000, 8);
  ledcAttachPin(BLK, 0);
  ledcWrite(0, lcd_b); 
  
  pinMode(RST, INPUT_PULLUP);
  pinMode(SET, INPUT_PULLUP);
  pinMode(MID, INPUT_PULLUP);
  pinMode(RHT, INPUT_PULLUP);
  pinMode(LFT, INPUT_PULLUP);
  pinMode(DWN, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);

  LCD.init();
  LCD.setRotation(3);
  LCD.fillScreen(TFT_BG);

  InitNotificationBar();

  ModuleSwitcher(GetModuleHomescreen());
  GetActiveModule().Init(0);

//  Serial.printf("Connecting to %s ", WIFI_SSID);
//  WiFi.begin(WIFI_SSID, WIFI_PASS);
//  while (WiFi.status() != WL_CONNECTED) {
//      delay(500);
//      Serial.print(".");
//  }
//  Serial.println(F(" CONNECTED"));
//  
//  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
//  printLocalTime();
//
//  WiFi.disconnect(true);
//  WiFi.mode(WIFI_OFF);

}

void loop()
{
  if(digitalRead(RST) == LOW) {
    delay(250);
    Serial.println(F("Reset Pin Is Pressed."));
    GetActiveModule().KeyReset();
    while(digitalRead(RST) == LOW);
  }

  if(digitalRead(SET) == LOW) {
    delay(250);
    Serial.println(F("Set Pin Is Pressed."));
    GetActiveModule().KeySet();
    while(digitalRead(SET) == LOW);
  }

  if(digitalRead(MID) == LOW) {
    delay(250);
    Serial.println(F("Middle Pin Is Pressed."));
    GetActiveModule().KeyMid();
    while(digitalRead(MID) == LOW);
  }

  if(digitalRead(RHT) == LOW) {
    delay(250);
    Serial.println(F("Right Pin Is Pressed."));
    GetActiveModule().KeyRight();
    while(digitalRead(RHT) == LOW);
  }

  if(digitalRead(LFT) == LOW) {
    delay(250);
    Serial.println(F("Left Pin Is Pressed."));
    GetActiveModule().KeyLeft();
    while(digitalRead(LFT) == LOW);
  }

  if(digitalRead(DWN) == LOW) {
    delay(250);
    Serial.println(F("Down Pin Is Pressed."));
    GetActiveModule().KeyDown();
    while(digitalRead(DWN) == LOW);
  }

  if(digitalRead(UP) == LOW) {
    delay(250);
    Serial.println(F("Up Pin Is Pressed."));
    GetActiveModule().KeyUp();
    while(digitalRead(UP) == LOW);
  }
}
