// Binary Pomodoro Timer - geeky productivity aid
// Copyright (C) 2015 Robbie Nichols
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.

// Use an arduino to count down the standard pomodoro times and breaks in binary
// 4 25 minute period with 5 min breaks between and a 20 minute break after the 4th
// It uses 11 GPIO pins for the seconds countdown
//          2 pins to track the round (0 - 3)
//          1 pin to denote the break perid (second half of the round)
//          1 pin for an interupt to advance to the next round
//          1 pin for a button to start the current period

// I'm glad that the Arduino analog pins can be used for GPIO, too

// Start button still needs to be implemented

#define NEXT_BTN_PIN 2
#define START_BTN_PIN 3

#define BIN_PIN0 9
#define BIN_PIN1 8
#define BIN_PIN2 7
#define BIN_PIN3 6
#define BIN_PIN4 5
#define BIN_PIN5 4
#define BIN_PIN6 10
#define BIN_PIN7 11
#define BIN_PIN8 12
#define BIN_PIN9 13
#define BIN_PIN10 A0

#define ROUND_PIN0 A2
#define ROUND_PIN1 A3

#define BREAK_PIN A1

// set initial variables
int start; //var to track the time of start
volatile int i; //counter var
volatile int rnd = 0; // first round is 0
volatile int count_down = 1500; // 25 minutes

void setup() {
  // Set pin modes for buttons
  pinMode(NEXT_BTN_PIN, INPUT_PULLUP);
  pinMode(START_BTN_PIN, INPUT_PULLUP);
  
  // Set pin modes for all the LEDs
  pinMode(BIN_PIN0, OUTPUT);
  pinMode(BIN_PIN1, OUTPUT);
  pinMode(BIN_PIN2, OUTPUT);
  pinMode(BIN_PIN3, OUTPUT);
  pinMode(BIN_PIN4, OUTPUT);
  pinMode(BIN_PIN5, OUTPUT);
  pinMode(BIN_PIN6, OUTPUT);
  pinMode(BIN_PIN7, OUTPUT);
  pinMode(BIN_PIN8, OUTPUT);
  pinMode(BIN_PIN9, OUTPUT);
  pinMode(BIN_PIN10, OUTPUT);
  pinMode(ROUND_PIN0, OUTPUT);
  pinMode(ROUND_PIN1, OUTPUT);
  pinMode(BREAK_PIN, OUTPUT);

  attachInterrupt(0, next, LOW);

  //Serial.begin(9600);      // open the serial port at 9600 bps:   
}

void loop() {
  // 
  if ( ! digitalRead(START_BTN_PIN) ){
    start = millis();
    
    do {
      i = count_down - ((millis() - start) / 1000); //i is an int, so this should round fine
      
      //Serial.print(i);       // prints a label
      //Serial.print("\t");              // prints a tab
      //Serial.print(start - millis());
      //Serial.print("\n");              // prints a tab
      
      //write out all the counter bits
      digitalWrite(BIN_PIN0, i %2);
      digitalWrite(BIN_PIN1, i/2 %2);
      digitalWrite(BIN_PIN2, i/4 %2);
      digitalWrite(BIN_PIN3, i/8 %2);
      digitalWrite(BIN_PIN4, i/16 %2);
      digitalWrite(BIN_PIN5, i/32 %2);
      digitalWrite(BIN_PIN6, i/64 %2);
      digitalWrite(BIN_PIN7, i/128 %2);
      digitalWrite(BIN_PIN8, i/256 %2);
      digitalWrite(BIN_PIN9, i/512 %2);
      digitalWrite(BIN_PIN10, i/1024 %2);
      delay(1000);
    } while (i >= 0);
    
    next();
  }
}

void next () {
  if ( i >= 0 ){
    i = -1;
  } else {
    
    rnd++;
    
    if ( rnd%2 ){
      count_down = 300;
    } else {
      count_down = 1500;
    }
    
    if ( rnd == 7 ){
      count_down = 1200;
    } else if ( rnd == 8 ){
      rnd = 0;
    }
   
    digitalWrite(BREAK_PIN, rnd %2);
    digitalWrite(ROUND_PIN0, rnd/2 %2);
    digitalWrite(ROUND_PIN1, rnd/3 %2);
  }

}
