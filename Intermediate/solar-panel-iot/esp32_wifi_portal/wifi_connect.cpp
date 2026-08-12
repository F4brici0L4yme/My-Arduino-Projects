#include "wifi_connect.h"
#include <Preferences.h>

#define BOOT_BTN 0

bool connectSaved() {
  Preferences prefs;
  prefs.begin("wifi", true);
  String s = prefs.getString("s", "");
  String p = prefs.getString("p", "");
  prefs.end();

  if (s == "") return false;

  WiFi.mode(WIFI_STA);
  WiFi.begin(s.c_str(), p.c_str());
  for (int i = 0; i < 20 && WiFi.status() != WL_CONNECTED; i++) delay(500);

  return WiFi.status() == WL_CONNECTED;
}

void checkForgetNetwork() {
  pinMode(BOOT_BTN, INPUT_PULLUP);

  Serial.println("Manten presionado BOOT ahora para olvidar la red (3s)...");
  unsigned long start = millis();
  bool forget = false;
  while (millis() - start < 3000) {
    if (digitalRead(BOOT_BTN) == LOW) {
      forget = true;
      break;
    }
  }

  if (forget) {
    Preferences prefs;
    prefs.begin("wifi", false);
    prefs.clear();
    prefs.end();
    Serial.println("Red WiFi olvidada.");
  }
}
