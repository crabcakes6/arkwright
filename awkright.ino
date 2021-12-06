#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const int buttonPin = 4;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
int buttonState = 0; 

void setup() {
  u8g2.begin(); 
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);    
}

void loop() {
  buttonState = digitalRead(buttonPin);
  u8g2.setFont(u8g2_font_ncenB14_tr);
  if (buttonState == HIGH) {
    u8g2.clearBuffer();
    u8g2.drawStr(0,20,"On");
    u8g2.sendBuffer();
  } else {
    u8g2.clearBuffer();
    u8g2.drawStr(0,20,"Off");
    u8g2.sendBuffer();
  }
}
