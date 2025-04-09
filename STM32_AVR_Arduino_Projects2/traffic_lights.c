#include <avr/io.h>
#include <util/delay.h>

// Define bit positions for the 7-segment display
#define SEG_A PD5
#define SEG_B PD7
#define SEG_F PD4
#define SEG_G PD6
#define SEG_E PD2
#define SEG_D PD3
#define SEG_C PB4
#define SEG_DP PB1

// Define LED pins
#define RED_LED PB0
#define YELLOW_LED PB2
#define GREEN_LED PB3

void setup() {
    // Set pins on PORTD and PORTB as output for the 7-segment display
    DDRD |= (1 << SEG_A) | (1 << SEG_B) | (1 << SEG_F) | (1 << SEG_G) |
            (1 << SEG_E) | (1 << SEG_D);
    DDRB |= (1 << SEG_C) | (1 << SEG_DP);
    
    // Set LED pins as output
    DDRB |= (1 << RED_LED) | (1 << YELLOW_LED) | (1 << GREEN_LED);
}

void clearDisplay() {
    // Turn off all segments
    PORTD |= (1 << SEG_A) | (1 << SEG_B) | (1 << SEG_F) | (1 << SEG_G) |
             (1 << SEG_E) | (1 << SEG_D);
    PORTB |= (1 << SEG_C) | (1 << SEG_DP);
}

// Functions to display digits 0-9 on the 7-segment display
void display0() {
    clearDisplay();
    PORTD &= ~((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_F) | (1 << SEG_E) | (1 << SEG_D));
    PORTB &= ~(1 << SEG_C);
}

void display1() {
    clearDisplay();
    PORTD &= ~(1 << SEG_B);
    PORTB &= ~(1 << SEG_C);
}

void display2() {
    clearDisplay();
    PORTD &= ~((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_G) | (1 << SEG_E) | (1 << SEG_D));
}

void display3() {
    clearDisplay();
    PORTD &= ~((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_G) | (1 << SEG_D));
    PORTB &= ~(1 << SEG_C);
}

void display4() {
    clearDisplay();
    PORTD &= ~((1 << SEG_B) | (1 << SEG_F) | (1 << SEG_G));
    PORTB &= ~(1 << SEG_C);
}

void display5() {
    clearDisplay();
    PORTD &= ~((1 << SEG_A) | (1 << SEG_F) | (1 << SEG_G) | (1 << SEG_D));
    PORTB &= ~(1 << SEG_C);
}

void display6() {
    clearDisplay();
    PORTD &= ~((1 << SEG_A) | (1 << SEG_F) | (1 << SEG_G) | (1 << SEG_E) | (1 << SEG_D));
    PORTB &= ~(1 << SEG_C);
}

void display7() {
    clearDisplay();
    PORTD &= ~((1 << SEG_A) | (1 << SEG_B));
    PORTB &= ~(1 << SEG_C);
}

void display8() {
    clearDisplay();
    PORTD &= ~((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_F) | (1 << SEG_G) | (1 << SEG_E) | (1 << SEG_D));
    PORTB &= ~(1 << SEG_C);
}

void display9() {
    clearDisplay();
    PORTD &= ~((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_F) | (1 << SEG_G) | (1 << SEG_D));
    PORTB &= ~(1 << SEG_C);
}

// Function to manage the traffic light LEDs based on the counter
void controlLEDs(int counter) {
    if (counter > 5) {
        PORTB |= (1 << RED_LED);      // Red ON
        PORTB &= ~(1 << YELLOW_LED);  // Yellow OFF
        PORTB &= ~(1 << GREEN_LED);   // Green OFF
    } else if (counter == 5) {
        PORTB |= (1 << RED_LED);      // Red ON
        PORTB |= (1 << YELLOW_LED);   // Yellow ON
        PORTB &= ~(1 << GREEN_LED);   // Green OFF
    } else if (counter == 0) {
        PORTB &= ~(1 << RED_LED);     // Red OFF
        PORTB &= ~(1 << YELLOW_LED);  // Yellow OFF
        PORTB |= (1 << GREEN_LED);    // Green ON
        _delay_ms(7000);              // Green light stays ON for 7 seconds
    } else {
        PORTB &= ~(1 << RED_LED);     // Red OFF
        PORTB &= ~(1 << YELLOW_LED);  // Yellow OFF
        PORTB |= (1 << GREEN_LED);    // Green ON
    }
}

// Main loop implementing the countdown and LED control
int main() {
    setup();
    
    while (1) {
        // Countdown from 9 to 0
        for (int i = 9; i >= 0; i--) {
            controlLEDs(i);  // Control LEDs based on the counter
            switch (i) {
                case 9: display9(); break;
                case 8: display8(); break;
                case 7: display7(); break;
                case 6: display6(); break;
                case 5: display5(); break;
                case 4: display4(); break;
                case 3: display3(); break;
                case 2: display2(); break;
                case 1: display1(); break;
                case 0: display0(); break;
            }
            _delay_ms(500);  // Delay for countdown
        }
    }

    return 0;
}
