/*

    Copyright (C) 2018 Mauricio Bustos (m@bustos.org)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "poofer.h"
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_Trellis.h"

void processAuxCommands();
void processRcCommands();
void processKeyboard();
void clearDisplay();
void displayPooferStatus();

poofer poof = poofer();
Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);
#define NUMTRELLIS 1
#define numKeys (NUMTRELLIS * 16)

int COMMAND_STARTED_LED_1 = 5;
int COMMAND_STARTED_LED_2 = 6;
int COMMAND_STARTED_LED_3 = 9;
int COMMAND_STARTED_LED_4 = 10;

#define POOFER_DEBOUNCE (20)
#define POOFER_COMMAND_START_WINDOW (500)
#define POOFER_COMMAND_WINDOW (4000)

int aux1pin = 12;
int aux2pin = 13;
int aux3pin = A2;
int aux4pin = A3;

const int BUTTON_COUNT = 3;
int buttonPin[BUTTON_COUNT] = {5, 6, 7};
unsigned long buttonStart[BUTTON_COUNT] = {0, 0, 0};
unsigned long buttonLimit[BUTTON_COUNT] = {1000, 1000, 1000};
unsigned long buttonWait[BUTTON_COUNT] = {0, 0, 0};

long pooferCommandStart = 0;
long pooferCommandDetect = 0;
long pooferStartControl = 0;

void setup() {
  Serial.begin(9600);
  pinMode(aux1pin, INPUT);
  pinMode(aux2pin, INPUT);
  pinMode(aux3pin, INPUT);
  pinMode(aux4pin, INPUT);

  for (int i = 0; i < BUTTON_COUNT; i++) {
    pinMode(buttonPin[i], INPUT_PULLUP);    
  }

  trellis.begin(0x70);
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();    
    delay(30);
  }
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();    
    delay(30);
  }
}

int keyForButton(int i) {
  if (i == 0) return 0;
  if (i == 1) return 3;
  if (i == 2) return 12;
  return 0;
}

void loop() {
  if (!poof.patternRunning()) {
    //processRcCommands();
  } else {
    poof.iteratePattern();
  }
  for (int i = 0; i < BUTTON_COUNT; i++) {
    if (buttonWait[i] > 0 && millis() - buttonWait[i] > 5000) {
      Serial.print("Button wait cleared - "); Serial.println(i);
      buttonWait[i] = 0;
    }
    if (!digitalRead(buttonPin[i])) {
      if (buttonWait[i] == 0 && buttonStart[i] == 0) {
        Serial.print("Button pressed - "); Serial.println(i);
        poof.poof(i, 1);
        trellis.setLED(keyForButton(i));
        buttonStart[i] = millis();
      }
      if (buttonStart[i] > 0 && millis() - buttonStart[i] > buttonLimit[i]) {
        Serial.print("Button off for limit - "); Serial.println(i);
        buttonStart[i] = 0;
        buttonWait[i] = millis();
        poof.poof(i, 0);
        trellis.clrLED(keyForButton(i));
      }
    } else {
      if (buttonStart[i] > 0) {Serial.print("Button released - "); Serial.println(i);}
      poof.poof(i, 0);
      trellis.clrLED(keyForButton(i));
      buttonStart[i] = 0;
    }
  }
  if (pooferCommandStart <= 0) {
    //clearDisplay();
    //processKeyboard();
    //displayPooferStatus();
  }
  trellis.writeDisplay();
}

//! Clear display
void clearDisplay() {
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.clrLED(i);
  }
}

//! Show which valves are open
void displayPooferStatus() {
  if (poof.patternRunning()) {
    delay(30);
    for (int i = 0; i < POOFER_COUNT; i++) {
      if (poof.valveOpen(i)) {
        trellis.setLED(poof.leds[i]);
      }
    }
  }
}

//! Process keyboard presses
void processKeyboard() {
  if (!poof.patternRunning()) {
    delay(30);
    if (trellis.readSwitches()) {
      for (uint8_t i = 0; i < numKeys; i++) {
        if (trellis.justPressed(i)) {
          Serial.print("v"); Serial.println(i);
          trellis.setLED(i);
        } 
        if (trellis.justReleased(i)) {
          Serial.print("^"); Serial.println(i);
          trellis.clrLED(i);
        }
      }
    }
  }
}

//! Process command
void processCommand(int pin, int pattern) {
  if (digitalRead(pin) == HIGH) {
    if (pooferStartControl > 0) {
      if (millis() - pooferStartControl > POOFER_DEBOUNCE) {
        poof.startPattern(pattern);
        pooferStartControl = 0;
        pooferCommandStart = 0;
        Serial.println(String("Command ") + String(pattern));
        for (uint8_t i = 0; i < numKeys; i++) {
          trellis.clrLED(i);
        }
        trellis.setLED(pattern);
      }
    } else {
      pooferStartControl = millis();
    }
  }
}

//! Process incoming aux commands
void processRcCommands() {
  if (digitalRead(aux1pin) == HIGH && pooferCommandStart == 0) {
    if (pooferCommandDetect > 0) {
      if (millis() - pooferCommandDetect > POOFER_DEBOUNCE) {
        pooferCommandStart = millis();
        pooferCommandDetect = 0;
        Serial.println("Command Started...");
        trellis.setLED(COMMAND_STARTED_LED_1);
        trellis.setLED(COMMAND_STARTED_LED_2);
        trellis.setLED(COMMAND_STARTED_LED_3);
        trellis.setLED(COMMAND_STARTED_LED_4);
      }
    } else {
      pooferCommandDetect = millis();
    }
  }
  if (pooferCommandStart > 0) {
    long timeSince = millis() - pooferCommandStart;
    if (timeSince > POOFER_COMMAND_WINDOW) {
      pooferCommandStart = 0;
      pooferStartControl = 0;
      Serial.println("Command Timeout");
      trellis.clrLED(COMMAND_STARTED_LED_1);
      trellis.clrLED(COMMAND_STARTED_LED_2);
      trellis.clrLED(COMMAND_STARTED_LED_3);
      trellis.clrLED(COMMAND_STARTED_LED_4);
    } else if (timeSince > POOFER_COMMAND_START_WINDOW) {
      processCommand(aux1pin, 0);
      processCommand(aux2pin, 1);
      processCommand(aux3pin, 2);
      processCommand(aux4pin, 3);
    }
  }
}
