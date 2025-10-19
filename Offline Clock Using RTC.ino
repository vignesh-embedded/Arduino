#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;

#define BUZZER_PIN 8

int lastHour = -1;

void setup() {
  lcd.init();
  lcd.backlight();
  Wire.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  lcd.setCursor(0, 0);
  lcd.print("Smart Clock");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1500);
  lcd.clear();

  if (!rtc.begin()) {
    lcd.print("RTC not found!");
    while (1);
  }

  rtc.adjust(DateTime(2025, 10, 19, 20, 59, 55)); 

  if (!rtc.isrunning()) {
    lcd.print("RTC not running!");
    delay(1000);
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
  }
}

void loop() {
  DateTime now = rtc.now();

  // ---- Display Time ----
  lcd.setCursor(0, 0);
  if (now.hour() < 10) lcd.print("0");
  lcd.print(now.hour());
  lcd.print(":");
  if (now.minute() < 10) lcd.print("0");
  lcd.print(now.minute());
  lcd.print(":");
  if (now.second() < 10) lcd.print("0");
  lcd.print(now.second());

  // ---- Display Date ----
  lcd.setCursor(0, 1);
  if (now.day() < 10) lcd.print("0");
  lcd.print(now.day());
  lcd.print("/");
  if (now.month() < 10) lcd.print("0");
  lcd.print(now.month());
  lcd.print("/");
  lcd.print(now.year());

  // ---- Hourly Buzzer ----
  if (now.minute() == 0 && now.second() == 0 && now.hour() != lastHour) {
    // Beep 3 times
    for (int i = 0; i < 7; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(200);
      digitalWrite(BUZZER_PIN, LOW);
      delay(200);
    }
    lastHour = now.hour();
  }

  delay(500);
}
