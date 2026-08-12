#include "ping.h"
#include <HTTPClient.h>

// URL del VPS, cambiar si es necesario
const char* SERVER_URL = "https://hidranix.phinix.com.pe/?esp32=ping";

void sendPing() {
  HTTPClient http;
  http.begin(SERVER_URL);
  int code = http.GET();
  Serial.println("Ping enviado, respuesta: " + String(code));
  http.end();
}
