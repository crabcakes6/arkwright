#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); 

int xMap, yMap, xValue, yValue;
int bValue = 0 ;

//'mouse'

#define image_width  7
#define image_height 7
static unsigned char image_bits[] = {
 0xfe,0xfc,0xf8,0xfc,0xee,0xc7,0x82};

#define clicked_width  9
#define clicked_height 9
static unsigned char clicked_bits[] = {
0xfe,0xff,0x04,0xff,0x08,0xff,0x18,0xff,0x2c,0xff,0x76,0xff,
0x9b,0xff,0x0d,0xff,0x06,0xfe};

void setup() {
  
  u8g2.begin();
  Serial.begin(9600) ;
  pinMode(2,INPUT); 
  digitalWrite(2,HIGH); 
  
  u8g2.setFont(u8g2_font_6x13_mr);
 
}

void loop() {
  
  xValue = analogRead(A0);  
  yValue = analogRead(A1);  
  bValue = digitalRead(2); //default is 1
  
  xMap = map(xValue, 0, 1023, 0 , 120); //joystick input to pixel coordinates on screen
  yMap = map(yValue,0, 1023, 0, 57);

  u8g2.clearBuffer();

  if (bValue == 0) { //if joystick clicked - draw clicked mouse
    u8g2.drawXBM(xMap, yMap, clicked_width, clicked_height, clicked_bits);
   }
   else {
    u8g2.drawXBM(xMap, yMap, image_width, image_height, image_bits);
   }

  
  u8g2.sendBuffer();

}
