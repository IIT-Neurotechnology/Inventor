#include <SPI.h>

//Valori random
// 482=4.8V
// 994= 1.190V
// 831 = 1.150V
// 863= 980mV
// 1023 = 110mV
// 959= 460mV

const uint8_t CSpin = 10;
const uint8_t PWM_PIN = 3; // Pin to generate PWM signal
const unsigned long period = 33.3; // PWM period in milliseconds

// Function declarations
void setupSPI();
void update_digipot(uint8_t reg, uint16_t value);

void setup() {
    pinMode(PWM_PIN, OUTPUT);
    setupSPI();
    Serial.begin(115200);
    while (!Serial); // Wait for the serial port to connect. Needed for native USB
    Serial.println("Enter a number between 1 and 1024 to update the digipot.");
}

void loop() {
    manualPWM(10); // Example: Set PWM to 20% duty cycle
    if (Serial.available() > 0) {
        String inputString = Serial.readStringUntil('\n');
        inputString.trim(); // Trim any whitespace
        int inputNumber = inputString.toInt();

        if (inputNumber >= 1 && inputNumber <= 1024) {
            update_digipot(0x01, (uint16_t)inputNumber);
            Serial.print("Digipot updated to: ");
            Serial.println(inputNumber);
        } else {
            Serial.println("Number out of range. Please enter a number between 1 and 1024.");
        }
    }
}

void manualPWM(byte dutyCycle) {
    unsigned long highTime = period * dutyCycle / 100; // Calculate the high time
    unsigned long lowTime = period - highTime; // Calculate the low time

    digitalWrite(PWM_PIN, HIGH); // Set the pin HIGH
    delay(highTime); // Wait for the high time

    digitalWrite(PWM_PIN, LOW); // Set the pin LOW
    delay(lowTime); // Wait for the low time
}

void update_digipot(uint8_t reg, uint16_t value) {
    union {
        uint16_t val;
        uint8_t bytes[2];
    } in;

    in.val = value << 6; // As per datasheet

    digitalWrite(CSpin, LOW);
    SPI.transfer(reg);
    SPI.transfer(in.bytes[1]);
    SPI.transfer(in.bytes[0]);
    digitalWrite(CSpin, HIGH);
}

void setupSPI() {
    pinMode(CSpin, OUTPUT);
    SPI.begin();
}
