/*
 * Author: Alexis.m
 * realesed 05/02/2023
 * This code is used for servo gripper with 6 pwm for 7 servo motor (2 parallel)
 * To use this code on Arduino board, please make sure you're using a suitable power!
 */

#include <Servo.h>
#include <Arduino.h>
#include "scanHandler.h"

#define TEST 1 //si 1 alors test les mouvements

Servo moteur_socle;  // create servo object to control a servo until 12 for arduino Uno
Servo moteur_pince_rotate;  
Servo moteur_pince_locking;
Servo moteur_bras_inf;//there are 2 motor in parallel for this
Servo moteur_coude;
Servo moteur_bras_sup;

int16_t error=0;
unsigned long time;

typedef enum{START,SCAN,TAKE,GIVE} ROBOT;
ROBOT EC_ROBOT,EF_ROBOT;

int16_t robotStartTest(void);
int16_t robotGoHome(void);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  moteur_socle.attach(11);
  moteur_pince_rotate.attach(3);
  moteur_pince_locking.attach(A1);
  moteur_bras_inf.attach(9);
  moteur_coude.attach(10);
  moteur_bras_sup.attach(5);
}


void loop() {

  switch (EC_ROBOT){
    case START:
      Serial.println(F("robot START"));
      Serial.println(F("robot GO HOME"));

      if (robotGoHome()){EF_ROBOT =-1;break;/*error*/}
      Serial.println(F("robot POSE HOME"));

      #ifdef TEST
      Serial.println(F("test des mouvements...."));
      #endif
      EF_ROBOT=SCAN;
    break;
    case SCAN:
    break;
    case TAKE:
    break;
    case GIVE:
    break;
    default:
    Serial.println(F("ERROR"));
    EF_ROBOT = START;
    break;
  }
 EC_ROBOT=EF_ROBOT;
}

int16_t robotGoHome(void)
{
  int16_t posRobot;
  posRobot=90;
 
  moteur_socle.write(0);//ok
  moteur_pince_rotate.write(70);//ok
  moteur_pince_locking.write(80);
  moteur_bras_inf.write(0);
  moteur_coude.write(0);
  moteur_bras_sup.write(90); //ok
  return 0;
}



/*
  // put your main code here, to run repeatedly:
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
     moteur_socle.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    moteur_socle.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
*/