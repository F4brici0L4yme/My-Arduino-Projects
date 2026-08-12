/*
 * nrfBlueNullifier - Versión con diagnóstico mejorado
 * nRF24L01+PA/LNA en VSPI (CE=22, CSN=21)
 */

#include <RF24.h>
#include <esp_bt.h>
#include <esp_bt_main.h>

SPIClass *sp = nullptr;

// Velocidad SPI más baja y estable (prueba 4MHz primero)
RF24 radio(22, 21, 19909090);   // CE=22, CSN=21, SPI 4 MHz

byte i = 45, ptr_hop = 0, flag = 0;
byte hopping_channel[] = {32, 34, 46, 48, 50, 52, 26, 28, 30, 74, 76, 78};

unsigned long lastStatusPrint = 0;
const unsigned long STATUS_INTERVAL = 2000;

bool radioOK = false;

void nrfSPIInit() {
    Serial.println(F("\n[nRF] === Inicializando nRF24 ==="));
    
    // Crear SPI VSPI
    if (sp == nullptr) {
        sp = new SPIClass(VSPI);
        sp->begin();   // SCK=18, MISO=19, MOSI=23
        Serial.println(F("[nRF] SPI VSPI iniciado (SCK=18, MISO=19, MOSI=23)"));
    }

    Serial.println(F("[nRF] Llamando a radio.begin()..."));
    
    if (radio.begin(sp)) {
        radioOK = true;
        Serial.println(F("[nRF] ¡radio.begin() EXITOSO!"));

        // Configuración para jamming
        radio.setAutoAck(false);
        radio.stopListening();
        radio.setRetries(0, 0);
        radio.setPayloadSize(31);
        radio.setAddressWidth(4);
        radio.setPALevel(RF24_PA_MAX, true);
        radio.setDataRate(RF24_2MBPS);
        radio.setCRCLength(RF24_CRC_DISABLED);

        Serial.println(F("[nRF] Configuración aplicada:"));
        Serial.println(F("  AutoAck OFF | Retries 0 | Payload 31"));
        Serial.println(F("  Address width 4 | PA MAX | 2Mbps | CRC OFF"));

        // Iniciar portadora continua
        radio.startConstCarrier(RF24_PA_MAX, i);
        Serial.print(F("[nRF] Portadora continua iniciada en canal "));
        Serial.println(i);

        // Información útil del chip
        Serial.print(F("[nRF] isChipConnected : "));
        Serial.println(radio.isChipConnected() ? "SÍ" : "NO");
        Serial.print(F("[nRF] isPVariant      : "));
        Serial.println(radio.isPVariant() ? "SÍ (nRF24L01+)" : "NO");

        // Imprimir detalles internos de la librería
        Serial.println(F("\n----- Detalles del radio -----"));
        radio.printDetails();
        Serial.println(F("------------------------------"));
    } 
    else {
        radioOK = false;
        Serial.println(F("[nRF] ¡ERROR! radio.begin() FALLÓ"));
        Serial.println(F("[nRF] Posibles causas:"));
        Serial.println(F("  1. Falta condensador 10-100uF en VCC-GND del nRF24"));
        Serial.println(F("  2. Alimentación insuficiente (usa 3.3V estable)"));
        Serial.println(F("  3. Cables sueltos o mal contacto"));
        Serial.println(F("  4. Módulo defectuoso"));
        Serial.println(F("Pines: CE=22 | CSN=21 | SCK=18 | MOSI=23 | MISO=19"));
    }
}

void adjustAndSweepChannels() {
    flag = (i > 79) ? 1 : (i < 2 ? 0 : flag);
    i += flag ? -2 : 2;

    for (int j = 0; j <= 79; j++) {
        radio.setChannel(j);
    }
}

void printStatus() {
    Serial.println(F("\n---------- ESTADO ----------"));
    Serial.print(F("Radio OK          : "));
    Serial.println(radioOK ? "SÍ" : "NO");

    if (radioOK) {
        Serial.print(F("Canal hop actual  : "));
        Serial.print(hopping_channel[ptr_hop]);
        Serial.print(F("  (índice "));
        Serial.print(ptr_hop);
        Serial.println(F(")"));

        Serial.print(F("Variable barrido i: "));
        Serial.println(i);

        Serial.print(F("Chip conectado    : "));
        Serial.println(radio.isChipConnected() ? "SÍ" : "NO");
    }

    Serial.print(F("Heap libre        : "));
    Serial.print(ESP.getFreeHeap());
    Serial.println(F(" bytes"));

    Serial.print(F("Uptime            : "));
    Serial.print(millis() / 1000);
    Serial.println(F(" s"));
    Serial.println(F("----------------------------"));
}

void setup() {
    Serial.begin(115200);
    delay(800);

    Serial.println();
    Serial.println(F("========================================"));
    Serial.println(F("   nrfBlueNullifier - Diagnóstico"));
    Serial.println(F("========================================"));
    Serial.print(F("Chip          : "));
    Serial.println(ESP.getChipModel());
    Serial.print(F("CPU           : "));
    Serial.print(ESP.getCpuFreqMHz());
    Serial.println(F(" MHz"));
    Serial.print(F("Heap inicial  : "));
    Serial.print(ESP.getFreeHeap());
    Serial.println(F(" bytes"));
    Serial.println();

    // Desactivar Bluetooth
    Serial.println(F("[BT] Desactivando Bluetooth..."));
    esp_bt_controller_deinit();

    if (esp_bluedroid_get_status() == ESP_BLUEDROID_STATUS_ENABLED) {
        esp_bluedroid_disable();
        esp_bluedroid_deinit();
        Serial.println(F("[BT] Bluedroid desactivado"));
    } else {
        Serial.println(F("[BT] Bluedroid ya estaba desactivado"));
    }

    // Inicializar nRF24
    nrfSPIInit();

    if (radioOK) {
        Serial.println(F("\n[OK] Setup terminado. Iniciando jamming..."));
        Serial.println(F("[INFO] Estado cada 2 segundos"));
    } else {
        Serial.println(F("\n[ERROR] No se pudo inicializar el nRF24"));
        Serial.println(F("Revisa alimentación + condensador y vuelve a subir"));
    }
}

void loop() {
    if (!radioOK) {
        // Reintentar cada 3 segundos si falló
        if (millis() - lastStatusPrint >= 3000) {
            Serial.println(F("\n[ERROR] Reintentando radio.begin()..."));
            nrfSPIInit();
            lastStatusPrint = millis();
        }
        delay(200);
        return;
    }

    // Lógica de jamming
    adjustAndSweepChannels();
    ptr_hop = (ptr_hop + 1) % sizeof(hopping_channel);
    radio.setChannel(hopping_channel[ptr_hop]);

    // Estado periódico
    if (millis() - lastStatusPrint >= STATUS_INTERVAL) {
        printStatus();
        lastStatusPrint = millis();
    }
}