 #include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);  // CE, CSN pins
const byte address[6] = "00001"; // address for the receiving Arduino Mega board
int buttonPin = 2; // Pin to which push button is connected
int buttonState = 0; // current state of the button
int lastButtonState = 0; // previous state of the button
unsigned long lastDebounceTime = 0; // last time the button was pressed
unsigned long debounceDelay = 50; // debounce delay in milliseconds
bool buttonPressed = false; // flag to indicate if the button was pressed
unsigned long buttonHoldTime = 3000; // time in milliseconds to consider the button as "held"
unsigned long buttonHoldStartTime = 0; // start time of button hold

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  // Read the state of the button
  int reading = digitalRead(buttonPin);

  // Debounce the button
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        buttonPressed = true;
        buttonHoldStartTime = millis(); // record the start time of button hold
      }
    }
  }

  lastButtonState = reading;

  // Send data if the button was pressed for 3 seconds or more
  if (buttonPressed && (millis() - buttonHoldStartTime >= buttonHoldTime)) {
    const char text[] = "Button held for 3 seconds!";
    radio.write(&text, sizeof(text));
    delay(1000); // Wait for 1 second before sending next message
    buttonPressed = false; // reset flag
  }
}
