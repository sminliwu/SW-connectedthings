/*
 Based on Bluetooth Serial Passthrough Sketch by Jim Lindblom
 modifications by: Shanel Wu

 The original code converts an RN-42 bluetooth module to
 communicate at 9600 bps (from 115200), and passes any serial
 data between Serial Monitor and bluetooth module. Additional 
 code configures the RN-42 to communicate through the HID 
 profile as a mouse.

 This sketch takes in characters through serial to simulate 
 other analog and digital sensors connected to the Arduino,
 which corresponds to X-Y input for the cursor position.
 WASD = up/left/down/right respectively
 */
#include <SoftwareSerial.h>  

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3
// const int led = 5; // LED connected to PWM pin 5
const int buttonA = A0; // one button/analog input connected to pin 5

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
  Serial.begin(9600);  // Begin the serial monitor at 9600bps
  //pinMode(led, OUTPUT);
  pinMode(buttonA, INPUT);
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
    // Send HID reports
    char input = Serial.read();
    // send mouse down
    if (input == 's') {
      bluetooth.write(0xFD); // start byte
      bluetooth.write(0x5);
      bluetooth.write(0x2);
      bluetooth.write((byte)0);
      bluetooth.write((byte)0); // X movement
      bluetooth.write(5);  // Y movement
      bluetooth.write((byte)0);
    } else if (input == 'w'){ // send mouse up
      bluetooth.write(0xFD); // start byte
      bluetooth.write(0x5);
      bluetooth.write(0x2);
      bluetooth.write((byte)0);
      bluetooth.write((byte)0); // X movement
      bluetooth.write(-5);  // Y movement
      bluetooth.write((byte)0);      
    } else if (input == 'd'){ // send mouse right
      bluetooth.write(0xFD); // start byte
      bluetooth.write(0x5);
      bluetooth.write(0x2);
      bluetooth.write((byte)0);
      bluetooth.write(5); // X movement
      bluetooth.write((byte)0);  // Y movement
      bluetooth.write((byte)0);      
    } else if (input == 'a'){ // send mouse left
      bluetooth.write(0xFD); // start byte
      bluetooth.write(0x5);
      bluetooth.write(0x2);
      bluetooth.write((byte)0);
      bluetooth.write(-5); // X movement
      bluetooth.write((byte)0);  // Y movement
      bluetooth.write((byte)0);      
    } else if (input == 'f'){ // send mouse click
      Serial.println("click");
      bluetooth.write(0xFD); // start byte
      bluetooth.write(0x5);
      bluetooth.write(0x2);
      bluetooth.write(0x1); // left button click?
      bluetooth.write((byte)0); // no X movement
      bluetooth.write((byte)0); // no Y movement
      bluetooth.write((byte)0); // no wheel movement
    } else {
      Serial.println("invalid mouse input");
    }
  }
  // and loop forever and ever!
}
