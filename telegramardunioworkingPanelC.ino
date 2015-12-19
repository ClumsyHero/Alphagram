#include <OscSerial.h>


/////////////Panel C - PURPLE

// tile support
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OscUDP.h>

int stopGap = 28;

int baudRate = 9600;
OscSerial oscSerial;
long serialTimerEnd;
long serialTimerInterval = 100;

int buttonState = 0;
const int buttonPin = 7;
int buttonValue;

int oldpress;
int newpress;
long endTime;
long aEndTime;
long bEndTime;
int test = 0;
int timeDelay = 500;

#include "FastLED.h"
#define NUM_LEDS 28
int numLed = 28;
#define DATA_PIN 6
CRGB leds[NUM_LEDS];

int timer = 27;
int ledNumber = 0;
int sendValue = stopGap;
int receivedValueA = stopGap;
int receivedValueB = stopGap;

byte mac[] = {
  0x90, 0xA2, 0xDA, 0x0E, 0xC3, 0xC3
};
////0x90, 0xA2, 0xDA, 0x0E, 0xC0, 0xA9   ///A
///0x90, 0xA2, 0xDA, 0x0E, 0xC0, 0x0D    ///B
////0x90, 0xA2, 0xDA, 0x0E, 0xC3, 0xC3   ///C

//Orange 0xF06F00
//Blue 0x0FC92
//Purple 0x6603A0
long int colorPrimary = 0x6603A0;

unsigned int listeningPort = 12112;

IPAddress panelADestIP(141, 117, 45, 111);
int panelAPort = 12111;
long int colorSecondary = 0x0FC92;

IPAddress panelBDestIP(141, 117, 45, 113);
int panelBPort = 12113;
long int colorTertiary = 0xF06F00;

EthernetUDP UDP;
OscUDP oscUdp;
NetAddress panelADest;
NetAddress panelBDest;
long netTimerEnd;
long netTimerInterval = 100;

void setup() {
  oscSerial.begin(Serial);

  // start ethernet on the shield
  Ethernet.begin(mac);

  // start UDP object, listening on port listeningPort
  UDP.begin(listeningPort);

  // display IP in serial debug (uncomment to use)
  Serial.begin(baudRate);
  Serial.println(Ethernet.localIP());
  oscUdp.begin(UDP);
  panelADest.set(panelADestIP, panelAPort);
  panelBDest.set(panelBDestIP, panelBPort);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  pinMode(buttonPin, INPUT);
}

