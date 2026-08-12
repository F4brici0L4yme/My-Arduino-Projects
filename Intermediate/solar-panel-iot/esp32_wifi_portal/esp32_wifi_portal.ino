#include "wifi_connect.h"
#include "wifi_portal.h"
#include "ping.h"

unsigned long lastPing = 0;

void setup() {
  Serial.begin(115200);

  // ventana de 3s para olvidar la red con el boton BOOT
  checkForgetNetwork();

  if (connectSaved()) {
    Serial.println("Conectado a WiFi -> SSID: " + WiFi.SSID() + " | IP: " + WiFi.localIP().toString());
  } else {
    startPortal();
  }
}

void loop() {
  handlePortal(); // solo activo si el portal esta corriendo

  if (WiFi.status() == WL_CONNECTED && millis() - lastPing > 10000) {
    lastPing = millis();
    sendPing();
  }
}
