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
  clearDisplaySafeArea();
  LCD.setTextFont(1);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  LCD.drawString(F("Press enter to start scan"), floor((TFT_W - LCD.textWidth(F("Press enter to start scan"))) / 2), 35);
}

void displayEmptyNetwork() {
  clearDisplaySafeArea();
  LCD.setTextFont(1);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  LCD.drawString(F("No networks found"), floor((TFT_W - LCD.textWidth(F("No networks found"))) / 2), 35);
}

void displayStartScanning() {
  clearDisplaySafeArea();
  LCD.setTextFont(1);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  LCD.drawString(F("Scanning"), floor((TFT_W - LCD.textWidth(F("Scanning"))) / 2), 35);
}

void displayNetwork() {
  clearDisplaySafeArea();
  char label[10];
  char b_r_ssid[22];
  LCD.setTextFont(1);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  sprintf(label, "%d of %d", network_cursor + 1, networks);
  LCD.drawString(label, floor((TFT_W - LCD.textWidth(label)) / 2), 12);
  LCD.drawString(WiFi.SSID(network_cursor), floor((TFT_W - LCD.textWidth(WiFi.SSID(network_cursor))) / 2), 27);
  sprintf(b_r_ssid, "%s %d", WiFi.BSSIDstr(network_cursor).c_str(), WiFi.RSSI(network_cursor));
  LCD.drawString(b_r_ssid, floor((TFT_W - LCD.textWidth(b_r_ssid)) / 2), 42);
  switch (WiFi.encryptionType(network_cursor)) {
    case WIFI_AUTH_OPEN:
      LCD.drawString(F("WIFI_AUTH_OPEN"), floor((TFT_W - LCD.textWidth(F("WIFI_AUTH_OPEN"))) / 2), 57);
      break;
    case WIFI_AUTH_WEP:
      LCD.drawString(F("WIFI_AUTH_WEP"), floor((TFT_W - LCD.textWidth(F("WIFI_AUTH_WEP"))) / 2), 57);
      break;
    case WIFI_AUTH_WPA_PSK:
      LCD.drawString(F("WIFI_AUTH_WPA_PSK"), floor((TFT_W - LCD.textWidth(F("WIFI_AUTH_WPA_PSK"))) / 2), 57);
      break;
    case WIFI_AUTH_WPA2_PSK:
      LCD.drawString(F("WIFI_AUTH_WPA2_PSK"), floor((TFT_W - LCD.textWidth(F("WIFI_AUTH_WPA2_PSK"))) / 2), 57);
      break;
    case WIFI_AUTH_WPA_WPA2_PSK:
      LCD.drawString(F("WIFI_AUTH_WPA_WPA2_PSK"), floor((TFT_W - LCD.textWidth(F("WIFI_AUTH_WPA_WPA2_PSK"))) / 2), 57);
      break;
    case WIFI_AUTH_WPA2_ENTERPRISE:
      LCD.drawString(F("WIFI_AUTH_WPA2_ENTERPRISE"), floor((TFT_W - LCD.textWidth(F("WIFI_AUTH_WPA2_ENTERPRISE"))) / 2), 57);
      break;
    default:
      LCD.drawString(F("UNKNOWN"), floor((TFT_W - LCD.textWidth(F("UNKNOWN"))) / 2), 57);
    }
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
