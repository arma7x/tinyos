#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include <WiFi.h>
#include <pgmspace.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "env.h"
#include "driver.h"
#include "module.h"
#include "register_module.h"
#include "resources.h"

#ifdef __cplusplus
 extern "C" {
#endif

TaskHandle_t TaskScanWifiPid;

bool mounted = 0;
int networks = 0;
uint8_t network_cursor = 0;
static char password[27];


void displayWelcome() {
  clearSafeArea();
  LCD.setTextFont(1);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  LCD.drawString("Press enter to start scan", floor((TFT_W - (25 * 6)) / 2), 35);
}

void displayEmptyNetwork() {
  clearSafeArea();
  LCD.setTextFont(1);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  LCD.drawString("No networks found", floor((TFT_W - (17 * 6)) / 2), 35);
}

void displayStartScanning() {
  clearSafeArea();
  LCD.setTextFont(1);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  LCD.drawString("Scanning", floor((TFT_W - (8 * 6)) / 2), 35);
}

void displayNetwork() {
  clearSafeArea(); 
  char label[10];
  char b_r_ssid[22];
  char *encryptionType; 
  LCD.setTextFont(1);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  sprintf(label, "%d of %d", network_cursor + 1, networks);
  LCD.drawString(label, floor((TFT_W - (strlen(label) * 6)) / 2), 12);
  LCD.drawString(WiFi.SSID(network_cursor), floor((TFT_W - (WiFi.SSID(network_cursor).length() * 6)) / 2), 27);;
  sprintf(b_r_ssid, "%s %d", WiFi.BSSIDstr(network_cursor).c_str(), WiFi.RSSI(network_cursor));
  LCD.drawString(b_r_ssid, floor((TFT_W - (strlen(b_r_ssid) * 6)) / 2), 42);
  switch (WiFi.encryptionType(network_cursor)) {
    case WIFI_AUTH_OPEN:
      encryptionType = "WIFI_AUTH_OPEN";
      break;
    case WIFI_AUTH_WEP:
      encryptionType = "WIFI_AUTH_WEP";
      break;
    case WIFI_AUTH_WPA_PSK:
      encryptionType = "WIFI_AUTH_WPA_PSK";
      break;
    case WIFI_AUTH_WPA2_PSK:
      encryptionType = "WIFI_AUTH_WPA2_PSK";
      break;
    case WIFI_AUTH_WPA_WPA2_PSK:
      encryptionType = "WIFI_AUTH_WPA_WPA2_PSK";
      break;
    case WIFI_AUTH_WPA2_ENTERPRISE:
      encryptionType = "WIFI_AUTH_WPA2_ENTERPRISE";
      break;
    default:
      encryptionType = "UNKNOWN";
    }
    LCD.drawString(encryptionType, floor((TFT_W - (strlen(encryptionType) * 6)) / 2), 57);
}

void taskScanWifi(void *pvParameters) {
  for(;;) {
    displayStartScanning();
    if (WiFi.scanComplete() == -1) {
      vTaskDelay(500);
      continue;
    }
    networks = 0;
    network_cursor = 0;
    WiFi.scanDelete();
    networks = WiFi.scanNetworks();
    if (networks == 0) {
      displayEmptyNetwork();
    } else {
      network_cursor = 0;
      displayNetwork();
    }
    vTaskSuspend(TaskScanWifiPid);
  }
}

void connectWifi(const char *ssid, char *password) {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    LCD.drawString("Wi-Fi:CONNECTING", 1, 1);
  }
  LCD.setTextFont(1);
  LCD.fillRect(0, 1, 96, 10, TFT_BG);
  LCD.drawString("Wi-Fi:CONNECTED", 1, 1);
  Serial.println(F("Connected to the WiFi network"));
}

static void setPasswordCallback(char *text) {
  snprintf(password, 27, "%s", text);
  GetActiveModule().Destroy();
  ModuleSwitcher(GetModuleWiFi());
  GetActiveModule().Init(0);
  if (strlen(password) > 0) {
    const char *key = WiFi.BSSIDstr(network_cursor).c_str();
    getPreferences().putString(key, String(password));
    connectWifi(WiFi.SSID(network_cursor).c_str(), password);
  }
}

static void _init(int num, ...) {
  if (mounted != 1) {
    mounted = 1;
    networks = 0;
    network_cursor = 0;
    displayWelcome();
  } else {
    displayNetwork();
  }
}

static void _destroy() {
  mounted = 0;
  networks = 0;
  network_cursor = 0;
  WiFi.scanDelete();
  for (uint8_t _i=0;_i<27;_i++) {
    password[_i] = '\0';
  }
  if (TaskScanWifiPid != NULL) {
    vTaskSuspend(TaskScanWifiPid);
    vTaskDelete(TaskScanWifiPid);
    TaskScanWifiPid = NULL;
  }
}

static void onKeyUp() {}

static void onKeyDown( ) {}

static void onKeyRight( ) {
  if (network_cursor >= networks - 1) {
    return;
  }
  network_cursor++;
  if (network_cursor < networks) {
    displayNetwork();
  }
}

static void onKeyLeft() {
  if (network_cursor == 0) {
    return;
  }
  network_cursor--;
  if (networks >= 0) {
    displayNetwork();
  }
}

static void onKeyMid() {
  if (TaskScanWifiPid == NULL) {
    xTaskCreatePinnedToCore(taskScanWifi, "TaskScanWifi", 8192, NULL, 3, &TaskScanWifiPid, ARDUINO_RUNNING_CORE);
  } else {
    vTaskResume(TaskScanWifiPid);
  }
}

static void onKeySet() {
  if (networks > 0) {
    if (WiFi.status() == WL_CONNECTED) {
      WiFi.disconnect(true);
    }
    if (WiFi.encryptionType(network_cursor) != WIFI_AUTH_OPEN) {
      const char *key = WiFi.BSSIDstr(network_cursor).c_str();
      String _password = getPreferences().getString(key, "");
      snprintf(password, 27, "%s", _password.c_str());
      ModuleSwitcher(GetModuleKeyboardUI());
      GetActiveModule().Init(3, password, 27, setPasswordCallback);
    } else {
      connectWifi(WiFi.SSID(network_cursor).c_str(), "");
    }
  }
}

static void onKeyReset( ) {
  GetActiveModule().Destroy();
  ModuleSwitcher(GetModuleSettings());
  GetActiveModule().Init(0);
}

Module wifi = { _init, _destroy, onKeyUp, onKeyDown,onKeyRight, onKeyLeft, onKeyMid, onKeySet, onKeyReset };

Module GetModuleWiFi() {
  return wifi;
}

#ifdef __cplusplus
}
#endif
