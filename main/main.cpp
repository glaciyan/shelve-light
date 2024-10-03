#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"

#include "rainbow.h"
#include "led_pulse.h"

extern "C" void app_main()
{
  initArduino();
  xTaskCreate(pulseTask, "Pulse Task", 1024, NULL, 1, NULL);
  xTaskCreate(rgbLedTask, "Led Task", 1024*2, NULL, 1, NULL);
}
