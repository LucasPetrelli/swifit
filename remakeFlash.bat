esptool -p COM9 erase_flash 
esptool -p COM9 write_flash -fm dio 0x0 "C:\Espressif\ESP8266_RTOS_SDK\bin\boot_v1.6.bin" 0x3fc000 "C:\Espressif\ESP8266_RTOS_SDK\bin\esp_init_data_default.bin"