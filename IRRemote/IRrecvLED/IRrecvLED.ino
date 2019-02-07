/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

int RECV_PIN = 11;
// HEX ONVAL = 0x1FE58A7;
// HEX OFFVAL = 0x1FEA05F;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  pinMode(12, OUTPUT);
  
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop() {
  // HEX ONVAL = 1FE58A7;
  // HEX OFFVAL = 1FEA05F;
  if (irrecv.decode(&results)) {
    if (results.value == 0x1FE58A7) {
      Serial.println("turning on");
      digitalWrite(12, HIGH);
    } else if (results.value == 0x1FEA05F) {
      Serial.println("turning off");
      digitalWrite(12, LOW);
    }
    // Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
