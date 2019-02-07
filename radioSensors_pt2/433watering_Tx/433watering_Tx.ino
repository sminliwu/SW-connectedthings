/*
  433 MHz RF Module Transmitter Demonstration 2
  RF-Xmit-Demo-2.ino
  Demonstrates 433 MHz RF Transmitter Module with DHT-22 Sensor
  Use with Receiver Demonstration 2
   adapted from
  https://dronebotworkshop.com/433mhz-rf-modules-arduino/
*/

// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>

// Define Constants
const int THRESHOLD = 30; // set humidity threshold

// Define Variables

int humidity;    // stores input values
int mode;
int watering;

int aPin = A0;
int dPin = 2;

// Define output strings

String str_humidity = "000"; // humidity range 0-100%
String str_mode = "0";
String str_watering = "0";
String str_out;

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

void setup() {

  // Initialize ASK Object
  rf_driver.init();

  // initialize digital pin to use an internal pullup
  pinMode(dPin, INPUT_PULLUP);
  
  Serial.begin(9600);
  Serial.print("started serial at 9600...");
}

void loop()
{
  humidity = analogRead(aPin);  // Get analog value
  mode = digitalRead(dPin);  // Get digital value

  // Convert analog read integer to string
  if (humidity < 100 && humidity > 9) {
    str_humidity = "0" + String(humidity);
  }
  else if (humidity < 10) {
    str_humidity = "00" + String(humidity);
  }
  else { //uses 3 digits
    str_humidity = String(humidity);
  }
  if (humidity < THRESHOLD) {
    str_watering = "1";
  }
  str_mode = String(mode);
  
  // Combine analog and digital sensors into one string (payload)
  str_out = "msg" + str_humidity + "," + str_mode + "," + str_watering;

  // Compose output character
  static char *msg = str_out.c_str();
  Serial.print("sending message: ");
  Serial.println(msg);
  Serial.print("message length: ");
  Serial.println(strlen(msg));

  rf_driver.send((uint8_t *)msg, strlen(msg));
  rf_driver.waitPacketSent();
  
  delay(43200000); // wait 12 hours
}
