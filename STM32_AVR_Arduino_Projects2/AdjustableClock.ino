//Melda Bolat
//Sude Sarı
//İrem Su Gül
//Ece Alpay
//Hilal Yurtoğlu
//Sarp Arslan


#include <Wire.h>
#include "rgb_lcd.h" // Grove RGB LCD library
#include <avr/interrupt.h>

#define BUTTON_PIN 2
#define LED_HOUR 10          // LED for hour adjustment
#define LED_MINUTE 11    // LED for minute adjustment

rgb_lcd lcd;

// Time variables
volatile int hours = 0;
volatile int minutes = 0;
volatile int seconds = 0;

// Button press variables
volatile bool buttonInterruptFlag = false;
unsigned long firstPressTime = 0;
int pressCount = 0;
const unsigned long doublePressInterval = 2000; // 2 seconds for double press

// Timer1 interrupt to increment the clock every second
ISR(TIMER1_COMPA_vect) {
  seconds++;
  if (seconds == 60) {
    seconds = 0;
    minutes++;
    if (minutes == 60) {
      minutes = 0;
      hours++;
      if (hours == 24) {
        hours = 0;
      }
    }
  }
}

// Button ISR
void buttonISR() {
  buttonInterruptFlag = true;
}

// Timer1 setup for 1-second intervals
void setupTimer1() {
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  // 16 MHz / 256 = 62500 counts per second
  // For 1 second: OCR1A = 62499
  OCR1A = 62499;              
  TCCR1B |= (1 << WGM12);     // CTC mode
  TCCR1B |= (1 << CS12);      // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);    // Enable compare interrupt
  interrupts();
}

int readIntegerFromSerial() {
  while (!Serial.available()) {
    // Wait for user input
  }
  int val = Serial.parseInt();
  // Clear remaining buffer
  while (Serial.available()) {
    Serial.read();
  }
  return val;
}

// Function to set the hour via UART with re-prompting for invalid input
void setHour() {
  bool valid = false;
  int newHour = 0;
  while (!valid) {
    Serial.println("Enter hour (0-23):");
    newHour = readIntegerFromSerial();
    if (newHour >= 0 && newHour < 24) {
      valid = true;
    } else {
      Serial.println("Invalid hour! Please try again.");
    }
  }
  
  hours = newHour;
  Serial.println("Hour updated successfully!");
  digitalWrite(LED_HOUR, HIGH);
  delay(1000);
  digitalWrite(LED_HOUR, LOW);
}

// Function to set the minute via UART with re-prompting for invalid input
void setMinute() {
  bool valid = false;
  int newMinute = 0;
  while (!valid) {
    Serial.println("Enter minute (0-59):");
    newMinute = readIntegerFromSerial();
    if (newMinute >= 0 && newMinute < 60) {
      valid = true;
    } else {
      Serial.println("Invalid minute! Please try again.");
    }
  }

  minutes = newMinute;
  Serial.println("Minute updated successfully!");
  digitalWrite(LED_MINUTE, HIGH);
  delay(1000);
  digitalWrite(LED_MINUTE, LOW);
}

void setup() {
  // LCD initialization
  lcd.begin(16, 2);
  lcd.setRGB(255, 255, 255);
  lcd.print("Adjustable Clock");

  // Pin setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_HOUR, OUTPUT);
  pinMode(LED_MINUTE, OUTPUT);

  digitalWrite(LED_HOUR, LOW);
  digitalWrite(LED_MINUTE, LOW);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);

  // UART initialization
  Serial.begin(9600);

  // Timer1 initialization
  setupTimer1();

  delay(1000);
  lcd.clear();
}

void loop() {
  // Display the time on the LCD in HH:MM:SS format
  lcd.setCursor(0, 0);
  if (hours < 10) lcd.print("0");
  lcd.print(hours);
  lcd.print(":");
  if (minutes < 10) lcd.print("0");
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10) lcd.print("0");
  lcd.print(seconds);

  // Check if the button was pressed
  if (buttonInterruptFlag) {
    buttonInterruptFlag = false;

    if (pressCount == 0) {
      pressCount = 1;
      firstPressTime = millis();
    } else if (pressCount == 1) {
      if (millis() - firstPressTime <= doublePressInterval) {
        // Double press: set minute
        setMinute();
        pressCount = 0;
      } else {
        // Too long since first press, treat this as a new first press
        pressCount = 1;
        firstPressTime = millis();
      }
    }
  }

  // If single press timed out, set hour
  if (pressCount == 1 && (millis() - firstPressTime > doublePressInterval)) {
    // Single press: set hour
    setHour();
    pressCount = 0;
  }

  delay(200); // Small delay
}
