#include <ESP32Servo.h>
//#include <analogWrite.h>
//#include <tone.h>
//#include <ESP32Tone.h>
//#include <ESP32PWM.h>

#include <Key.h>
#include <Keypad.h>

#include <Wire.h>

#include <LiquidCrystal.h>


const int servoPin = 13;
Servo steeringServo;
LiquidCrystal lcd(14, 27, 26, 25, 19, 18);

const int ROW_NUM = 4;
const int COLUMN_NUM = 3;

int bufferIndex = 0;
#define MAX_COMMAND_LENGTH 10
char commandBuffer[MAX_COMMAND_LENGTH];

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[ROW_NUM] = {15, 2, 0, 4};      // connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {16, 17, 5}; // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
//LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

int cursorColumn = 0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();//clear content
  delay(50);
  //lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" Set up complete");
  lcd.setCursor(0,1);
  steeringServo.attach(servoPin);
  Wire.begin();//initiallise I2C as the master


}

void loop()
{
  char key = keypad.getKey();
  if(key != NO_KEY){
    if (key == '0'){
      //send commands to slave
      for (int i = 0; i < bufferIndex; i++){
        sendCommand(commandBuffer[i]);
        delay(100);
      }
      bufferIndex = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ready");
      }
      else {
        if (bufferIndex < MAX_COMMAND_LENGTH){
          commandBuffer[bufferIndex++] = key;
          lcd.setCursor(0, 0);
          lcd.print("EEEBOT command: ");
          lcd.print(commandBuffer);
          lcd.setCursor(0,1);// move to the next line
          //switch statement
          switch(key){
            case'*':
                  lcd.print("Turn Right ");
            break;
            case '#':
                  lcd.print("Turn left ");
            break;
            default:
            lcd.print("Move forward ");
            break;
            
          }
        }
      }
  }

delay(100);
}

void sendCommand(char command){
  Wire.beginTransmission(9);
  Wire.write(command);
  Wire.endTransmission();
}
