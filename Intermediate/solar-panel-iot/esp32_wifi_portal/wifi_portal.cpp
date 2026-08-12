#include "wifi_portal.h"
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>

static WebServer server(80);
static DNSServer dns;
static bool portalActive = false;

void startPortal() {
  String opt = "";
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) opt += "<option>" + WiFi.SSID(i) + "</option>";

  // pagina principal: selector de red + contrasena
  server.on("/", [opt]() {
    String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'></head>"
      "<body style='display:flex;justify-content:center;align-items:center;height:100vh;margin:0;font-family:sans-serif;font-size:20px'>"
      "<form action='/save' method='POST' style='text-align:center;width:90%;max-width:400px'>"
      "<h1>Configurar WiFi</h1>"
      "<select name='s' style='width:100%;padding:14px;font-size:20px;margin-bottom:16px'>" + opt + "</select><br>"
      "<input type='password' id='p' name='p' placeholder='Contrasena' style='width:100%;padding:14px;font-size:20px;margin-bottom:16px;box-sizing:border-box'><br>"
      "<label style='font-size:18px'><input type='checkbox' onclick=\"p.type=p.type=='password'?'text':'password'\" style='width:20px;height:20px;vertical-align:middle'> Mostrar contrasena</label><br><br>"
      "<input type='submit' value='Conectar' style='width:100%;padding:16px;font-size:22px'>"
      "</form></body></html>";
    server.send(200, "text/html", html);
  });

  // guarda credenciales y reinicia
  server.on("/save", HTTP_POST, []() {
    Preferences prefs;
    prefs.begin("wifi", false);
    prefs.putString("s", server.arg("s"));
    prefs.putString("p", server.arg("p"));
    prefs.end();

    server.send(200, "text/html",
      "<body style='display:flex;justify-content:center;align-items:center;height:100vh;font-family:sans-serif;font-size:22px'>Guardado. Reiniciando...</body>");
    delay(1000);
    ESP.restart();
  });

  // cualquier peticion desconocida redirige al portal (activa aviso automatico en el celular)
  server.onNotFound([]() {
    server.send(200, "text/html", "<meta http-equiv='refresh' content='0;url=/'>");
  });

  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32-Config");
  dns.start(53, "*", WiFi.softAPIP());
  server.begin();

  portalActive = true;
  Serial.println("Access Point creado correctamente: ESP32-Config");
}

void handlePortal() {
  if (!portalActive) return;
  dns.processNextRequest();
  server.handleClient();
}
