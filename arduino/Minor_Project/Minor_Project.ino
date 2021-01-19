
/*  
 *   Hunter Ruebsamen
 *   Engineering Innovations - HBHS
 *   Teacher: Mr. Crossett
 *   2020-2021
 *   
 *   License: GPL v3
 */

// I2C Pins on NODE MCU
// D4 - SDA
// D5 - SCL

/* PARTS */
// NODE MCU V3 LOLIN
// RCWL-0516 - Doppler Radar Sensor
// 1602 LCD i2c interface
// 16x2 LCD Screen

#include <LCD_I2C.h>
#include <ESP8266WiFi.h>

// The output from the RCWL-0516 will feed into D3
#define SENSORPIN D3

//Variable instantiation
LCD_I2C lcd (0x27);
int sensorValue = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode (SENSORPIN, INPUT);   
  lcd.begin();
  lcd.backlight();
  
  WiFi.begin("<your ssid>", "<password>");

  Serial.print("Connecting");
  lcd.print("Connecting");

  int time = 0;
  while(WiFi.status() != WL_CONNECTED) { //Gives Microcontroller 5 seconds to find a connection
    delay(500);
    Serial.print(".");
    time += 500;
    if(time >= 5000) { //If connections is not found within this time, print "Connection not Found"
      lcd.clear();
      lcd.print("Connection not Found");
      break;
    }
  }
  if(WiFi.status() == WL_CONNECTED) { // Outputs the IP Address if a connection is found
    Serial.println();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Display IP Address we connected to
    lcd.clear();
    lcd.print("IP Address: ");
    lcd.setCursor(0,1);
    lcd.print(WiFi.localIP());
    delay(5000);
  } 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Scanning");
  lcd.setCursor(0,1);
  lcd.print("for motion...");
  delay(1500);
  lcd.noBacklight();
}

int lastValue = LOW;
void loop() {
  sensorValue = digitalRead (SENSORPIN);
  if(sensorValue != lastValue) { //Sensor value has changed
    lcd.clear();
    lcd.setCursor(0,0);
    
    if(sensorValue == LOW) { //turns off lcd backlight when no motion is detected
      lcd.noBacklight();
      lcd.print("Scanning");
      lcd.setCursor(0,1);
      lcd.print("for motion...");
      Serial.println("No Movement");
    } else { //turns on lcd backlight when motion is detected
      lcd.backlight();
      lcd.print("Motion");
      lcd.setCursor(0,1);
      lcd.print("detected");
      Serial.println("Movement detected");
      // Could send an email or text message here, and sound an alarm
    }
    lastValue = sensorValue; // sets lastValue to current Sensor 
  }
}
