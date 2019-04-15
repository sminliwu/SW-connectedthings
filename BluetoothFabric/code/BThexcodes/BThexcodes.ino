/*
 Example Bluetooth Serial Passthrough Sketch
 by: Jim Lindblom
 SparkFun Electronics
 date: February 26, 2013
 license: Public domain

 This example sketch converts an RN-42 bluetooth module to
 communicate at 9600 bps (from 115200), and passes any serial
 data between Serial Monitor and bluetooth module.
 */
#include <SoftwareSerial.h>  

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3
const int led = 5; // LED connected to PWM pin 5
const int button = 7; // button will be connected to pin 7

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
  Serial.begin(9600);  // Begin the serial monitor at 9600bps
  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
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
  if(bluetooth.available())  // If the bluetooth sent any characters
  {
    // Send any characters the bluetooth prints to the serial monitor
    char input = bluetooth.read();
    Serial.print(input);
  }
  if(Serial.available())  // If stuff was typed in the serial monitor
  {
    // Send any characters the Serial monitor prints to the bluetooth
    //bluetooth.print((char)Serial.read());
    char input = Serial.read();
    //Serial.println(input, HEX);
    // send capital A
    bluetooth.write(0xFD); // start byte
    bluetooth.write(0x9);
    bluetooth.write(0x1);
    bluetooth.write(0x2);
    bluetooth.write((byte)0);
    bluetooth.write(0x04);
    bluetooth.write((byte)0);
    bluetooth.write((byte)0);
    bluetooth.write((byte)0);
    bluetooth.write((byte)0);
    bluetooth.write((byte)0);

    // send button release
    bluetooth.write(0xFD); // start byte
    bluetooth.write(0x9);
    bluetooth.write(0x1);
    bluetooth.write((byte)0);
    bluetooth.write((byte)0);
    bluetooth.write((byte)0);
    bluetooth.write((byte)0);
    bluetooth.write((byte)0);
    bluetooth.write((byte)0);
    bluetooth.write((byte)0);
    bluetooth.write((byte)0);
    //bluetooth.write((byte)0);
    
    //bluetooth.write(0xA); // 0xA = new line
    //bluetooth.write(0xF0); 
    //bluetooth.write(0x9F); 
    //bluetooth.write(0xA4); 
    //bluetooth.write(0xAB);
  }
  // and loop forever and ever!
}
