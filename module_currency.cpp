#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <pgmspace.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "env.h"
#include "driver.h"
#include "types.h"
#include "register_module.h"
#include "resources.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define NUM_MENU 3

typedef struct currency {
  int idx;
  char *code;
  char *rate;
} Currency;

static char from[27];
static char to[27];
static char amount[27];
static uint8_t menu_index = 0;

static const char* BaltimoreCyberTrustRoot PROGMEM = \
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n" \
  "RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n" \
  "VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n" \
  "DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n" \
  "ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n" \
  "VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n" \
  "mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n" \
  "IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n" \
  "mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n" \
  "XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n" \
  "dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n" \
  "jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n" \
  "BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n" \
  "DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n" \
  "9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n" \
  "jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n" \
  "Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n" \
  "ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n" \
  "R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n" \
  "-----END CERTIFICATE-----\n";
  
static void drawField(uint8_t index, char *key, char *value, uint8_t x) {
  if (menu_index == index) {
    LCD.fillRect(0, x, 160, 20, TFT_BLACK);
    LCD.setTextColor(TFT_BG, TFT_BLACK);
  } else {
    LCD.fillRect(0, x, 160, 20, TFT_BG);
    LCD.setTextColor(TFT_BLACK, TFT_BG);
  }
  LCD.setFreeFont(&FreeSans9pt7b);
  LCD.setTextFont(2);
  LCD.drawString(key, 1, x + 1);
  LCD.drawString(value, TFT_W - LCD.textWidth(value) - 1, x + 1);
}

static void drawMenu() {
  clearDisplaySafeArea();
  drawField(0, "Amount", amount, (20 * 0) + 11);
  drawField(1, "From", from, (20 * 1) + 11);
  drawField(2, "To", to, (20 * 2) + 11);
}

static void setValueCallback(char *text) {
  if (menu_index == 0) {
    snprintf(amount, 27, "%s", text);
  } else if (menu_index == 1) {
    snprintf(from, 27, "%s", text);
    for (uint8_t i=0;i<strlen(from);i++) {
      from[i] = toupper(*(from + i));
    }
  } else if (menu_index == 2) {
    snprintf(to, 27, "%s", text);
    for (uint8_t i=0;i<strlen(to);i++) {
      to[i] = toupper(*(to + i));
    }
  }
  GetActiveModule().Destroy();
  ModuleSwitcher(GetModuleCurrency());
  GetActiveModule().Init(0);
}

static void convert(char *_from, char *_to, char *_amount) {
  clearDisplaySafeArea();
  char url[200];
  if (strcmp(_from, "USD") == 0) {
    sprintf(url, "GET https://api.exchangerate.host/latest?amount=%s&base=USD&symbols=%s,EUR,JPY,GBP,CHF,CAD,AUD,NZD&format=tsv&places=2 HTTP/1.0", _amount, _to);
  } else {
    sprintf(url, "GET https://api.exchangerate.host/latest?amount=%s&base=%s&symbols=%s,USD,EUR,GBP,CHF,CAD,AUD,NZD&format=tsv&places=2 HTTP/1.0", _amount, _from, _to);
  }

  WiFiClientSecure client;

  client.setCACert(BaltimoreCyberTrustRoot);
  LCD.setFreeFont(&FreeSans9pt7b);
  LCD.setTextFont(1);
  LCD.drawString(F("Connecting..."), floor((TFT_W - LCD.textWidth(F("Connecting..."))) / 2), 35);
  if (!client.connect("api.exchangerate.host", 443)) {
    LCD.setFreeFont(&FreeSans9pt7b);
    LCD.setTextFont(1);
    LCD.drawString(F("Connection failed!"), floor((TFT_W - LCD.textWidth(F("Connection failed!"))) / 2), 35);
  } else {
    Currency *curencies = (Currency*) malloc(0 * sizeof(Currency));
    uint8_t line_number = 0;
    uint8_t tab_number = 0;
    char code[4];
    uint8_t code_index = 0;
    char rate[10];
    uint8_t rate_index = 0;
    clearDisplaySafeArea();
    client.println(url);
    client.println("Host: api.exchangerate.host");
    client.println("Connection: close");
    client.println();

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }
    while (client.available()) {
      char c = client.read();
      if (line_number > 0) {
        if (tab_number == 0) {
          if (c == '\t') {
            code[code_index] = '\0';
          } else {
            code[code_index++] = c;
          }
        } else if (tab_number == 1) {
          if (c == '\t') {
            rate[rate_index] = '\0';
          } else {
            if (c == ',') {
              rate[rate_index++] = '.';
            } else {
              rate[rate_index++] = c;
            }
          }
        }
        if (c == '\t') {
          tab_number++;
        } else if (c == '\n') {
          curencies = (Currency*) realloc(curencies, line_number * sizeof(Currency));
          curencies[line_number - 1].idx = line_number;
          curencies[line_number - 1].code = (char*)malloc(strlen(code) * sizeof(char*));
          sprintf(curencies[line_number - 1].code, "%s", code);
          curencies[line_number - 1].rate = (char*)malloc(strlen(rate) * sizeof(char*));
          sprintf(curencies[line_number - 1].rate, "%s", rate);
          memset(&code[0], 0, sizeof(code));
          memset(&rate[0], 0, sizeof(rate));
        }
      }
      if (c == '\n') {
        line_number++;
        tab_number = 0;
        code_index = 0;
        rate_index = 0;
      }
    }
    client.stop();
    uint8_t pad_top = 10;
    for (int i=0;i<line_number-1;i++) {
      LCD.setFreeFont(&FreeSans9pt7b);
      LCD.setTextFont(1);
      LCD.drawString(curencies[i].code, 1, pad_top);
      LCD.drawString(curencies[i].rate, TFT_W - LCD.textWidth(curencies[i].rate) - 1, pad_top);
      pad_top += 9;
      free(curencies[i].code);
      free(curencies[i].rate);
    }
    free(curencies);
  }
}

