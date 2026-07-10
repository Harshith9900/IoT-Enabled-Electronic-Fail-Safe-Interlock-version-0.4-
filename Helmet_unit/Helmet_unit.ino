#include <RH_ASK.h>
#include <SPI.h> 

int irHelmetPin = 3;
int buzzerPin = 12;
int irHelmetValue = 0;

RH_ASK driver(2000, 11, 0, 2);

void setup() {
  pinMode(irHelmetPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  
  if (!driver.init()) {
    Serial.println("RF init failed");
  }
}

void loop() {
  // Read the IR sensor (LOW usually means object detected/helmet on)
  irHelmetValue = digitalRead(irHelmetPin);
  
  Serial.print("Helmet IR: "); 
  Serial.println(irHelmetValue);
  
  bool unsafeCondition = false;
  
  // If no reflection is detected, it's unsafe
  if (irHelmetValue == HIGH) unsafeCondition = true; 
  
  // Sound the alarm in the helmet if it's off the head
  digitalWrite(buzzerPin, unsafeCondition ? HIGH : LOW);
  
  // Create a tiny 2-byte payload: [Status, Null Terminator]
  char msg[2];
  msg[0] = unsafeCondition ? '1' : '0'; 
  msg[1] = '\0';
  
  // Transmit the status to the bike
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  
  delay(500); // Check twice a second
}