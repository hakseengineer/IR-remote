/*
 * Author : Hak Se Engineer
 * Version : 0.1
 * Date : 11/12/2024
 * 
 */

#include <IRremote.hpp>

int IR_RECEIVE_PIN = 2;
String recvString;


void setup()
{
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK); // Start the receiver
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (IrReceiver.decode()) {
    recvString =  String(IrReceiver.decodedIRData.decodedRawData, HEX);  
    Serial.println(recvString);
    IrReceiver.resume(); // Receive the next value
  }
  if(recvString == "d002ff")
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  delay(100);      
}
