#include <avr/io.h>
#include <util/delay.h>
#include <ctype.h>   // Karakter kontrolü için

// Segment pinlerini yeni tanımlama (PORTD ve PORTB)
#define SEG_A PD5   // A segmenti
#define SEG_B PD7   // B segmenti
#define SEG_C PB4   // C segmenti
#define SEG_D PD3   // D segmenti
#define SEG_E PD2   // E segmenti
#define SEG_F PD4   // F segmenti
#define SEG_G PD6   // G segmenti
#define SEG_DP PB3  // DP segmenti

void setup() {
  // PORTD ve PORTB'deki pinleri çıkış olarak ayarla
  DDRD |= (1 << SEG_A) | (1 << SEG_B) | (1 << SEG_C) | (1 << SEG_D) | (1 << SEG_E) | (1 << SEG_F) | (1 << SEG_G);
  DDRB |= (1 << SEG_C) | (1 << SEG_DP);
  
  // UART başlatma
  UBRR0H = 0;
  UBRR0L = 103;  // 9600 baud için ayar (16 MHz)
  UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Alıcı ve verici aktif
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit veri

  allOnDisplay();  // Başta tüm segmentleri aç
}

// Tüm segmentleri açma (clear yerine tüm segmentler açık kalır)
void allOnDisplay() {
  PORTD &= ~((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_D) | (1 << SEG_E) | (1 << SEG_F) | (1 << SEG_G));
  PORTB &= ~((1 << SEG_C) | (1 << SEG_DP));  // SEG_C ve SEG_DP'yi de ekleyin
}

// Tüm segmentleri kapatma (sadece 'd' komutu ile çağrılacak)
void clearDisplay() {
  PORTD |= (1 << SEG_A) | (1 << SEG_B) | (1 << SEG_D) | (1 << SEG_E) | (1 << SEG_F) | (1 << SEG_G);
  PORTB |= (1 << SEG_C) | (1 << SEG_DP);  // SEG_C ve SEG_DP'yi de ekleyin
}

// Sayıları göstermek için özel fonksiyonlar


void displayZero() {
  clearDisplay();
  PORTD &= ~((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_D) | (1 << SEG_E) | (1 << SEG_F));
  PORTB &= ~(1 << SEG_C);
}

void displayOne() {
  clearDisplay();
  PORTD &= ~(1 << SEG_B);
  PORTB &= ~(1 << SEG_C);
}

void displayTwo() {
  clearDisplay();
  PORTD &= ~((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_D) | (1 << SEG_E));
  PORTD &= ~(1 << SEG_G);
}

void displayThree() {
  clearDisplay();
  PORTD &= ~((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_D));
  PORTD &= ~(1 << SEG_G);
  PORTB &= ~(1 << SEG_C);
}

void displayFour() {
  clearDisplay();
  PORTD &= ~((1 << SEG_B) | (1 << SEG_G) | (1 << SEG_F));
  PORTB &= ~(1 << SEG_C);
}

void displayFive() {
  clearDisplay();
  PORTD &= ~((1 << SEG_A) | (1 << SEG_F) | (1 << SEG_D));
  PORTD &= ~(1 << SEG_G);
  PORTB &= ~(1 << SEG_C);
}

void displaySix() {
  clearDisplay();
  PORTD &= ~((1 << SEG_A) | (1 << SEG_D) | (1 << SEG_E) | (1 << SEG_F));
  PORTD &= ~(1 << SEG_G);
  PORTB &= ~(1 << SEG_C);
}

void displaySeven() {
  clearDisplay();
  PORTD &= ~((1 << SEG_A) | (1 << SEG_B));
  PORTB &= ~(1 << SEG_C);
}

void displayEight() {
  clearDisplay();
  PORTD &= ~((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_D) | (1 << SEG_E) | (1 << SEG_F));
  PORTD &= ~(1 << SEG_G);
  PORTB &= ~(1 << SEG_C);
}

void displayNine() {
  clearDisplay();
  PORTD &= ~((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_D) | (1 << SEG_F));
  PORTD &= ~(1 << SEG_G);
  PORTB &= ~(1 << SEG_C);
}


// (displayZero, displayOne vb. aynı kalacak)

void displayNumber(int num) {
  switch (num) {
    case 0: displayZero(); break;
    case 1: displayOne(); break;
    case 2: displayTwo(); break;
    case 3: displayThree(); break;
    case 4: displayFour();
      _delay_ms(1000);
      displayThree();
      _delay_ms(1000);
      displayTwo();
      _delay_ms(1000);
      displayOne();
      _delay_ms(1000);
      displayZero();
      break;
    case 5: displayTwo(); break;
    case 6: displaySix(); break;
    case 7: displaySeven(); break;
    case 8: displayEight(); break;
    case 9: displayNine(); break;
  }
}

// Seri porttan veri okuma
char readSerial() {
  while (!(UCSR0A & (1 << RXC0))); // Veri gelene kadar bekle
  return UDR0;
}

int serialAvailable() {
  return (UCSR0A & (1 << RXC0));
}

void loop() {
    if (serialAvailable()) {
        char inputBuffer[20];
        int i = 0;

        char input = readSerial();

        if (input == 'd') {
            clearDisplay();
            while (serialAvailable()) {
                readSerial();
            }
            return;
        } else if (isdigit(input)) {
            inputBuffer[i++] = input;

            int dataReceived = 1;
            while (dataReceived && i < sizeof(inputBuffer) - 1) {
                dataReceived = 0;
                for (int j = 0; j < 20; j++) {
                    _delay_ms(10);
                    if (serialAvailable()) {
                        input = readSerial();
                        if (isdigit(input)) {
                            inputBuffer[i++] = input;
                            dataReceived = 1;
                        }
                        j = -1;
                    }
                }
            }
            inputBuffer[i] = '\0';

            if (i == 1) {
                int number = inputBuffer[0] - '0';
                displayNumber(number);
            } else if (i > 1) {
                for (int j = 0; j < i; j++) {
                    int digit = inputBuffer[j] - '0';
                    displayNumber(digit);
                    _delay_ms(1000);
                   // allOnDisplay();  // Tüm segmentleri tekrar aç
                    _delay_ms(200);
                }
            }
        }
    }
}
