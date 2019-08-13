  /**
   * Nano with DHT11, ST7735 and WS2812
   * GITHUB: https://github.com/EduardsZ/NanoDHT11ST7735
   * 
   * This thing shows temp and some other info, like bones
   * Assumed to flash with platformio on VS code
   * 
   * PINS:
   * 5  - THIS IS WS2812B LEDs DATA PIN
   * 6  - DATA PIN FROM DHT11
   * 7  - LCD RESET PIN
   * 8  - LCD CS PIN
   * 9  - LCD A0/DC PIN
   * 10 - LCD LED DIMM PIN
   * 11 - LCD SDA PIN
   * 13 - LCD SCK PIN
   * 
   */

  #include <SimpleDHT.h>
  #include <SPI.h>
  #include "Ucglib.h"
  #include <WS2812.h>

  #define LEDS_NUMBER 16
  
  unsigned long mil;
  uint8_t pinDHT11 = 6, dimm = 127;
  uint8_t bgcol[3] = {255,255,255}, bncol[3] = {28,33,127}, btcl1[3] = {0,15,10}, btcl2[3] = {127,0,10};
  char boneSign = '*';
  byte temperature = 0,  humidity = 0 , oldtemp= 0, oldhum = 0;
  cRGB value;

  SimpleDHT11 dht11(pinDHT11);
  // Ucglib_ST7735_18x128x160_SWSPI ucg(/*sclk=*/ 0, /*data=*/ 4, /*cd=*/ 10, /*cs=*/ 2, /*reset=*/ 5);
  Ucglib_ST7735_18x128x160_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 8, /*reset=*/ 7);
  WS2812 LED(16);
  
  void showBones(int number){
    ucg.setColor(bgcol[0], bgcol[1], bgcol[2]); ucg.drawBox(18, 51, 52, 52);
    ucg.setColor(63, 63, 63); ucg.drawRFrame(16, 49, 56, 56, 5);
    ucg.setFont(ucg_font_ncenR12_tr); ucg.setColor(bncol[0], bncol[1], bncol[2]);

    switch (number) {
      case 1: {
        ucg.setPrintPos(40,85); ucg.print(boneSign);
        break;
      }
      case 2: {
        ucg.setPrintPos(20,65); ucg.print(boneSign); ucg.setPrintPos(60,105); ucg.print(boneSign);
        break;
      }
      case 3: {
        ucg.setPrintPos(20,65); ucg.print(boneSign); ucg.setPrintPos(40,85); ucg.print(boneSign);
        ucg.setPrintPos(60,105); ucg.print(boneSign);
        break;
      }
      case 4: {
        ucg.setPrintPos(20,65); ucg.print(boneSign); ucg.setPrintPos(20,105); ucg.print(boneSign);
        ucg.setPrintPos(60,65); ucg.print(boneSign); ucg.setPrintPos(60,105); ucg.print(boneSign);
        break;
      }
      case 5: {
        ucg.setPrintPos(20,65); ucg.print(boneSign); ucg.setPrintPos(20,105); ucg.print(boneSign);
        ucg.setPrintPos(60,65); ucg.print(boneSign); ucg.setPrintPos(60,105); ucg.print(boneSign);
        ucg.setPrintPos(40,85); ucg.print(boneSign);
        break;
      }
      case 6: {
        ucg.setPrintPos(20,65); ucg.print(boneSign); ucg.setPrintPos(40,65); ucg.print(boneSign);
        ucg.setPrintPos(60,65); ucg.print(boneSign); ucg.setPrintPos(20,105); ucg.print(boneSign);
        ucg.setPrintPos(40,105); ucg.print(boneSign); ucg.setPrintPos(60,105); ucg.print(boneSign);
        break;
      }
    }
  }

  void getTH () {
      int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
      return;
    }
    Serial.print("Sample OK: ");
    Serial.print((int)temperature); Serial.print(" *C, "); 
    Serial.print((int)humidity); Serial.println(" %");
  }

  void showDHT () {
    if(oldhum != humidity || oldtemp != temperature) {
      ucg.setFont(ucg_font_ncenR14_tr);
      ucg.setColor(bgcol[0], bgcol[1], bgcol[2]); ucg.drawBox(0, 10, 100, 38);
      ucg.setColor(0,31,0);
      ucg.setPrintPos(10,30);
      ucg.print((String)temperature + " *C");
      ucg.setPrintPos(64,30);
      ucg.print((String)humidity + " %");
      oldhum = humidity; oldtemp = temperature;
    }
  }

  void showBattery (int lvl) {
    ucg.setColor(btcl1[0], btcl1[1], btcl1[2]); 
    if (lvl > 0)ucg.drawBox(116, 3, 2, 4);  
    if (lvl > 1)ucg.drawBox(119, 3, 2, 4);
    if (lvl > 2)ucg.drawBox(122, 3, 2, 4);
    ucg.drawBox(126, 3, 1, 4); ucg.drawFrame(114, 1, 12, 8);
  }
  
  void setup() {
    pinMode(10, OUTPUT);
    Serial.begin(115200);
    Serial.println("Start");
    ucg.begin(UCG_FONT_MODE_TRANSPARENT);
    // ucg.begin(UCG_FONT_MODE_SOLID);
    // ucg.clearScreen();
    ucg.setColor(bgcol[0], bgcol[1], bgcol[2]); ucg.drawBox(0, 0, 128, 160);
    ucg.setFont(ucg_font_ncenR14_tr);
    // ucg.setColor(1, 255, 255,255);

    LED.setOutput(5);

    analogWrite(10, 5); delay(500);
    showBattery(3); delay(500);
    
    getTH();
    showDHT();
    showBones(random(1, 6));

    value.b = 0; value.g = 0; value.r = 0;
    for (int nn = 0; nn < LEDS_NUMBER; nn++){
      LED.set_crgb_at(nn, value);
    }
    LED.sync();
  }
  
  void loop() {
    if (millis() > mil + 15000 || millis() < mil){
      mil = millis()/1000*1000;
      getTH ();
      showDHT ();
      showBones(random(1, 6));

      value.b = 15; value.g = 15; value.r = 15;
      LED.set_crgb_at(0, value);
      LED.sync();
      delay(15);
      value.b = 0; value.g = 15; value.r = 0;
      LED.set_crgb_at(0, value);
      LED.sync();
    }
  }