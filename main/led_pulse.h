#pragma once

#include "Arduino.h"
#include "functions.h"
#include "sleep_util.h"
#include "led/led_driver.h"

constexpr uint8_t BLUE_LED_PIN = GPIO_NUM_7;

void pulseTask(void *parameters)
{
    const uint32_t period = 1000;   // Controls the speed of brightness cycle
    const uint32_t amplitude = 255; // Max brightness value (0-255)

    ledc::Driver<BLUE_LED_PIN> driver{};

    while (true)
    {
        TickType_t time = xTaskGetTickCount();
        uint32_t brightness = zigzag(time, period, amplitude);
        driver.transmit(brightness, amplitude);
        SLEEP(16);
    }
}