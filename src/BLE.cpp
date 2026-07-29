#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


bool _BLEClientConnected = false;
#define BatteryService BLEUUID((uint16_t)0x180F)

BLECharacteristic BatteryLevelCharacteristic(BLEUUID((uint16_t)0x2A19), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BatteryLevelDescriptor(BLEUUID((uint16_t)0x2901));


class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        Serial.print("Event-Connect..");//Serial.println(remoteAddress);
        _BLEClientConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
        _BLEClientConnected = false;
        Serial.println("Event-Disconnect..");
        delay(300); // give the bluetooth stack the chance to get things ready
        BLEDevice::startAdvertising();  // restart advertising
    }
};

void ble_init(){
    BLEDevice::init("UPS-PC-WIN11"); //init BLE stack..
    // Create the BLE Server
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    // Create the BLE Service
    BLEService *pBattery = pServer->createService(BatteryService);
    pBattery->addCharacteristic(&BatteryLevelCharacteristic);
    BatteryLevelDescriptor.setValue("Percentage 0 - 100");
    BatteryLevelCharacteristic.addDescriptor(&BatteryLevelDescriptor);
    BatteryLevelCharacteristic.addDescriptor(new BLE2902());
    pServer->getAdvertising()->addServiceUUID(BatteryService);
    pBattery->start();
    // Start advertising
    pServer->getAdvertising()->start();
}

//change BLE service value
void update_battery_level(uint8_t blevel){ 
  BatteryLevelCharacteristic.setValue(&blevel, 1);
  BatteryLevelCharacteristic.notify();
}