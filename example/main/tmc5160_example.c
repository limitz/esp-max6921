#include <max6921.h>

void app_main()
{
	MAX6921.init();
	MAX6921.enable(true);

	MAX6921.update();
	vTaskDelay(1500);
	
	MAX6921.update();
	vTaskDelay(1500);
	
	MAX6921.deinit();
}
