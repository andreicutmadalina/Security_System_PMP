#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include<SoftwareSerial.h>

#define Password_Length 5 

SoftwareSerial s(2, 3);

char Data[Password_Length]; 
char Master[Password_Length] = "123*"; 
byte data_count = 0, master_count = 0, wrong_count = 0;
bool Pass_is_good;
char customKey;

const byte ROWS = 4;
const byte COLS = 3;

int buttonPin = 6;
int lastPressedTime = 0;
int lastButtonState;
int debounceTime = 20;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {13, 12, 11, 10};
byte colPins[COLS] = {9, 8, 7};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup(){
  Serial.begin(9600);
  s.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  lcd.init(); 
  lcd.backlight();
}

void debounce()
{
  int buttonState = digitalRead(buttonPin);
  if( (millis() - lastPressedTime) > debounceTime )
  {
      lastPressedTime = millis();
      if( buttonState == 0 && lastButtonState == 1) //if the button is now pressed and the last time was released
        {
            s.write(2); 
            lastButtonState = 0;
        }
      
      if(buttonState == 1 && lastButtonState == 0) //if the button is not pressed and the last time was pressed
        {
            lastButtonState = 1;
        }
  }
}

void loop()
{
  debounce();
  enterPassword();
}

void enterPassword()
{
  lcd.setCursor(0,0);
  lcd.print("Enter Password:");

  customKey = customKeypad.getKey();
  if (customKey){
    Data[data_count] = customKey; 
    lcd.setCursor(data_count,1); 
    lcd.print(Data[data_count]); 
    data_count++; 
    }

  if(data_count == Password_Length-1){
    lcd.clear();

    if(!strcmp(Data, Master)){
      lcd.print("Correct");
      delay(5000);
      wrong_count = 0;
      }
    else{
      lcd.print("Incorrect");
      wrong_count++;
      delay(1000);
      }

    if(wrong_count == 4){
      lcd.clear();
      lcd.print("ALARM!");
      s.write(1); 
      delay(3000);
      wrong_count = 0;
    }
    
    lcd.clear();
    clearData();  
  }
}

void clearData(){
  while(data_count !=0){
    Data[data_count--] = 0; 
  }
  return;
}
