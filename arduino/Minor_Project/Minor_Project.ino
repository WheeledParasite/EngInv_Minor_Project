#include <LCD_I2C.h>
#include <ESP8266WiFi.h>

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
  
  WiFi.begin("commodore", "TeddybearPW707");

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
    }
    lastValue = sensorValue; // sets lastValue to current Sensor 
  }
}
