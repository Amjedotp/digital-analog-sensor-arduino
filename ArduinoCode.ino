#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x20,16,2);


const int pirPin = 2;
const int ledPin = 8;
const int analogPin = A1;
const int buttonPin = 7;


int pirValue = 0;
int analogValue = 0;
bool systemOn = true; 
bool lastButtonState = LOW;
bool currentButtonState;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(analogPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
}

void loop() {
 
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != currentButtonState) {
      currentButtonState = reading;
      if (currentButtonState == HIGH) {
        systemOn = !systemOn; 
      }
    }
  }

  lastButtonState = reading;

  pirValue = digitalRead(pirPin);
  analogValue = analogRead(analogPin);

  Serial.print("PIR: ");
  Serial.print(pirValue);
  Serial.print(" | Analog: ");
  Serial.print(analogValue);
  Serial.print(" | System: ");
  Serial.println(systemOn ? "ON" : "OFF");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PIR: ");
  lcd.print(pirValue);
  lcd.print(" A:");
  lcd.print(analogValue);

  lcd.setCursor(0, 1);
  lcd.print("System: ");
  lcd.print(systemOn ? "ON" : "OFF");

  if (systemOn && (pirValue == HIGH || analogValue > 500)) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(300);
}