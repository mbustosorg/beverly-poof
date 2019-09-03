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
#include "Arduino.h"

//! Create the poofer object
poofer::poofer() {
  for (int i = 0; i < POOFER_COUNT; i++) {
    pinMode(pooferPins[i], OUTPUT);
    digitalWrite(pooferPins[i], LOW);
    pinMode(ignitorPins[i], OUTPUT);
    digitalWrite(ignitorPins[i], LOW);
  }

  runningPattern = PATTERNS_OFF;
  patternStartTime = 0;
  stepNumber = 0;

  pattern* patternsSetup = (pattern *) malloc(PATTERN_COUNT * sizeof(pattern));
  patterns = patternsSetup;

  // Pattern 1
  patternStep* pattern1steps = (patternStep *) malloc(PATTERN_1_COUNT * sizeof(patternStep));
  pattern1steps[0] = {0, {HIGH, LOW, LOW}};
  pattern1steps[1] = {100, {LOW, LOW, LOW}};
  pattern1steps[2] = {500, {LOW, HIGH, HIGH}};
  pattern1steps[3] = {600, {LOW, LOW, LOW}};
  pattern1steps[4] = {1000, {HIGH, LOW, LOW}};
  pattern1steps[5] = {1100, {LOW, LOW, HIGH}};
  pattern1steps[6] = {1300, {LOW, HIGH, LOW}};
  pattern1steps[7] = {1400, {LOW, LOW, HIGH}};  
  pattern1steps[8] = {2000, {HIGH, HIGH, LOW}};
  pattern1steps[9] = {2500, {LOW, LOW, LOW}};
  pattern1steps[10] = {3000, {HIGH, HIGH, HIGH}};
  pattern1steps[11] = {3500, {LOW, LOW, LOW}};
  pattern1steps[12] = {4500, {HIGH, HIGH, LOW}};
  pattern1steps[13] = {5000, {HIGH, HIGH, HIGH}};
  pattern1steps[14] = {5500, {HIGH, HIGH, HIGH}};
  pattern1steps[15] = {6000, {HIGH, HIGH, LOW}};
  pattern1steps[16] = {6500, {HIGH, HIGH, LOW}};
  pattern1steps[17] = {8000, {HIGH, HIGH, LOW}};
  patterns[0] = {PATTERN_1_COUNT, pattern1steps};

  // Pattern 2
  patternStep* pattern2steps = (patternStep *) malloc(PATTERN_2_COUNT * sizeof(patternStep));
  pattern2steps[0] = {0, {HIGH, LOW, LOW}};
  pattern2steps[1] = {100, {LOW, HIGH, LOW}};
  pattern2steps[2] = {200, {HIGH, LOW, LOW}};
  pattern2steps[3] = {300, {LOW, HIGH, LOW}};
  pattern2steps[4] = {400, {HIGH, LOW, LOW}};
  pattern2steps[5] = {500, {LOW, HIGH, LOW}};
  pattern2steps[6] = {600, {HIGH, LOW, LOW}};
  pattern2steps[7] = {700, {LOW, HIGH, LOW}};
  pattern2steps[8] = {800, {HIGH, LOW, LOW}};
  pattern2steps[9] = {900, {LOW, HIGH, LOW}};
  pattern2steps[10] = {1000, {HIGH, LOW, LOW}};
  pattern2steps[11] = {1100, {LOW, HIGH, LOW}};
  pattern2steps[12] = {1200, {HIGH, LOW, LOW}};
  pattern2steps[13] = {1300, {LOW, HIGH, LOW}};
  pattern2steps[14] = {1400, {HIGH, LOW, LOW}};
  pattern2steps[15] = {1500, {LOW, HIGH, LOW}};
  pattern2steps[16] = {1600, {HIGH, LOW, LOW}};
  pattern2steps[17] = {1700, {LOW, HIGH, LOW}};
  pattern2steps[18] = {1800, {HIGH, LOW, LOW}};
  pattern2steps[19] = {1900, {LOW, HIGH, LOW}};
  pattern2steps[20] = {2000, {HIGH, LOW, LOW}};
  pattern2steps[21] = {2100, {LOW, HIGH, LOW}};
  pattern2steps[22] = {2200, {HIGH, LOW, LOW}};
  pattern2steps[23] = {2300, {LOW, HIGH, LOW}};
  pattern2steps[24] = {2400, {HIGH, LOW, LOW}};
  pattern2steps[25] = {2500, {LOW, LOW, LOW}};
  patterns[1] = {PATTERN_2_COUNT, pattern2steps};

  // Pattern 3
  patternStep* pattern3steps = (patternStep *) malloc(PATTERN_3_COUNT * sizeof(patternStep));
  pattern3steps[0] = {0, {HIGH, LOW, LOW}};
  pattern3steps[1] = {100, {LOW, HIGH, LOW}};
  pattern3steps[2] = {200, {LOW, LOW, LOW}};
  pattern3steps[3] = {300, {LOW, HIGH, LOW}};
  pattern3steps[4] = {400, {HIGH, LOW, LOW}};
  pattern3steps[5] = {500, {LOW, LOW, LOW}};
  pattern3steps[6] = {600, {HIGH, LOW, LOW}};
  pattern3steps[7] = {700, {LOW, HIGH, LOW}};
  pattern3steps[8] = {800, {LOW, LOW, LOW}};
  pattern3steps[9] = {900, {LOW, HIGH, LOW}};
  pattern3steps[10] = {1000, {HIGH, LOW, LOW}};
  pattern3steps[11] = {1100, {LOW, LOW, LOW}};
  pattern3steps[12] = {1200, {HIGH, HIGH, LOW}};
  pattern3steps[13] = {1500, {LOW, LOW, LOW}};
  pattern3steps[14] = {2000, {HIGH, HIGH, LOW}};
  pattern3steps[15] = {2500, {LOW, LOW, LOW}};
  pattern3steps[16] = {3000, {HIGH, HIGH, LOW}};
  pattern3steps[17] = {3500, {LOW, LOW, LOW}};
  patterns[2] = {PATTERN_3_COUNT, pattern3steps};

  // Pattern 4
  patternStep* pattern4steps = (patternStep *) malloc(PATTERN_4_COUNT * sizeof(patternStep));
  pattern4steps[0] = {0, {HIGH, LOW, LOW}};
  pattern4steps[1] = {100, {LOW, LOW, LOW}};
  pattern4steps[2] = {200, {HIGH, LOW, LOW}};
  pattern4steps[3] = {300, {LOW, LOW, LOW}};
  pattern4steps[4] = {400, {HIGH, LOW, LOW}};
  pattern4steps[5] = {500, {LOW, LOW, LOW}};
  pattern4steps[6] = {600, {HIGH, LOW, LOW}};
  pattern4steps[7] = {700, {LOW, HIGH, LOW}};
  pattern4steps[8] = {800, {LOW, LOW, LOW}};
  pattern4steps[9] = {900, {LOW, HIGH, LOW}};
  pattern4steps[10] = {1000, {LOW, LOW, LOW}};
  pattern4steps[11] = {1100, {LOW, HIGH, LOW}};
  pattern4steps[12] = {1200, {LOW, LOW, LOW}};
  pattern4steps[13] = {1300, {LOW, HIGH, LOW}};
  pattern4steps[14] = {1400, {LOW, LOW, LOW}};
  pattern4steps[15] = {1500, {HIGH, HIGH, LOW}};
  pattern4steps[16] = {2000, {HIGH, HIGH, LOW}};
  pattern4steps[17] = {2200, {LOW, LOW, LOW}};
  pattern4steps[18] = {2500, {HIGH, HIGH, LOW}};
  pattern4steps[19] = {2700, {HIGH, HIGH, LOW}};
  pattern4steps[20] = {2900, {LOW, LOW, LOW}};
  pattern4steps[21] = {3200, {HIGH, HIGH, LOW}};
  pattern4steps[22] = {3400, {HIGH, HIGH, LOW}};
  pattern4steps[23] = {3600, {LOW, LOW, LOW}};
  patterns[3] = {PATTERN_4_COUNT, pattern4steps};
}

