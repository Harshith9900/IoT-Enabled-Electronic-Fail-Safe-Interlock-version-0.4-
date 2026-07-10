#include <RH_ASK.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// updating the code due to hardware wiring

// --- BIKE UNIT (RECEIVER) PINS ---
const int rfRxPin = 11;       // RF Receiver Data Pin
const int relayPin = 8;       // Relay Module IN Pin

// Initialize Radio (Speed: 2000, RX: 11, TX: 12 (unused), PTT: 0)
RH_ASK driver(2000, rfRxPin, 12, 0);

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  
  // Set up the Relay switch (Default to cutting the engine for safety)
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); 
  
  // Boot up the LCD Screen
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Booting..");

  // Boot up the Radio
  if (!driver.init()) {
    Serial.println("RF init failed!");
    lcd.setCursor(0, 1);
    lcd.print("Radio Error!");
  } else {
    Serial.println("Bike Receiver Online.");
    lcd.setCursor(0, 1);
    lcd.print("Radio Active");
    delay(2000);
    lcd.clear();
  }
}

void loop() {
  uint8_t buf[2];
  uint8_t buflen = sizeof(buf);

  // Check if a radio packet arrived
  if (driver.recv(buf, &buflen)) {
    char status = (char)buf[0];
    
    Serial.print("Received Status: ");
    Serial.println(status);
    
    // Update the LCD and switch the relay
    lcd.setCursor(0, 0);
    if (status == '1') {
      // UNSAFE: Helmet is off
      lcd.print("Status: UNSAFE  ");
      digitalWrite(relayPin, HIGH); // Cut engine
      
      lcd.setCursor(0, 1);
      lcd.print("Engine: STOPPED ");
    } else if (status == '0') {
      // SAFE: Helmet is on
      lcd.print("Status: SAFE    ");
      digitalWrite(relayPin, LOW);  // Start engine
      
      lcd.setCursor(0, 1);
      lcd.print("Engine: RUNNING ");
    }
  }
}
