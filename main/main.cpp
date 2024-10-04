#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_gap_ble_api.h"
#include "Arduino.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include "rainbow.h"
#include "led_pulse.h"

BLEServer *pServer = nullptr;
BLECharacteristic *pCharacteristic = nullptr;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks : public BLEServerCallbacks
{
private:
    void onConnect(BLEServer *bleServer) override
    {
        deviceConnected = true;
        BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer *bleServer) override
    {
        deviceConnected = false;
    }
};

const static char *TAG = "app_main";

void setup()
{
    initArduino();

    xTaskCreate(pulseTask, "Pulse Task", 1024, nullptr, 1, nullptr);
    xTaskCreate(rgbLedTask, "Led Task", 1024 * 2, nullptr, 1, nullptr);

    BLEDevice::init("ESP32C6");

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_INDICATE
    );

    pCharacteristic->addDescriptor(new BLE2902());

    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    ESP_LOGI(TAG, "Waiting a client connection to notify...");
}

void loop()
{
    if (deviceConnected)
    {
        pCharacteristic->setValue((uint8_t *) &value, 4);
        pCharacteristic->notify();
        value++;
        SLEEP(10);  // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
        SLEEP(500);                   // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising();  // restart advertising
        ESP_LOGI(TAG, "start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