//! Destroy the object
poofer::~poofer() {
}

//! Create multi poofers
void poofer::startPattern(int id) {
  if (id >= 0 && id < PATTERN_COUNT) {
    if (runningPattern != id) {
      runningPattern = id;
      patternStartTime = millis();
      stepNumber = 0;
      for (int i = 0; i < POOFER_COUNT; i++) {
      	digitalWrite(pooferPins[i], patterns[runningPattern].steps[stepNumber].pooferState[i]);
      }
      stepNumber++;
    }
  }
}

//! Iterate the current pattern
void poofer::iteratePattern() {
  if (runningPattern != PATTERNS_OFF) {
    if (stepNumber >= patterns[runningPattern].numberOfSteps) {
      runningPattern = PATTERNS_OFF;
      // Safety Shutdown
      for (int i = 0; i < POOFER_COUNT; i++) {
      	digitalWrite(pooferPins[i], LOW);
      	digitalWrite(ignitorPins[i], LOW);
      }
    } else if (runningPattern >= 0) {
      unsigned long currentDiff = millis() - patternStartTime;
      if (currentDiff >= patterns[runningPattern].steps[stepNumber].stepStart) {
        Serial.print("Step: "); Serial.println(String(stepNumber));
      	for (int i = 0; i < POOFER_COUNT; i++) {
	        digitalWrite(pooferPins[i], patterns[runningPattern].steps[stepNumber].pooferState[i]);
          if (patterns[runningPattern].steps[stepNumber].pooferState[i] == HIGH) {
            ignitorState[i] = !ignitorState[i];
  	        digitalWrite(ignitorPins[i], ignitorState[i]);
          } else {
  	        digitalWrite(ignitorPins[i], LOW);
          }
	      }
	      stepNumber++;
      }
    }
  }
}

//! Is a certain valve on?
bool poofer::valveOpen(int valve) {
  if (runningPattern == PATTERNS_OFF || stepNumber >= patterns[runningPattern].numberOfSteps) {
    return false;
  } else if (valve < POOFER_COUNT) {
    return patterns[runningPattern].steps[stepNumber].pooferState[valve];
  }
  return false;
}

//! Pattern runnning?
bool poofer::patternRunning() {
  return runningPattern != PATTERNS_OFF;
}

//! Poof on command
void poofer::poof(int id, int state) {
  if (runningPattern == PATTERNS_OFF || state == HIGH) {
    if (id >= 0 && id < POOFER_COUNT) {
      runningPattern = PATTERNS_OFF;
      //Serial.println(id);
      digitalWrite(pooferPins[id], state);
    }
  }
}



