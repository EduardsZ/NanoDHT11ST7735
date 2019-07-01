#include <SimpleDHT.h>
#include <SPI.h>
#include "Ucglib.h"
 
unsigned long mil;
int pinDHT11 = 6, bgcol[3] = {0,10,10};
char boneSign = '*';
byte temperature = 0,  humidity = 0 , oldtemp= 0, oldhum = 0;
SimpleDHT11 dht11(pinDHT11);
// Ucglib_ST7735_18x128x160_SWSPI ucg(/*sclk=*/ 0, /*data=*/ 4, /*cd=*/ 10, /*cs=*/ 2, /*reset=*/ 5);
Ucglib_ST7735_18x128x160_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 8, /*reset=*/ 7);
 
 
void showBones(int number){
  ucg.setColor(bgcol[0], bgcol[1], bgcol[2]); ucg.drawBox(18, 51, 52, 52);
  ucg.setColor(63, 63, 63); ucg.drawRFrame(16, 49, 56, 56, 5);
  ucg.setFont(ucg_font_ncenR12_tr); ucg.setColor(28, 208, 255);

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
  Serial.println("Show Bones" + String(number));
}

void showDHT () {
  // ucg.clearScreen();
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }

  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" %");

  if(oldhum != humidity || oldtemp != temperature) {
    ucg.setColor(bgcol[0], bgcol[1], bgcol[2]); ucg.drawBox(10, 0, 118, 38);
    ucg.setColor(0,255,0);
    ucg.setPrintPos(10,30);
    ucg.print((String)temperature + " *C");
    ucg.setPrintPos(64,30);
    ucg.print((String)humidity + " %");
    oldhum = humidity; oldtemp = temperature;
  }
}
 
void setup() {
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  Serial.begin(115200);
  Serial.println("Start");
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  //ucg.begin(UCG_FONT_MODE_SOLID);
  ucg.clearScreen();
  ucg.setColor(bgcol[0], bgcol[1], bgcol[2]); ucg.drawBox(0, 0, 128, 160);
  ucg.setFont(ucg_font_ncenR10_tr);
  ucg.setColor(255, 255, 255);
  //ucg.setColor(0, 255, 0);
  // ucg.setColor(1, 255, 255,255);
 
  ucg.setPrintPos(10,25); ucg.print("Happy bones!");
 
}
 
void loop() {
  if (millis() > mil + 5000 || millis() < mil){
    mil = millis()/1000*1000;

    showDHT();
    showBones(random(1, 6));
  }
 
}