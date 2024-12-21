/*
 * Author : Hak Se Engineer
 * Version : 1.1
 * Date : 13/12/2024
 * 
 */

#include "IRremote.hpp"
#include <Adafruit_NeoPixel.h>
#include <Servo.h>

// Which pin on the Arduino is connected to the IR Reciever?
int IR_RECEIVE_PIN = 2;

int RELAY_PIN = 4;

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 24 // Popular NeoPixel ring size
// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// pixelss you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


//Button Name
enum mode {Power, Center, Up, Right, Down, Left, Low, High, Wrong};
// Hex code for respactive buttons
const char *codes[] = {"ab054f", "d002ff", "6509af", "150eaf", "e501af", "9506af",  "8b074f", "b0f4f"};

String recvString;

int pos = 0;    // variable to store the servo position
int LED_state = LOW;
int Relay_state = LOW;
int toggle = 0;

Servo myservo;  // create servo object to control a servo


void setup()
{
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK); // Start the receiver
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  myservo.attach(11);  // attaches the servo on pin 9 to the servo object
  pixels.begin(); // INITIALIZE NeoPixel pixels object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'
}

void loop() {

  if (IrReceiver.decode()) {
    recvString =  String(IrReceiver.decodedIRData.decodedRawData, HEX);   
    Serial.println(recvString);
    IrReceiver.resume(); // Receive the next value
  
    mode input = stringToMode(recvString.c_str());

    switch(input)
    {
      case Power:  // power
        Relay_state = !Relay_state;
        digitalWrite(RELAY_PIN, Relay_state);
        break;
      case Center:  //center
          toggle = !toggle;
          if(toggle)
          {
            rainbowCycle(5);
          }
          else
          {
            pixels.clear();
            pixels.show();
          }
        break;
      case Up:  // up
        setPixel(0,NUMPIXELS/4);
        break;
      case Right:  // right
        setPixel((NUMPIXELS/4),2*(NUMPIXELS/4));     
        break;
      case Down:  // down
        setPixel(2*(NUMPIXELS/4),3*(NUMPIXELS/4)); 
        break;
      case Left:  // left
        setPixel(3*(NUMPIXELS/4),NUMPIXELS); 
        break;
      case Low:  // low
        pos -=30;
        if(pos <= 0)
        {
          pos =1;
        }
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        pixels.setBrightness(pos);
        pixels.show();
        break;
      case High:  // high
        pos +=30;
        if(pos > 180)
        {
          pos =180;
        }
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        pixels.setBrightness(pos);
        pixels.show();
        break;
      default:
        LED_state = !LED_state;
        digitalWrite(LED_BUILTIN, LED_state);
        break;
    }


  }
  delay(100); 
}

void setPixel(int start, int end)
{
  pixels.clear(); // Set all pixel colors to 'off'
  for(int i=start; i<end; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(100, 0, 0));
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*2; j++) { // 2 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

mode stringToMode(const char *RcvCode) {
    if (strcmp(RcvCode, codes[0]) == 0) {
        return Power;
    } else if (strcmp(RcvCode, codes[1]) == 0) {
        return Center;
    } else if (strcmp(RcvCode, codes[2]) == 0) {
        return Up;
    } else if (strcmp(RcvCode, codes[3]) == 0) {
        return Right;
    } else if (strcmp(RcvCode, codes[4]) == 0) {
        return Down;
    } else if (strcmp(RcvCode, codes[5]) == 0) {
        return Left;
    } else if (strcmp(RcvCode, codes[6]) == 0) {
        return Low;
    } else if (strcmp(RcvCode, codes[7]) == 0) {
        return High;
    } else {
        return Wrong;
    }
}

