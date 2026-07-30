#include <Arduino.h>
//#include <BLEDevice.h>
//#include <BLEServer.h>
//#include <BLEUtils.h>
//#include <BLE2902.h>
extern void ble_battery_init();
extern void ble_term_init();
extern void update_battery_level(uint8_t blevel);


void setup() {

    Serial.begin(115200);

    delay(1000);
    Serial.println();
    Serial.println("----------------Start Info---------------");
    Serial.printf("Total heap:\t%d \r\n", ESP.getHeapSize());
    Serial.printf("Free heap:\t%d \r\n", ESP.getFreeHeap());
    Serial.printf("Total PSRAM:\t%d \r\n", ESP.getPsramSize());
    Serial.printf("Free PSRAM:\t%d \r\n", ESP.getFreePsram());
    Serial.printf("Flash size:\t%d (bytes)\r\n", ESP.getFlashChipSize());
    Serial.println("-----------------------------------------");

    //ble_battery_init();
    ble_term_init();

    Serial.println("OK!-START..");

}

uint8_t battery_level = 0;

void loop() {
  //update_battery_level(battery_level);  //change BLE service value
  //Serial.println(int(battery_level));

  delay(5000);

  battery_level++;
  if (int(battery_level) == 100)
    battery_level = 0;

}

