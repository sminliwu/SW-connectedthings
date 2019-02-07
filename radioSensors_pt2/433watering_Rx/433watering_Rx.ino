/*
  433 MHz RF Module Receiver Demonstration 1
  RF-Rcv-Demo-1.ino
  Demonstrates 433 MHz RF Receiver Module
  Use with Transmitter Demonstration 1
  adapted from
  https://dronebotworkshop.com/433mhz-rf-modules-arduino/
*/

// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

// vars
String str_humidity;
String str_mode;
String str_watering;
String str_out;

bool mode;
bool watering;

int dPin = 2;

void setup()
{
  // Initialize ASK Object
  rf_driver.init();

  // initialize digital pin
  pinMode(dPin, OUTPUT);
  digitalWrite(dPin, LOW);
  
  // Setup Serial Monitor
  Serial.begin(9600);
  Serial.println("waiting for received data of size 10 chars...");
}

void loop()
{
  // Set buffer to size of expected message
  uint8_t buf[10];

  uint8_t buflen = sizeof(buf);

  // Check if there is a received packet and if it is expected size
  if (rf_driver.recv(buf, &buflen))
  {
    Serial.print("len: " );
    Serial.println(buflen);

    // Message received with valid checksum
    Serial.print("Message Received: ");
    Serial.println((char*)buf);

    String str_out = String((char*)buf);
    
    if(str_out.substring(0,4) == "msg"){
      Serial.print("got a sensor msg: ");
      Serial.println(str_out.substring(4, buflen));

      str_humidity = str_out.substring(4, 7); // starting 3 characters in, after the message flag
      str_mode = str_out.substring(8);
      str_watering = str_out.substring(10);
      mode = str_mode.toInt();
      watering = str_watering.toInt();
      }
    }
   // Print values to Serial Monitor
      Serial.print("humidity: ");
      Serial.print(str_humidity.toInt());
      Serial.print("  - mode: ");
      Serial.print(mode);
      Serial.print("  - watering? ");
      Serial.println(watering);

   if (mode && watering) {
      digitalWrite(dPin, HIGH);
      delay(2000); // water for 2 seconds
      digitalWrite(dPin, LOW);
    }
  }
}
