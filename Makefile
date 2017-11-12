# Main settings includes
include	settings.mk

# Individual project settings (Optional)
SDK_BASE	= c:/Espressif/ESP8266_RTOS_SDK
#BOOT		= new
#APP		= 1
#SPI_SPEED	= 40
#SPI_MODE	= DIO
#SPI_SIZE_MAP	= 6
#ESPPORT		= COM5
#ESPBAUD		= 256000

# Basic project settings
CFLAGS = -DICACHE_FLASH
MODULES	= code/driver \
			code/projconf \
			code/web \
			code/adapter/memory \
			code/adapter/types \
			code/adapter/debug_adapter \
			code/adapter/gpio_adapter \
			code/adapter/wifi_adapter \
			code/adapter/udp_adapter \
			code/adapter/spiffs_adapter \
			code/adapter/http_adapter \
			code/module/webserver \
			code/testing/testsocket \
			code/testing/testgpio \
			code/testing/testspiffs \
			code/testing/testhttp \
			code/application/wifi_task \
			code 
LIBS	= gcc c hal phy pp net80211 wpa crypto main freertos lwip minic smartconfig espconn spiffs

# Root includes
include	common_rtos.mk
