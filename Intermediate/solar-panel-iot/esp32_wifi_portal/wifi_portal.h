#ifndef WIFI_PORTAL_H
#define WIFI_PORTAL_H

#include <WiFi.h>

void startPortal();   // levanta el access point y la pagina web
void handlePortal();  // atiende peticiones DNS/HTTP, llamar en loop()

#endif
