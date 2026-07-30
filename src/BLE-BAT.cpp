#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

//Bluetooth 16-bit UUID Numbers (from revision 2022-05-19)
//0x180F Battery
//0x2A19 Battery Level
//0x2901 Characteristic User Description
//0x2902 Client Characteristic Configuration


bool _BLEClientConnected = false;
#define BatteryServiceUUID BLEUUID((uint16_t)0x180F) //set Service UUID=>"Battery"

//Create Characteristic with their UUID and Properties=(R,N)
BLECharacteristic BatteryLevelCharacteristic(BLEUUID((uint16_t)0x2A19),
                                             BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BatteryLevelDescriptor(BLEUUID((uint16_t)0x2901)); //Create Descriptor

//Callback functions
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        Serial.println("Event-Connect..");
        _BLEClientConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
        _BLEClientConnected = false;
        Serial.println("Event-Disconnect..");
        delay(300); // give the bluetooth stack the chance to get things ready
        BLEDevice::startAdvertising();  // restart advertising
    }
};

void ble_battery_init(){
    BLEDevice::init("UPS-PC-WIN11"); //init BLE stack..
    // Create the BLE Server
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pBattery = pServer->createService(BatteryServiceUUID);  //Service create
    pBattery->addCharacteristic(&BatteryLevelCharacteristic);  //Characteristic add
    BatteryLevelDescriptor.setValue("Percentage 0 - 100");  //Descriptor init
    BatteryLevelCharacteristic.addDescriptor(&BatteryLevelDescriptor);  //Descriptor add
    BatteryLevelCharacteristic.addDescriptor(new BLE2902());  //need for notifications control
    pServer->getAdvertising()->addServiceUUID(BatteryServiceUUID);  //Advertising init
    pBattery->start(); //Service start
    pServer->getAdvertising()->start(); //Advertising start
}

//change BLE service value
void update_battery_level(uint8_t blevel){ 
  BatteryLevelCharacteristic.setValue(&blevel, 1);
  BatteryLevelCharacteristic.notify();
}
