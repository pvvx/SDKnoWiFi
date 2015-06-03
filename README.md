# esp8266noWiFi
OpenSDK without WiFi ESP8266
SDK to work with sensors.
Startup < 30 ms from reset ESP8266:
Switch off messages boots blocks in ROM-BIOS - change the second byte in the header 0x00000.bin to 0 (from 0xE9).

http://esp8266.ru/forum/threads/revers-sdk-espressif-i-rom-bios-dlja-sozdanija-otkrytogo-sdk.292/

UDK
http://esp8266.ru/forum/forums/devkit/