static void _init(int num, ...) {
  if (strlen(from) == 0) {
    sprintf(from, "%s", "USD");
  }
  if (strlen(to) == 0) {
    sprintf(to, "%s", "JPY");
  }
  if (strlen(amount) == 0) {
    sprintf(amount, "%s", "1.00");
  }
  drawMenu();
}

static void _destroy() {}

static void onKeyUp() {
  if (menu_index - 1 < 0) {
    menu_index = NUM_MENU - 1;
  } else {
    menu_index--;
  }
  drawMenu();
}

static void onKeyDown( ) {
  menu_index++;
  if (menu_index >= NUM_MENU) {
    menu_index = 0;
  }
  drawMenu();
}

static void onKeyRight( ) {}

static void onKeyLeft() {}

static void onKeyMid() {
  switch (menu_index) {
    case 0:
      ModuleSwitcher(GetModuleKeyboardUI());
      GetActiveModule().Init(3, amount, 27, setValueCallback);
      break;
    case 1:
      ModuleSwitcher(GetModuleKeyboardUI());
      GetActiveModule().Init(3, from, 27, setValueCallback);
      break;
    case 2:;
      ModuleSwitcher(GetModuleKeyboardUI());
      GetActiveModule().Init(3, to, 27, setValueCallback);
      break;
  }
}

static void onKeySet() {
  if (strlen(from) == 0) {
    sprintf(from, "%s", "Required!");
    drawMenu();
    return;
  }
  if (strlen(to) == 0) {
    sprintf(to, "%s", "Required!");
    drawMenu();
    return;
  }
  if (strlen(amount) == 0) {
    sprintf(amount, "%s", "Required!");
    drawMenu();
    return;
  }
  bool err = 0;
  uint8_t decimal = 0;
  for (uint8_t i=0;i<strlen(amount);i++) {
    if (amount[i] == '.') {
      decimal++;
      if (decimal > 1) {
        err = 1;
        break;
      } else {
        continue;
      }
    }
    if (amount[i] > '9' || amount[i] < '0') {
      err = 1;
      break;
    }
  }
  if (err) {
    sprintf(amount, "%s", "Invalid!");
    drawMenu();
    return;
  }
  convert(from, to, amount);
}

static void onKeyReset( ) {
  GetActiveModule().Destroy();
  ModuleSwitcher(GetModuleMenu());
  GetActiveModule().Init(0);
  memset(&from[0], 0, sizeof(from));
  memset(&to[0], 0, sizeof(to));
  memset(&amount[0], 0, sizeof(amount));
  menu_index = 0;
}

static Module currency = { "CURRENCY", _init, _destroy, onKeyUp, onKeyDown,onKeyRight, onKeyLeft, onKeyMid, onKeySet, onKeyReset };

Module GetModuleCurrency() {
  return currency;
}

#ifdef __cplusplus
}
#endif
