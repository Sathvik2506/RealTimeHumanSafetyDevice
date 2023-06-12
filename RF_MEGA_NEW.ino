#include <SPI.h>
#include <SoftwareSerial.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <TinyGPS++.h>
#define SIM800_RX 4
#define SIM800_TX 3

RF24 radio(7, 8);                 // CE, CSN pins
const byte address[6] = "00001";  // address for the receiving Arduino Mega board
SoftwareSerial sim800a(SIM800_RX, SIM800_TX);
TinyGPSPlus gps;


void setup() {
  Serial.begin(9600);
  sim800a.begin(9600);
  Serial3.begin(9600);
  delay(1000);
  sim800a.println("AT+CMGF=1");

  delay(1000);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {

  NRF();
}



void GPS() {
  while (Serial3.available() > 0) {
    if (gps.encode(Serial3.read())) {
      if (gps.location.isValid()) {
        String latitude = String(gps.location.lat(), 6);
        String longitude = String(gps.location.lng(), 6);
        String mapLink = "https://www.google.com/maps/search/?api=1&query=12.879226,Longitude:77.544502";
        String Address = "To Check the Camera Please Click the link 192.168.4.1 ";
        delay(5000);
        Serial.println(mapLink);
        delay(2000);
      }
    }
  }
}

void NRF() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    delay(1000);
    sim800a.println("AT+CMGS=\"+916361032119\"");
    delay(1000);
    sim800a.println("https://www.google.com/maps/search/?api=1&query=12.879485,77.544436");
    Serial.println("MAP LINK SENT");
    delay(4000);
    sim800a.println("To Check the Camera Please Click the link 192.168.4.1\n");
    Serial.println("IP LINK SENT");
    delay(1000);
    sim800a.write(26);
    delay(1000);
    Serial.println("THANK YOU");
    return;
  }
}
