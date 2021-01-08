#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_system.h>
#include <esp_log.h>
#include <driver/spi_master.h>
#include <driver/gpio.h>

#pragma pack(push,1)

#define MAX6921_SPI_USE_GPIO CONFIG_LMTZ_MAX6921_SPI_USE_GPIO

#if (CONFIG_LMTZ_MAX6921_SPI_HSPI)
#define MAX6921_SPI_HOST HSPI_HOST
#if !MAX6921_SPI_USE_GPIO
#define MAX6921_PIN_MOSI 13
#define MAX6921_PIN_MISO 12
#define MAX6921_PIN_SCK  14
#define MAX6921_PIN_CS   15
#endif
#elif (CONFIG_LMTZ_MAX6921_SPI_VSPI)
#define MAX6921_SPI_HOST VSPI_HOST
#if !MAX6921_SPI_USE_GPIO
#define MAX6921_PIN_MOSI 23
#define MAX6921_PIN_MISO 19
#define MAX6921_PIN_SCK  18
#define MAX6921_PIN_CS   5
#endif
#endif

#if MAX6921_SPI_USE_GPIO
#define MAX6921_PIN_MOSI CONFIG_LMTZ_MAX6921_PIN_MOSI
#define MAX6921_PIN_MISO CONFIG_LMTZ_MAX6921_PIN_MISO
#define MAX6921_PIN_SCK  CONFIG_LMTZ_MAX6921_PIN_SCK
#define MAX6921_PIN_CS   CONFIG_LMTZ_MAX6921_PIN_CS
#endif

#define MAX6921_PIN_EN CONFIG_LMTZ_MAX6921_PIN_EN
#define MAX6921_CLOCK_SPEED CONFIG_LMTZ_MAX6921_CLOCK_SPEED

typedef struct
{
	int (*init)();
	int (*deinit)();
	int (*update)();
	int (*enable)(bool en);
	
	int spi_host;

	struct
	{
		union { int8_t MOSI, DIN; };
		union { int8_t MISO, DOUT; };
		union { int8_t SCK; };
		union { int8_t CS, LOAD }; // LOAD must be high during transmission
		int8_t EN;
	} pins;


	spi_bus_config_t bus_config;
	spi_device_interface_config_t dev_config;
	spi_device_handle_t device;

} max6921_driver_t;

#define MAX6921_SPI_MODE 3

extern max6921_driver_t MAX6921;

#pragma pack(pop)
