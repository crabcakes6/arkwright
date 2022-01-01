#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <stdio.h>
#include <string.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); 

int xMap, yMap, xValue, yValue, ballX, ballY, bValue;

int cpuY = 0;

const int paddleLength = 18;

int playerScore = 0;
int cpuScore = 0;

int ballDirX = 1;
int ballDirY =  1;

float ballSpd = 1.4;

void gameEnd(int x){

  while (true){
    bValue = digitalRead(2);
    u8g2.clearBuffer();
    
    char a[15] = "Player score: ";
    char b[2];
    itoa(playerScore, b, 10);
    strcat(a,b);
    u8g2.drawStr(2,47,a);
    
    char c[12] = "CPU score: ";
    char d[2];
    itoa(cpuScore, d, 10);
    strcat(c,d);
    u8g2.drawStr(2,61,c);
    
    if (x == 0){
      u8g2.drawStr(2,10,"Player wins!");
      u8g2.drawStr(2,24,"Click to restart.");
    }
    else {
      u8g2.drawStr(2,10,"CPU wins!");
      u8g2.drawStr(2,24,"Click to restart.");
    }
    u8g2.sendBuffer();
    
    if (bValue == 0){
      cpuScore = 0;
      playerScore = 0;
      break;
    }
  }
}

void restart(){
  ballX = 60; 
  ballY = random(5, 60);
  ballSpd = 1.4;
}

void setup() {
  
  u8g2.begin();
  Serial.begin(9600) ;
  pinMode(2,INPUT); 
  digitalWrite(2,HIGH); 
  
  u8g2.setFont(u8g2_font_6x13_mr);

  ballX = random(20,102); 
  ballY = random(3,61);
}

void loop() {

//joystick input to pixel coordinates on screen
  xValue = analogRead(A0);  
  yValue = analogRead(A1);  
  
  xMap = map(xValue, 0, 1023, 0 , 128);
  yMap = map(yValue,0, 1023, 0, 57); 
  if (yMap > 64-paddleLength){
    yMap = 64-paddleLength-2;
  }

//cpu ai
  if (cpuY + paddleLength > ballY){
    cpuY -= 1;
  }
  else if (cpuY + paddleLength < ballY){
    cpuY += 1;
  }
  if (cpuY < 1) {
    cpuY = 1;
  }
  if (cpuY > 64-paddleLength){
    cpuY = 64-paddleLength-2;
  }
  
//bounce off the top & bottom walls
  if (ballY <= 3){ 
    ballDirY = -ballDirY;
    ballY = 4;
  } 
  if (ballY >= 62){
    ballDirY = -ballDirY;
    ballY = 61;
  }
  
//check if hit player paddle
  if (ballX <= 5 && ballY >= yMap && ballY  <= yMap + paddleLength){
    ballDirX = -ballDirX; 
    ballX = 6;
    ballSpd += 0.16;

  }
  else if (ballX <= 5){
    cpuScore += 1;
    restart();
  }

//check if hit cpu paddle
  if (ballX >= 118 && ballY >= cpuY && ballY <= cpuY + paddleLength){
    ballDirX = -ballDirX;
    ballX = 117;
    ballSpd += 0.16;
  }
  else if (ballX >= 122){
    playerScore +=1;
    restart();
  }

//ball movement
  ballX = ballX+(ballDirX*ballSpd);
  ballY = ballY+(ballDirY*ballSpd);

//game end conditions
  if (playerScore == 5){
    gameEnd(0);
  }
  else if (cpuScore == 5){
    gameEnd(1);
  }

  u8g2.clearBuffer();

  u8g2.drawRFrame(1,yMap, 5, paddleLength,1);
  u8g2.drawRFrame(120,cpuY, 5, paddleLength,1);
  u8g2.drawRFrame(ballX,ballY,3,3,1);
  u8g2.drawVLine(60,0,65);
  char b[2];
  itoa(playerScore, b, 10);
  u8g2.drawStr(52,10,b);
  char d[2];
  itoa(cpuScore, d, 10);
  u8g2.drawStr(65,8,d);
  
  
  u8g2.sendBuffer();

}
