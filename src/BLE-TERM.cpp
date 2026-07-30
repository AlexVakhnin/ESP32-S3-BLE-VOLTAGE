#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

//Nordic UART Service (NUS)
#define Service_Term_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" //16-bit offset: 0x0001
#define Characteristic_Rx_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E" //16-bit offset: 0x0002 (Write)
#define Characteristic_Tx_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E" //16-bit offset: 0x0003 (Notify)


bool _BLETermConnected = false;

//Create Characteristics with their UUID and Properties
BLECharacteristic pTxCharacteristic(Characteristic_Tx_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLECharacteristic pRxCharacteristic(Characteristic_Rx_UUID, BLECharacteristic::PROPERTY_WRITE);

//Connect, Disconnect handling
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        Serial.println("Event-Connect..");
        _BLETermConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
        _BLETermConnected = false;
        Serial.println("Event-Disconnect..");
        delay(300); // give the bluetooth stack the chance to get things ready
        BLEDevice::startAdvertising();  // restart advertising
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        if (value.length() > 0) {
            Serial.print("Received: ");
            for (int i = 0; i < value.length(); i++) Serial.print(value[i]);
            
            //echo back to terminal
            pTxCharacteristic.setValue(value);
            Serial.print("Sent: "+String(value.c_str()));
            pTxCharacteristic.notify();
        }
    }
};



void ble_term_init(){
    BLEDevice::init("UPS-PC-WIN11-TERM"); //init BLE stack..

    BLEServer *pServer = BLEDevice::createServer();  //Srever create
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pTerm = pServer->createService(Service_Term_UUID);  //Service create
    pTerm->addCharacteristic(&pTxCharacteristic);  //Characteristic Tx (Notify)
    pTxCharacteristic.addDescriptor(new BLE2902());  //need for notifications control from client side
    pTerm->addCharacteristic(&pRxCharacteristic);  //Characteristic Rx (Write)
    pRxCharacteristic.setCallbacks(new MyCallbacks()); //set callback for terminal


    pServer->getAdvertising()->addServiceUUID(Service_Term_UUID);  //Advertising init
    pTerm->start(); //Service start
    pServer->getAdvertising()->start(); //Advertising start
}

