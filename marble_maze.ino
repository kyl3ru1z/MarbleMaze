#include <Servo.h>
#include "pitches.h"
  
Servo servo1;
Servo servo2;

const int SPEAKER = 13;
const int joyX = A0;
const int joyY = A1;
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 7;
const int servo1Pin = 3;
const int servo2Pin = 5;
const int startButton = 12;   
const int endButton = 11;

int servoVal;

int startButtonState;
int endButtonState;

unsigned long startTime;
unsigned long endTime;
unsigned long elapsedTime;
  
int introNotes1[] = 
{
  NOTE_D4, NOTE_D4, NOTE_E5, NOTE_G6, NOTE_E5, NOTE_A6, 0, NOTE_B5,
  0, NOTE_C7, NOTE_B5, NOTE_G5, NOTE_D3, NOTE_E4, 0, NOTE_D4, 0
};

int introDuration1[] = 
{
  200, 200, 200, 200, 200, 200, 400, 200, 
  200, 200, 200, 200, 200, 200, 400, 200, 
};

int introNotes2[] = 
{
  NOTE_F4, NOTE_A3, NOTE_B4, NOTE_F4, NOTE_A3, NOTE_B4, 
  NOTE_F4, NOTE_A3, NOTE_B4, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_D5, 
  NOTE_B4, NOTE_G4, NOTE_E4, NOTE_D4, 
  NOTE_E4, NOTE_G4, NOTE_E4, 0
};

int introDuration2[] = 
{
  200, 200, 400, 200, 200, 400,
  200, 200, 200, 200, 400, 200, 200, 
  200, 200, 1000, 200,
  200, 200, 1200,
};
  
void setup() 
{
  randomSeed(analogRead(A2));   // generating a random seed so that random numbers are generated
  Serial.begin(9600);
  
  int randomIntro = random(2);  // generates a random number from 0 to 1
  switch (randomIntro)          // uses the random number to play a random introTone
  {
    case 0:
      Serial.println("Playing intro 1");  
      for (int i = 0; i < 17; i++) // Loops through notes and durations to play tones
      {
        tone(SPEAKER, introNotes1[i], introDuration1[i]);
        if (i == 16)  
        {
          noTone(SPEAKER);
          break;
        } 
        delay(introDuration1[i]);
      }
      break;
    case 1:
      Serial.println("Playing Intro 2");
      for (int i = 0; i < 21; i++)
      {
        tone(SPEAKER, introNotes2[i], introDuration2[i]);
        if (i == 20) 
        {
          noTone(SPEAKER); 
          break;
        }
        delay(introDuration2[i]);
      }
      break;  
  }

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  // Centers the maze and the outer wall
  servo1.write(125); 
  servo2.write(125);
}
  
void loop()
{
  turnLEDRed();
  startButtonState = LOW;
  Serial.println("Press to Start!");
  while (startButtonState == LOW)  // Waiting for the user to push start button to start the timer and the game. 
  {
    startButtonState = digitalRead(startButton);
  }
  turnLEDGreen();
  playPushedButtonTone();
  startTime = millis();  // After button is pushed the current millis() is recorded 

  endButtonState = LOW;
  while (endButtonState == LOW)  // Lets the user play the game until the user pushes the endButton
  {
    servoVal = analogRead(joyX);
    servoVal = map(servoVal, 0, 1023, 90, 160);
    servo1.write(servoVal);
  
    servoVal = analogRead(joyY);
    servoVal = map(servoVal, 0, 1023, 90, 160);
    servo2.write(servoVal);
    delay(10); 
    
    endButtonState = digitalRead(endButton);
  }
  playPushedButtonTone();
  turnLEDRed();
  noTone(SPEAKER);
  endTime = millis(); // records the time when the user has pushed the end button 
  giveScore(startTime, endTime); // scores the user out of 5 beeps how good they did time wise with the maze. 
}

void giveScore(unsigned long startTime, unsigned long endTime) 
{
  elapsedTime = (endTime - startTime)/1000; // gets elapsed time and converts it from milliseconds to seconds.
  int numBeeps = 0;
  
  Serial.print("Elapsed Time: ");
  Serial.println(elapsedTime);

  // Lower times yield better scores 
  if (elapsedTime <= 15) {
    numBeeps = 5;
  }
  else if (elapsedTime <= 20) {
    numBeeps = 4;
  }
  else if (elapsedTime <= 25) {
    numBeeps = 3;
  }
  else if (elapsedTime <= 30) {
    numBeeps = 2;
  }
  else if (elapsedTime <= 35 || elapsedTime > 35) {
    numBeeps = 1;
  }
  
  delay(250);

  Serial.print("You scored ");
  Serial.print(numBeeps);
  Serial.println(" out of 5");

  turnLEDBlue();
  for (byte i = 0; i < numBeeps; i++) 
  {
    tone(SPEAKER,NOTE_D5);
    delay(250);
    noTone(SPEAKER);
    delay(100);
  }
  noTone(SPEAKER);
}

void playPushedButtonTone() 
{
  tone(SPEAKER,NOTE_A5);
  delay(70);
  tone(SPEAKER,NOTE_B5);
  delay(70);
  tone(SPEAKER,NOTE_C5);
  delay(70);
  tone(SPEAKER,NOTE_B5);
  delay(70);
  tone(SPEAKER,NOTE_C5);
  delay(70);
  tone(SPEAKER,NOTE_D5);
  delay(70);
  tone(SPEAKER,NOTE_E5);
  delay(250);
  noTone(SPEAKER);
}

void turnLEDRed()
{
  analogWrite(redPin, 255);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
}

void turnLEDGreen()
{
  analogWrite(redPin, 0);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 0);
}

void turnLEDBlue()
{
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 255);
}
