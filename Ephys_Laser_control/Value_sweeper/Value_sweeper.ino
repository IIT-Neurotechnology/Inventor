#include <SPI.h>
const uint8_t CSpin = 10;

void setup() {
    setupSPI();
    setupPWM();
    Serial.begin(115200);
}

void loop() {
    for(uint16_t i = 0; i < 1024; i++) { // Loop over the full range of the digipot
        update_digipot(0x01, i); // Update wiper position
        delay(100); // Short delay between updates for visibility
        Serial.println(i);
    }
    delay(1000); // Wait for 1 second after completing a full range update
}



void update_digipot(uint8_t reg, uint16_t value) {
  union {
    uint16_t val;
    uint8_t bytes[2];
  } in;

  in.val = value << 6; //as per datasheet

  digitalWrite(CSpin, LOW);

  //Write Wiper Register.  0x01: wiper1, 0x02: wiper2
  SPI.transfer(reg);

  //send upper value byte
  SPI.transfer(in.bytes[1]);

  //send lower value byte
  SPI.transfer(in.bytes[0]);
  digitalWrite(CSpin, HIGH);
}



void setupPWM() {
    analogWrite(3, 50); // Set PWM to 50% duty cycle on pin D3
}

void setupSPI() {
    pinMode(CSpin, OUTPUT);
    pinMode(3, OUTPUT);
    SPI.begin();
} 