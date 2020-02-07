#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include <Servo.h> 
#include <ros.h>
#include <sensor_msgs/Joy.h>
#include <ArduinoHardware.h>
#include <geometry_msgs/Twist.h>
#include <Stepper.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

ros::NodeHandle nh;
geometry_msgs::Twist msg;
//ros::NodeHandle_<ArdunioHardware, 2, 2, 150, 150> nh;

//Neopixel
#define NEO_PIN 13

// B1
#define en1 2
#define in1 36
#define in2 37

// B2
#define en2 9
#define in3 41
#define in4 40

// B3
#define en3 8
#define in5 33
#define in6 32

// B4 
#define en4 3
#define in7 35
#define in8 34

// B5
#define en5 5
#define in9 27
#define in10 28

// B6
#define en6 7
#define in11 39
#define in12 29

// B7
#define en7 6
#define in13 24
#define in14 23

// B8
#define en8 4
#define in15 25
#define in16 26

// PWM signal breakouts
#define PWM1 12
#define PWM2 11
#define PWM3 10

// Mosfets
#define M1 46
#define M2 45
#define M3 44
#define M4 43

Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, NEO_PIN, NEO_GRB + NEO_KHZ800);

Servo S1;
Servo S2;
Servo S3;

// Stepper

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper B1B2_Stepper(stepsPerRevolution, in2,in1,in3,in4);            
Stepper B3B4_Stepper(stepsPerRevolution, in6,in5,in7,in8);

void twist(const geometry_msgs::Twist& cmd_vel)
{
  // What do autonomous
}

void joystick(const sensor_msgs::Joy& cmd_msg)
{
  // What do manned
  int svo1 = cmd_msg.axes[0] * 100;

  svo1 = map(svo1, -100, 100, 1000, 2000);

  S1.writeMicroseconds(svo1);


  int m1_dir = cmd_msg.axes[6] + 1;

  switch(m1_dir){
  case 0:
    digitalWrite(in10, HIGH);
    digitalWrite(in9, LOW);
    analogWrite(en5, 250);
    break;

  case 2:
    digitalWrite(in10, LOW);
    digitalWrite(in9, HIGH);
    analogWrite(en5, 250);
    break;

  default:
    digitalWrite(in10, LOW);
    digitalWrite(in9, LOW);
    analogWrite(en5, 0);
    break;

  }
  if(cmd_msg.axes[7] == -1){
    digitalWrite(en1, HIGH);
    digitalWrite(en2, HIGH);
    B1B2_Stepper.step(-stepsPerRevolution);
  }

  if(cmd_msg.axes[7] == 1){
    digitalWrite(en1, HIGH);
    digitalWrite(en2, HIGH);
    B1B2_Stepper.step(stepsPerRevolution);
  }
  
  if(cmd_msg.buttons[0] == 1){
    digitalWrite(en3, HIGH);
    digitalWrite(en4, HIGH);
    B3B4_Stepper.step(-stepsPerRevolution);
  }

  if(cmd_msg.buttons[1] == 1){
    digitalWrite(en3, HIGH);
    digitalWrite(en4, HIGH);
    B3B4_Stepper.step(stepsPerRevolution);
  }


  digitalWrite(en1, LOW);
  digitalWrite(en2, LOW);
  digitalWrite(en3, LOW);
  digitalWrite(en4, LOW);

}

ros::Subscriber<sensor_msgs::Joy> sub("joy", joystick);
ros::Subscriber<geometry_msgs::Twist> subb("/cmd_vel", twist);

void setup(){


  strip.begin();
  strip.setBrightness(20);
  strip.show(); // Initialize all pixels to 'off'
  colorWipe(strip.Color(255, 255, 255), 50); // White
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  colorWipe(strip.Color(255, 255, 255), 50); // White
  colorWipe(strip.Color(0, 0, 0), 50); // White

  Serial.begin(57600);
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.subscribe(sub);
  nh.subscribe(subb);

  while(!nh.connected()) {
    nh.spinOnce();
  }

  rainbowCycle(3);

  // PWM Breakouts
  S1.attach(PWM1); //attach it to pin S1
  S2.attach(PWM2); //attach it to pin S2
  S3.attach(PWM3); //attach it to pin S3


    // Full Bridge assignments
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(en3, OUTPUT);
  pinMode(en4, OUTPUT);
  pinMode(en5, OUTPUT);
  pinMode(en6, OUTPUT);
  pinMode(en7, OUTPUT);
  pinMode(en8, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(in5, OUTPUT);
  pinMode(in6, OUTPUT);
  pinMode(in7, OUTPUT);
  pinMode(in8, OUTPUT);
  pinMode(in9, OUTPUT);
  pinMode(in10, OUTPUT);
  pinMode(in11, OUTPUT);
  pinMode(in12, OUTPUT);
  pinMode(in13, OUTPUT);
  pinMode(in14, OUTPUT);
  pinMode(in15, OUTPUT);
  pinMode(in16, OUTPUT);

  //Mosfets
  pinMode(M1, OUTPUT);  
  pinMode(M2, OUTPUT);  
  pinMode(M3, OUTPUT);  
  pinMode(M4, OUTPUT);

  B1B2_Stepper.setSpeed(50);
  B3B4_Stepper.setSpeed(50);

}

void loop(){
  nh.spinOnce();
  delay(1);
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}








