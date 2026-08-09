#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIN_LED      21 // Пин, к которому подключен RGB светодиод
#define NUM_LEDS      1 // Количество светодиодов на плате

// Инициализируем объект NeoPixel
Adafruit_NeoPixel rgb_led(NUM_LEDS, PIN_LED, NEO_GRB + NEO_KHZ800);

extern void ble_term_init();
extern void update_battery_level(uint8_t blevel);


void set_blue(){
    rgb_led.setPixelColor(0, rgb_led.Color(0, 0, 255));
    rgb_led.show();
}
void set_green(){
    rgb_led.setPixelColor(0, rgb_led.Color(0, 255, 0));
    rgb_led.show();
}


void setup() {
    delay(7000);  //10 sec for Platformio start terminal...

    Serial.begin(115200);

    Serial.println();
    Serial.println("----------------Start Info---------------");
    Serial.printf("Total heap:\t%d \r\n", ESP.getHeapSize());
    Serial.printf("Free heap:\t%d \r\n", ESP.getFreeHeap());
    Serial.printf("Total PSRAM:\t%d \r\n", ESP.getPsramSize());
    Serial.printf("Free PSRAM:\t%d \r\n", ESP.getFreePsram());
    Serial.printf("Flash size:\t%d (bytes)\r\n", ESP.getFlashChipSize());
    Serial.println("-----------------------------------------");

    rgb_led.begin();           // Инициализация светодиода
    rgb_led.setBrightness(30); // Установка яркости от 0 до 255
    set_blue();

    ble_term_init();  //test Nordic UART Service (NUS)

    Serial.println("OK!-START..");

}

uint8_t battery_level = 0;

void loop() {
  update_battery_level(battery_level);  //change Battery Service value
  Serial.println(int(battery_level));

  delay(5000);

  battery_level++;
  if (int(battery_level) == 100)
    battery_level = 0;

}

