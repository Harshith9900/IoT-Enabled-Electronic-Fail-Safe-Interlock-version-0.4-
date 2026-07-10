#include <RH_ASK.h>
#include <SPI.h> 

// --- UPDATED HARDWARE PINS ---
const int leftIRPin = 2;    // Left sensor
const int rightIRPin = 3;   // Right sensor
const int buzzerPin = 4;    // Active buzzer
const int rfTxPin = 12;     // RF Transmitter Data Pin

// Initialize Radio (Speed: 2000, RX: 11 (Unused), TX: 12, PTT: 0)
RH_ASK driver(2000, 11, rfTxPin, 0);

void setup() {
  pinMode(leftIRPin, INPUT);
  pinMode(rightIRPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  Serial.begin(9600);
  
  if (!driver.init()) {
    Serial.println("RF init failed. Check wiring!");
  } else {
    Serial.println("Helmet Brain Online. Radio transmitting.");
  }
}

void loop() {
  // Read both IR sensors (LOW means it detects your head)
  int leftValue = digitalRead(leftIRPin);
  int rightValue = digitalRead(rightIRPin);
  
  Serial.print("Left IR: "); 
  Serial.print(leftValue);
  Serial.print(" | Right IR: "); 
  Serial.println(rightValue);
  
  bool unsafeCondition = false;
  
  // FAILSAFE: Both sensors must lose contact to trigger the alarm
  if (leftValue == HIGH && rightValue == HIGH) {
    unsafeCondition = true; 
  }
  
  // Sound the alarm inside the helmet if it's off
  if (unsafeCondition) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }
  
  // Create a tiny 2-byte payload: [Status, Null Terminator]
  char msg[2];
  msg[0] = unsafeCondition ? '1' : '0'; 
  msg[1] = '\0';
  
  // Beam the status to the bike
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  
  delay(500); // Check twice a second to save power
}
