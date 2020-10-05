#include <TFT.h> 
#include <SPI.h>
#include <Servo.h>

#define ANGLELOW -2.7
#define ANGLEHIGH 0.05
#define CS 10
#define DC 9
#define RST 8

TFT TFTscreen = TFT(CS, DC, RST);
Servo servo;

int lineStartX;
int lineStartY;
int lineEndX;
int lineEndY;

int redRandom;
int greenRandom;
int blueRandom;

int iteration;
int clearIteration;
float lineAngle;
float adder;

int radialPointY = TFTscreen.height() + 17;
int radialPointX = TFTscreen.width() / 2;

void setup() {
  Serial.begin(9600);
  TFTscreen.begin();
  TFTscreen.stroke(50, 255, 140);
  TFTscreen.background(0, 0, 0);

  servo.attach(6);
  servo.write(0);
  
  lineStartX = radialPointX;
  lineStartY = radialPointY;
  lineEndX = radialPointX;
  lineEndY = 0;
  lineAngle = ANGLELOW;
  adder = 0.02;
  iteration = 0;
  clearIteration = 20;
  lineEndX = (lineStartX + radialPointY) * cos(lineAngle);
  lineEndY = (lineStartY + radialPointY) * sin(lineAngle);
  Serial.println(radialPointY);
  Serial.println(TFTscreen.width());
}

void loop() {
  DrawBackground();
  DrawLine();
  TurnMotor();
  CheckDetection();
  
  //Serial.println(lineAngle);
  //Serial.println(adder);
}
void CheckDetection()
{
  if(random(0, 10) == 5)
  {
    TFTscreen.stroke(0, 10, 200);
    TFTscreen.circle(random(lineEndX, radialPointX), random(0, lineStartY), 5);
  }
}

void TurnMotor()
{
  Serial.println(map(lineAngle * -1, ANGLELOW, ANGLEHIGH, 1, 31));
}

void DrawLine()
{
  TFTscreen.stroke(0, 255, 20);
  // Draw a line from circle centers to center top of screen
  TFTscreen.line(lineStartX, lineStartY, lineEndX, lineEndY);
  if(lineAngle < ANGLELOW || lineAngle > ANGLEHIGH) adder = adder * -1;
  lineAngle += adder;
  lineEndX = (lineStartX + radialPointY) * cos(lineAngle);
  lineEndY = (lineStartY + radialPointY) * sin(lineAngle);
  Serial.println(lineAngle);
  if(iteration == clearIteration)
  {
    iteration = 0;
    TFTscreen.background(0, 0, 0);
  }
  iteration++;
}

void DrawBackground()
{
  redRandom = random(0, 255);
  greenRandom = random (0, 255);
  blueRandom = random (0, 255);
  TFTscreen.stroke(redRandom, greenRandom, blueRandom);
  TFTscreen.circle(radialPointX, radialPointY, 40);
  TFTscreen.circle(radialPointX, radialPointY, 90);  
  TFTscreen.circle(radialPointX, radialPointY, 145);
}
