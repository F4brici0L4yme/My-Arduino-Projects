# Diccionario de campos - Metadata ESP32

| Campo | Descripción técnica |
|---|---|
| chip_model | Identificador comercial del SoC (System on Chip). |
| chip_revision | Revisión de silicio del die, codificada por el SDK (ej. 301 = rev 3, wafer 01). |
| chip_cores | Cantidad de núcleos físicos Xtensa/RISC-V disponibles para ejecutar tareas. |
| cpu_freq_mhz | Frecuencia de reloj configurada para la CPU, en MHz. |
| feature_wifi | Indica si el chip integra radio WiFi 802.11 b/g/n. |
| feature_bt | Indica si el chip integra Bluetooth Classic. |
| feature_ble | Indica si el chip integra Bluetooth Low Energy. |
| flash_embedded | true si la flash está embebida en el die; false si es un chip externo soldado en la PCB. |
| heap_total_bytes | Tamaño total del heap dinámico gestionado por el allocator de FreeRTOS/ESP-IDF. |
| heap_free_bytes | Memoria heap disponible en el momento de la lectura. |
| heap_used_bytes | Memoria heap actualmente reservada por el firmware (total - libre). |
| heap_min_free_bytes | Mínimo histórico de heap libre desde el arranque; indicador de fugas de memoria. |
| heap_max_alloc_bytes | Bloque contiguo más grande que se puede reservar de una sola vez (mide fragmentación). |
| psram_found | Indica si se detectó un chip de PSRAM externo conectado por SPI/OSPI. |
| flash_size_bytes | Tamaño real de la memoria flash detectada, en bytes. |
| flash_speed_hz | Frecuencia de reloj del bus SPI hacia la flash. |
| flash_mode | Modo de interfaz de la flash (0=QIO, 1=QOUT, 2=DIO, 3=DOUT). |
| sketch_size_bytes | Tamaño del binario del firmware actualmente cargado. |
| sketch_free_space_bytes | Espacio libre en la partición OTA para actualizaciones o crecimiento del binario. |
| mac_address | Dirección MAC de la interfaz WiFi (formato IEEE 802). |
| sdk_version | Versión del ESP-IDF (framework base) con el que se compiló el firmware. |
| freertos_task_count | Número de tareas (hilos) activas registradas en el scheduler de FreeRTOS. |
| freertos_tick_rate_hz | Frecuencia del tick del scheduler, define la resolución mínima de scheduling. |
| loop_core_id | ID del núcleo (0 o 1) donde se está ejecutando la tarea loop() en este momento. |
