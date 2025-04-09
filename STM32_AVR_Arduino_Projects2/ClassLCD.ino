//Melda Bolat
//Sude Sarı
//İrem Su Gül
//Ece Alpay
//Hilal Yurtoğlu
//Sarp Arslan


#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int buttonPin = 2;

void setup() {
  lcd.begin(16, 2);  
  
  lcd.setRGB(0, 255, 0);
  
  lcd.print("CMPE453-2024F");
  delay(3000);
  
  lcd.clear();

  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {
    lcd.clear();
    lcd.setRGB(128, 0, 128); 
    lcd.setCursor(0, 0); 
    lcd.print("Welcome to Class");
    lcd.setCursor(0, 1); 
    lcd.print("Button is ON");
  } else {
    lcd.clear();
    lcd.setRGB(0, 255, 0); 
    lcd.setCursor(0, 0); 
    lcd.print("GoodBye to Class");
  }
  
  delay(100);
}