void loop() {
  //  limits sends to every 100 mS


  if (millis() >= serialTimerEnd) {
    //sendOscSerial();
    if (sendValue != stopGap)
    {
      sendOscNet();
    }
    serialTimerEnd = millis() + serialTimerInterval;
  }

  // important! this is needed in order to receive messages
  // oscSerial.listen();
  newpress = digitalRead(buttonPin);

  if (newpress > oldpress) {

    buttonValue = timer;
    if (timer == 17) {
      buttonValue = 9;
    }
    if (timer == 16) {
      buttonValue = 10;
    }
    if (timer == 15) {
      buttonValue = 11;
    }
    if (timer == 14) {
      buttonValue = 12;
    }
    if (timer == 13) {
      buttonValue = 13;
    }
    if (timer == 12) {
      buttonValue = 14;
    }
    if (timer == 11) {
      buttonValue = 15;
    }
    if (timer == 10) {
      buttonValue = 16;
    }
    if (timer == 9) {
      buttonValue = 17;
    }
    sendValue = buttonValue;
  } else {
    buttonValue = stopGap;
  }

  //Serial.println(sendValue);
  //Serial.println(test);

  if ( millis() > endTime) {
    timer--;
    if (timer <= -1) {
      timer = 27;
    }

    endTime = millis() + timeDelay;

  }

  if (timer == 26) {
    leds[0] = CRGB::Black;
    FastLED.show();
  }

  if (timer <= 26 && timer >= 18) {
    leds[timer] = colorPrimary;
    FastLED.show();
    leds[timer + 1] = CRGB::Black;
    FastLED.show();
  }
  if (timer == 17) {
    leds[9] = colorPrimary;
    FastLED.show();
    leds[18] = CRGB::Black;
    FastLED.show();
  }
  if (timer == 16) {
    leds[10] = colorPrimary;
    FastLED.show();
    leds[9] = CRGB::Black;
    FastLED.show();
  }
  if (timer == 15) {
    leds[11] = colorPrimary;
    FastLED.show();
    leds[10] = CRGB::Black;
    FastLED.show();
  }
  if (timer == 14) {
    leds[12] = colorPrimary;
    FastLED.show();
    leds[11] = CRGB::Black;
    FastLED.show();
  }
  if (timer == 13) {
    leds[13] = colorPrimary;
    FastLED.show();
    leds[12] = CRGB::Black;
    FastLED.show();
  }
  if (timer == 12) {
    leds[14] = colorPrimary;
    FastLED.show();
    leds[13] = CRGB::Black;
    FastLED.show();
  }
  if (timer == 11) {
    leds[15] = colorPrimary;
    FastLED.show();
    leds[14] = CRGB::Black;
    FastLED.show();
  }
  if (timer == 10) {
    leds[16] = colorPrimary;
    FastLED.show();
    leds[15] = CRGB::Black;
    FastLED.show();
  }
  if (timer == 9) {
    leds[17] = colorPrimary;
    FastLED.show();
    leds[16] = CRGB::Black;
    FastLED.show();
  }

  if (timer == 8 ) {
    leds[17] = CRGB::Black;
    FastLED.show();
  }
  if (timer <= 8 && timer >= 0) {
    leds[timer] = colorPrimary;
    FastLED.show();
    leds[timer + 1] = CRGB::Black;
    FastLED.show();
  }

  // important! this is needed in order to receive messages
  oscUdp.listen();

  if (millis() > aEndTime)
  {
    leds[receivedValueA] = CRGB::Black;
    FastLED.show();
  }

  if (millis() > bEndTime)
  {
    leds[receivedValueB] = CRGB::Black;
    FastLED.show();
  }

  oldpress = newpress;


}

// incoming messages will arrive here
void oscEvent(OscMessage & msg) {
  // handle data here
  // OR
  // route messages using the "plug" functionality

  // this example redirects messages called "/incoming" to myFunction (below)
  msg.plug("/panelA", panelARead );

  msg.plug("/panelB", panelBRead);
}

// plugged function
void panelARead(OscMessage & msg) {
  // get the data out of the message
  int data = msg.getInt(0);
  if (data != stopGap)
  {
    if (receivedValueA != data)
    {
      leds[receivedValueA] = CRGB::Black;
      FastLED.show();
      //Serial.println(data);
      receivedValueA = data;
      leds[receivedValueA] = colorSecondary;
      FastLED.show();
      aEndTime = millis() + (timeDelay*4);
    }
  }
}

// plugged function
void panelBRead(OscMessage & msg) {
  // get the data out of the message
  int data = msg.getInt(0);
  if (data != stopGap)
  {
    if (receivedValueB != data)
    {
      leds[receivedValueB] = CRGB::Black;
      FastLED.show();
      //Serial.println(data);
      receivedValueB = data;
      leds[receivedValueB] = colorTertiary;
      FastLED.show();
      bEndTime = millis() + (timeDelay*4);
    }
  }
}

// plugged function
void panelCRead(OscMessage & msg) {

  // get the data out of the message
//  int data = msg.getInt(0);
//  if (data != stopGap)
//  {
//    if (receivedValueC != data)
//    {
//      leds[receivedValueC] = CRGB::Black;
//      FastLED.show();
//      //Serial.println(data);
//      receivedValueC = data;
//      leds[receivedValueC] = colorTertiary;
//      FastLED.show();
//      cEndTime = millis() + 3000;
//    }
//  }
}

// send OSC Serial messages
void sendOscNet() {
  if (sendValue != stopGap)
  {
    OscMessage msgPanelA("/panelC");      //identify ourselves as panel C
    msgPanelA.add(sendValue);             //attach the value we want to send
    oscUdp.send(msgPanelA, panelADest);   //send to panel A

    OscMessage msgPanelB("/panelC");      //identify ourselves as panel C
    msgPanelB.add(sendValue);             //attach the value we want to send
    oscUdp.send(msgPanelB, panelBDest);   //send to value B
  }
  sendValue = stopGap;
}
