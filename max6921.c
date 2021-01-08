#include <max6921.h>

#define TAG "MAX6921"

static int max6921_init()
{
	MAX6921.bus_config.max_transfer_sz = 20; 
	MAX6921.bus_config.miso_io_num = MAX6921.pins.MISO; 
	MAX6921.bus_config.mosi_io_num = MAX6921.pins.MOSI; 
	MAX6921.bus_config.sclk_io_num = MAX6921.pins.SCK;
	MAX6921.dev_config.spics_io_num = MAX6921.pins.CS;

	ESP_LOGI(TAG, "PIN: MOSI = %d", MAX6921.pins.MOSI);
	ESP_LOGI(TAG, "PIN: MISO = %d", MAX6921.pins.MISO);
	ESP_LOGI(TAG, "PIN: SCK  = %d", MAX6921.pins.SCK);
	ESP_LOGI(TAG, "PIN: CS   = %d", MAX6921.pins.CS);

	if (MAX6921.pins.EN >= 0)
	{
		ESP_LOGI(TAG, "PIN: EN   = %d", MAX6921.pins.EN);

		gpio_config_t config = {
			.pin_bit_mask = 1 << MAX6921.pins.EN,
			.mode = GPIO_MODE_OUTPUT,
			.pull_up_en = GPIO_PULLUP_ENABLE,
		};

		gpio_reset_pin(MAX6921.pins.EN);
		gpio_config(&config);
	}

	spi_bus_initialize(MAX6921.spi_host, &MAX6921.bus_config, 0);
	spi_bus_add_device(MAX6921.spi_host, &MAX6921.dev_config, &MAX6921.device);

	return ESP_OK;
}

static int max6921_enable(bool en)
{
	if (MAX6921.pins.EN >= 0)
	{
		gpio_set_level(MAX6921.pins.EN, en ? 0 : 1);
	}
	return ESP_OK;
}

static int max6921_update()
{
	spi_transaction_t t = {
		.length = 20,
		.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA,
		.tx_data = { 0xAA, 0xAA, 0xAE, 0xEE },
	};
	spi_device_polling_transmit(MAX6921.device, &t);

	ESP_LOGI(TAG, "SPI result DOUT: %02X %02X %02X %02X", t.rx_data[0], t.rx_data[1], t.rx_data[2], t.rx_data[3]);

	return ESP_OK;
}

static int max6921_deinit()
{
	spi_bus_remove_device(MAX6921.device);
	spi_bus_free(MAX6921.spi_host);
	return ESP_OK;
}

max6921_driver_t MAX6921 = { 
	.device = 0, 
	.spi_host = MAX6921_SPI_HOST, 
	
	.bus_config = { 
		.miso_io_num = MAX6921_PIN_MISO, 
		.mosi_io_num = MAX6921_PIN_MOSI, 
		.sclk_io_num = MAX6921_PIN_SCK, 
		.quadwp_io_num = -1, 
		.quadhd_io_num = -1, 
		.flags =  SPICOMMON_BUSFLAG_MASTER
#if !MAX6921_SPI_USE_GPIO
			| SPICOMMON_BUSFLAG_IOMUX_PINS 
#endif
			| SPICOMMON_BUSFLAG_SCLK 
			| SPICOMMON_BUSFLAG_MOSI 
			| SPICOMMON_BUSFLAG_MISO,
	}, 
	
	.dev_config = { 
		.clock_speed_hz = MAX6921_CLOCK_SPEED, 
		.mode = 3, 
		.spics_io_num = MAX6921_PIN_CS,
		.queue_size = 1, 
		.flags = SPI_DEVICE_NO_DUMMY | SPI_DEVICE_POSITIVE_CS,
	},

	.pins = 
	{
		.EN   = MAX6921_PIN_EN,
		.MISO = MAX6921_PIN_MISO,
		.MOSI = MAX6921_PIN_MOSI,
		.SCK  = MAX6921_PIN_SCK,
		.CS   = MAX6921_PIN_CS,
	},

	.init = max6921_init,
	.enable = max6921_enable,
	.update = max6921_update,
	.deinit = max6921_deinit,
};

