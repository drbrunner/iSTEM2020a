// Adafruit IO Digital Input Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-input
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.
// I showed this to Year 10 when using GitHub

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// digital pin 5
#define BUTTON_PIN 5 //This is the D1 pin on the board

// digital pin 4
#define LED_PIN 4 //this is the D2 pin on the board

// button state
bool current = false;
bool last = false;

// set up the 'digital' feed
AdafruitIO_Feed *digital = io.feed("digital");


AdafruitIO_Feed *digitalLED = io.feed("digital"); //This is the LED button on adafruit


void setup() {

  // set button pin as an input
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'digital' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  digitalLED->onMessage(handleMessage);
  

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  digitalLED->get();
}


void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // grab the current state of the button.
  // we have to flip the logic because we are
  // using a pullup resistor.
  if(digitalRead(BUTTON_PIN) == LOW)
    current = true;
  else
    current = false;

  // return if the value hasn't changed
  if(current == last)
    return;

  // save the current state to the 'digital' feed on adafruit io
  Serial.print("sending button -> ");
  Serial.println(current);
  digital->save(current);

  // store last button state
  last = current;

}

// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");

  if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");


  digitalWrite(LED_PIN, data->toPinLevel());
}
