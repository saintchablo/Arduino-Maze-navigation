#include <PID_v1.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <ESP32Encoder.h>





#define MAX_COMMAND_LENGTH 10
char commandBuffer[MAX_COMMAND_LENGTH];
int bufferIndex = 0;

const int motor_power = 190;
const int wheel_d = 60;
const float wheel c = PI*wheel_d;
const int counts_per_rev = 384;


const int enc_l_pin = 36;
const int enc_r_pin = 39;
const int pwma_pin = 33;
const int ain1_pin = 26;
const int ain2_pin = 27;
const int pwmb_pin = 25;
const int bin1_pin = 14;
const int bin2_pin = 12;

//PWM Properties for ESP32
const int freq = 2000;
const int ledChannelA = 0;
const int ledChannelB = 1;
const int resolution = 8;

//PID variables
float kp = 0.001;
float ki = 0.1;
float kd = 1;


const int iteration_delay = 10;
//Define PID variables

float lastError = 0;
float integral = 0;

const int forwardSpeed = 200;
int servopin = 13;
Servo servo;


//PWM properties

int dutyCycl = 5;
const int servoChannel = 2;
int steeringAnglePWM = 0;

int steeringAngle = 78;
int rightAngle = 130;
int leftAngle = 48;

const int servoFrequency = 50;
float factorA = 2.5;

//global variables for encoder counts
volatile unsigned long enc_1 = 0;
volatile unsigned long enc_r = 0;

//function protoyping 
void driveStraight(float dist, int power);
void enableMotors(boolean en);
void drive(int power_a, int power_b);
void brake();
void countLeft();
void countRight();



void setup() {
  Serial.begin(9600);

  pinMode(enc_l_pin, INPUT_PULLUP);
  pinMode(enc_r_pin, INPUT_PULLUP);
  pinMode(ain1_pin, OUTPUT);
  pinMode(ain2_pin, OUTPUT);
  pinMode(ain1_pin, OUTPUT);
  pinMode(pwma_pin, OUTPUT);
  pinMode(pwmb_pin, OUTPUT);
  pinMode(bin1_pin, OUTPUT);
  pinMode(bin2_pin, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(enc_l_pin), countLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_r_pin), countRight, CHANGE);

  ledcSetup(ledChannelA, freq, resolution);
  ledcSetup(ledChannelB, freq, resolution);
  ledcAttachPin(pwma_pin, ledChannelA);
  ledcAttachPin(pwmb_pin, ledChannelB);
  servo.attach(servopin);
  servo.write(steeringAngle);

  Wire.begin(9);//i2c communication as a slave with address 9;
  Wire.onRecieve(receiveEvent);//register i2c receive event
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (bufferIndex > 0){
    executeCommands();
    }
    delay(iteration_delay);


}
void receiveEvent(int howMany){
  while (wire.avalible() > 0){
    char c = wire.read(); //read the received character
    if (bufferIndex < MAX_COMMAND_LENGTH){
      commandBuffer[bufferIndex++] = c;
    }
  }
}

void executeCommands(){
  for (int i = 0; i < bufferIndex; i++){
    switch (commandBuffer[i]){
      case '0':
      {
        //sending commands, no action needeed
        break;
      }

      case '#': //moving left
      {
        servo.write(leftAngle);
        delay(100);
        moveForward(450);
        delay(100);
        returnServo();
        break;
      }
    case '*': //moving right
    {
      servo.write(rightAngle);
      delay(100);
      moveForward(450); //
      delay(100);
      returnServo();
      break;
    }
    case '1':returnServo();
    }

  }
}