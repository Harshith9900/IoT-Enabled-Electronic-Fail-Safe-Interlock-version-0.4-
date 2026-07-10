#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver(2000, 0, 11, 2);

int relayPin = 12;

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Default to engine off
  Serial.begin(9600);
  
  if (!driver.init()) {
    Serial.println("RF init failed");
  }
  Serial.println("Receiver ready...");
}

void loop() {
  uint8_t buf[2];
  uint8_t buflen = sizeof(buf);
  
  // If a verified packet is received from the helmet
  if (driver.recv(buf, &buflen)) {
    Serial.print("Message Received: ");
    Serial.println((char*)buf);
    
    // Check the single safety flag
    if (buf[0] == '1') {
      digitalWrite(relayPin, LOW); 
      Serial.println("Helmet NOT worn -> Motor OFF");
    } else {
      digitalWrite(relayPin, HIGH); 
      Serial.println("Helmet Worn -> Motor ON");
    }
  }
}