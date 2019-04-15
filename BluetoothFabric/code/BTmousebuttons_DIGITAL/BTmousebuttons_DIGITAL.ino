/*
 Based on Bluetooth Serial Passthrough Sketch by Jim Lindblom
 modifications by: Shanel Wu

 The original code converts an RN-42 bluetooth module to
 communicate at 9600 bps (from 115200), and passes any serial
 data between Serial Monitor and bluetooth module. Additional 
 code configures the RN-42 to communicate through the HID 
 profile as a mouse.

 This sketch uses a simple digital input (button worn on finger)
 to provide X-Y input for the cursor position along the vertical
 axis.
 */
#include <SoftwareSerial.h>  

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3
// const int led = 5; // LED connected to PWM pin 5
const int dPin = 7; // one button/analog input connected to pin 5

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
  Serial.begin(9600);  // Begin the serial monitor at 9600bps
  //pinMode(led, OUTPUT);
  pinMode(dPin, INPUT);
  Serial.println("beginning");

  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode, no line break
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600

  // bluetooth.print("$$$"); // re-enter command mode
}

void loop()
{ 
  bool buttonReading = digitalRead(dPin);
  //Serial.println(buttonReading);
  if(bluetooth.available())  // If the bluetooth sent any characters
  {
    // Send any characters the bluetooth prints to the serial monitor
    char input = bluetooth.read();
    Serial.print(input);
  }
  //if(Serial.available())  // If stuff was typed in the serial monitor
  //{
    // Send HID reports
    // char input = Serial.read();
    // send mouse down
    if (buttonReading) {
      bluetooth.write(0xFD); // start byte
      bluetooth.write(0x5);
      bluetooth.write(0x2);
      bluetooth.write((byte)0);
      bluetooth.write((byte)0); // X movement
      bluetooth.write(5);  // Y movement
      bluetooth.write((byte)0);
    } else if (!buttonReading){ // send mouse up
      bluetooth.write(0xFD); // start byte
      bluetooth.write(0x5);
      bluetooth.write(0x2);
      bluetooth.write((byte)0);
      bluetooth.write((byte)0); // X movement
      bluetooth.write(-5);  // Y movement
      bluetooth.write((byte)0);      
    } 
    /* else if (input == 'd'){ // send mouse right
      bluetooth.write(0xFD); // start byte
      bluetooth.write(0x5);
      bluetooth.write(0x2);
      bluetooth.write((byte)0);
      bluetooth.write(10); // X movement
      bluetooth.write((byte)0);  // Y movement
      bluetooth.write((byte)0);      
    } else if (input == 'a'){ // send mouse left
      bluetooth.write(0xFD); // start byte
      bluetooth.write(0x5);
      bluetooth.write(0x2);
      bluetooth.write((byte)0);
      bluetooth.write(-10); // X movement
      bluetooth.write((byte)0);  // Y movement
      bluetooth.write((byte)0);      
    } else {
      Serial.println("invalid mouse direction");
    }*/
  //}
  // and loop forever and ever!
  delay(100);
}
