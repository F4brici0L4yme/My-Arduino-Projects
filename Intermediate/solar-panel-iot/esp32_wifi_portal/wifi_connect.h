#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

#include <WiFi.h>

bool connectSaved();       // intenta conectar con la red guardada
void checkForgetNetwork(); // ventana de 3s para borrar la red con el boton BOOT

#endif
