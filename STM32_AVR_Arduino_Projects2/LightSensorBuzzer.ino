#include <Wire.h>
#include "rgb_lcd.h" // Include library for the Grove RGB LCD

#define BUTTON_PIN 2           
#define BUZZER_PIN 4           // Digital pin connected to the buzzer
#define LIGHT_SENSOR_PIN A3    // Analog pin connected to the light sensor

rgb_lcd lcd;                   // Create an LCD object
volatile bool buttonPressed = false; // Interrupt flag for button press

// Interrupt Service Routine (ISR) for button press
void buttonISR() {
  buttonPressed = true;        // Set the flag when the button is pressed
}

void setup() {
  // Initialize the LCD
  lcd.begin(16, 2);            // Initialize the LCD (16 columns x 2 rows)
  lcd.setRGB(255, 255, 255);   // Set LCD backlight to white
  lcd.print("Initializing..."); // Display initializing message

  // Configure pins
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configure button as input with pull-up resistor
  pinMode(BUZZER_PIN, OUTPUT);       // Configure buzzer as output
  pinMode(LIGHT_SENSOR_PIN, INPUT);  // Configure light sensor as input

  // Attach interrupt to the button
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);

  // Small delay for initialization
  delay(1000);
  lcd.clear();                // Clear LCD after initialization
}

void loop() {
  // Read the light sensor value
  int lightValue = analogRead(LIGHT_SENSOR_PIN);

  // Display the light sensor value on the LCD
  lcd.setCursor(0, 0);         // Set cursor to the first row
  lcd.print("Light Value: ");  // Display label
  lcd.setCursor(0, 1);         // Set cursor to the second row
  lcd.print(lightValue);       // Display the light sensor value
  lcd.print("    ");           // Clear extra characters from the previous value

  // Check if the button was pressed
  if (buttonPressed) {
    buttonPressed = false;      // Reset the flag

    // Ring the buzzer for 2 seconds
    digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer
    delay(2000);                   // Wait for 2 seconds
    digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
  }

  delay(500); // Update light value every 500ms
}
