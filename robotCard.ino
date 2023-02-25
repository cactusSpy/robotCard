/*
 * Author: Alexis.m
 * realesed 05/02/2023
 * This code is used for servo gripper with 6 pwm for 7 servo motor (2 parallel)
 * To use this code on Arduino board, please make sure you're using a suitable power!
 */

#include <Servo.h>
#include <Arduino.h>
#include "scanHandler.h"

#define DEMO   1 //si 1 alors test les mouvements aatention consommation d'energie élevé
#define FALSE  0
#define TRUE   1

Servo moteur_socle;  // create servo object to control a servo until 12 for arduino Uno
Servo moteur_pince_rotate;  
Servo moteur_pince_locking;
Servo moteur_bras_inf;//there are 2 motor in parallel for this
Servo moteur_coude;
Servo moteur_bras_sup;

int error=0;
unsigned long time;

typedef enum{START,SCAN,TAKE,GIVE} ROBOT;
ROBOT EC_ROBOT,EF_ROBOT;

int robotStartTest(void);
int robotGoHome(void);
int robotGoDeck(void);
int robotGoGive(void);
int robotDemo(void);
int robotMaintient(int);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  scanInit();//initialisation du capteur ultrasons

  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(3,OUTPUT);

}


void loop() {

  switch (EC_ROBOT){

    case START:
      Serial.println(F("robot START"));

      #ifdef DEMO
        robotMaintient(TRUE);/*Active l'auto maintient*/
        Serial.println(F("robot GO HOME"));     
        if (robotGoHome()){EF_ROBOT =-1;break;/*error*/}
        Serial.println(F("robot POSE HOME"));
    
        Serial.println(F("test des mouvements...."));//mini demo
        robotDemo();
      #else          
      robotMaintient(FALSE);/*Desactive l'auto maintient*/
      #endif

      EF_ROBOT=SCAN;
      Serial.println(F("Waiting for client"));
    break;

    case SCAN:
    if(scanPresence()){
      Serial.println(F("Client detecté!"));
      delay(1000);//permet d'eviter une colision avec la main du client
      EF_ROBOT = TAKE;
    }      
    break;

    case TAKE:
      Serial.println(F("Robot take a card"));
      robotGoDeck();
      EF_ROBOT = GIVE;
    break;
    case GIVE:
      Serial.println(F("Robot Give the card"));
      robotGoGive();
      EF_ROBOT = START; 

    break;
    default:
    Serial.println(F("ERROR"));
    EF_ROBOT = START;
    break;
  }
 EC_ROBOT=EF_ROBOT;
}

int robotGoHome(void)
{
  moteur_socle.write(0);
  moteur_pince_rotate.write(10);
  moteur_pince_locking.write(150);
  moteur_bras_inf.write(90);
  moteur_coude.write(0);
  moteur_bras_sup.write(180);
  return 0;
}

int robotGoDeck(void)
{
   moteur_socle.write(0);
   moteur_pince_rotate.write(10);
   moteur_pince_locking.write(150);
   moteur_bras_inf.write(90);
   moteur_coude.write(0);
   moteur_bras_sup.write(180);
   return 0;
}

int robotGoGive(void)
{
   moteur_socle.write(0);
   moteur_pince_rotate.write(10);
   moteur_pince_locking.write(150);
   moteur_bras_inf.write(90);
   moteur_coude.write(0);
   moteur_bras_sup.write(180);
   return 0;  
}

int robotDemo(void)
{
  return 0;
}

int robotMaintient(int state)
{
  if (!state)
  {
      moteur_socle.detach();
      moteur_pince_rotate.detach();
      moteur_pince_locking.detach();
      moteur_bras_inf.detach();
      moteur_coude.detach();
      moteur_bras_sup.detach();   
  }
  else {
      moteur_socle.attach(11);
      moteur_pince_rotate.attach(10);
      moteur_pince_locking.attach(9);
      moteur_bras_inf.attach(6);
      moteur_coude.attach(5);
      moteur_bras_sup.attach(3);       
  }

}