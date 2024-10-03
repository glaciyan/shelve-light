#pragma once

#include "Arduino.h"
#include "ledc_util.h"
#include "functions.h"
#include "sleep_util.h"

#define BLUE_LED_PIN 7
#define LEDC_BASE_FREQ 5000

void pulseTask(void *parameters)
{
    const uint32_t period = 1000;   // Controls the speed of brightness cycle
    const uint32_t amplitude = 255; // Max brightness value (0-255)

    ledcAttach(BLUE_LED_PIN, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);

    while (true)
    {
        TickType_t time = xTaskGetTickCount();
        uint32_t brightness = zigzag(time, period, amplitude);
        ledcAnalogWrite(BLUE_LED_PIN, brightness, amplitude);
        SLEEP(16);
    }
}