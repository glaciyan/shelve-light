#pragma once

#include "Arduino.h"
#include "led/led_driver.h"
#include "led/led_pixel.h"
#include "sleep_util.h"

#define PWR_PIN 19 // power pin for rgb led
constexpr gpio_num_t RGBLED_PIN = GPIO_NUM_20;
#define LED_COUNT 1
#define LED_SIZE 24 * LED_COUNT

static const char *RAINBOW_TAG = "rainbow";

rmt_data_t led_data[LED_SIZE]; // Array to store the pulse sequence data
float hue = 0.0;               // Hue value for the rainbow animation

void rgbLedTask(void *parameters)
{
    ESP_LOGI(RAINBOW_TAG, "Writing UP to PWR pin");
    pinMode(PWR_PIN, OUTPUT);
    digitalWrite(PWR_PIN, 1);

    ESP_LOGI(RAINBOW_TAG, "Initializing driver");
    led::Driver<RGBLED_PIN> ledDriver{};

    ESP_LOGI(RAINBOW_TAG, "Displaying Test pixels");
    led::Pixel red{255, 0, 0};
    ledDriver.transmitData(&red, sizeof(red));
    SLEEP(500);

    led::Pixel green{0, 255, 0};
    ledDriver.transmitData(&green, sizeof(green));
    SLEEP(500);

    led::Pixel blue{0, 0, 255};
    ledDriver.transmitData(&blue, sizeof(blue));
    SLEEP(500);

    led::Pixel white{255, 255, 255};
    ledDriver.transmitData(&white, sizeof(white));
    SLEEP(500);

    while (true)
    {
        TickType_t time = xTaskGetTickCount();
        double hue = (std::sin(time / 500.0) * 180) + 180;
        led::Pixel pixel = led::Pixel::pixelFromHsv(hue, 100, 100);

        std::array<led::Pixel, LED_COUNT> data{{pixel}};

        ledDriver.transmitData(data.data(), sizeof(pixel) * data.size());
        ledDriver.joinAll();

        SLEEP(16);
    }
}
