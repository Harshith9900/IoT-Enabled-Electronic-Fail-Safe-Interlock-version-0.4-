// moved the buzzer to receiver due to lack of ground in helmet arduino . It will buzz if 

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initializing the LCD with standard I2C address 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int rfReceivePin = 11;
const int buzzerPin = 4;

void setup() {
  lcd.init();
  lcd.backlight();
  
  pinMode(rfReceivePin, INPUT);
  pinMode(buzzerPin, OUTPUT); 
  
  // Quick booting up the screen to verify the LCD is getting power and data
  lcd.setCursor(0, 0);
  lcd.print("System Booting..");
  delay(2000);
  lcd.clear();
}

void loop() {
  int signal = digitalRead(rfReceivePin);

  // If the pin drops to LOW, the transmitter is sending the alarm state
  if (signal == LOW) { 
     // UNSAFE STATE (Helmet is off)
     lcd.setCursor(0, 0);
     lcd.print("Engine: STOPPED ");
     lcd.setCursor(0, 1);
     lcd.print("Helmet: UNSAFE  ");
     
     digitalWrite(buzzerPin, HIGH); // Turns the buzzer ON
  } else {
     // SAFE STATE (Helmet is on)
     lcd.setCursor(0, 0);
     lcd.print("Engine: RUNNING ");
     lcd.setCursor(0, 1);
     lcd.print("Helmet: SAFE    ");
     
     digitalWrite(buzzerPin, LOW);  // Turns the buzzer OFF
  }
  
  delay(200); // Small refresh delay
}
