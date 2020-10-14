#include <TFT.h> 
#include <SPI.h>
#include <Servo.h>

#define ANGLELOW -2.7
#define ANGLEHIGH 0.05
#define CS 10
#define DC 9
#define RST 8

#define TRIGGER 7
#define ECHO 4

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

int lineR = 0;
int lineG = 255;
int lineB = 20;

int radialPointY = TFTscreen.height() + 17;
int radialPointX = TFTscreen.width() / 2;
int motorPosition = 0;

int distance;
long duration;

void setup() {
  Serial.begin(9600);
  TFTscreen.begin();
  TFTscreen.stroke(50, 255, 140);
  TFTscreen.background(0, 0, 0);

  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);

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
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  if(distance < 100)
  {
    lineR = 0;
    lineG = 10;
    lineB = 200;
  }
  else
  {
    lineR = 0;
    lineG = 255;
    lineB = 20;
  }
  if(random(0, 10) == 5)
  {
    TFTscreen.stroke(0, 10, 200);
    TFTscreen.circle(random(lineEndX, radialPointX), random(0, lineStartY), 5);
  }
}

void TurnMotor()
{
  //Serial.println(map(lineAngle < 0 ? lineAngle * -1 : lineAngle, ANGLELOW, ANGLEHIGH, 1, 31));
  Serial.print("Angle: ");
  motorPosition = lineAngle * (180 / 3.14);
  if(motorPosition < 0) motorPosition *= -1; 
  Serial.println(motorPosition);
  servo.write(motorPosition);
}

void DrawLine()
{
  TFTscreen.stroke(lineR, lineG, lineB);
  // Draw a line from circle centers to center top of screen
  TFTscreen.line(lineStartX, lineStartY, lineEndX, lineEndY);
  if(lineAngle < ANGLELOW || lineAngle > ANGLEHIGH) adder = adder * -1;
  lineAngle += adder;
  lineEndX = (lineStartX + radialPointY) * cos(lineAngle);
  lineEndY = (lineStartY + radialPointY) * sin(lineAngle);
  //Serial.println(lineAngle);
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
