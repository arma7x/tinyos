#include <WiFi.h>
#include <pgmspace.h>
#include <stdint.h>
#include "env.h"
#include "driver.h"
#include "module.h"
#include "register_module.h"
#include "resources.h"

#ifdef __cplusplus
 extern "C" {
#endif

bool mounted = 0;

static void scanWifi() {
    WiFi.scanDelete();
    Serial.println("scan start");
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
      Serial.println("no networks found");
    } else {
      Serial.print(n);
      Serial.println(" networks found");
      for (int i = 0; i < n; ++i) {
        if (mounted == 0) {
          WiFi.scanDelete();
          break;
        }
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.print(WiFi.BSSIDstr(i));
        Serial.print(") ");
        switch (WiFi.encryptionType(i)) {
          case WIFI_AUTH_OPEN:
            Serial.println("WIFI_AUTH_OPEN");
            break;
          case WIFI_AUTH_WEP:
            Serial.println("WIFI_AUTH_WEP");
            break;
          case WIFI_AUTH_WPA_PSK:
            Serial.println("WIFI_AUTH_WPA_PSK");
            break;
          case WIFI_AUTH_WPA2_PSK:
            Serial.println("WIFI_AUTH_WPA2_PSK");
            break;
          case WIFI_AUTH_WPA_WPA2_PSK:
            Serial.println("WIFI_AUTH_WPA_WPA2_PSK");
            break;
          case WIFI_AUTH_WPA2_ENTERPRISE:
            Serial.println("");
            break;
            Serial.println("WIFI_AUTH_WPA2_ENTERPRISE");
            break;
          default:
            Serial.println("UNKNOWN");
        }
        delay(10);
      }
    }
    Serial.println("");
}

static void _init(int num, ...) {
  clearSafeArea();
  scanWifi();
}

static void _destroy() {
  WiFi.scanDelete();
}

static void onKeyUp() {}

static void onKeyDown( ) {}

static void onKeyRight( ) {}

static void onKeyLeft() {}

static void onKeyMid() {}

static void onKeySet() {}

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
