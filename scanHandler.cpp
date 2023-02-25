#include "scanHandler.h"
#include <Arduino.h>

#define TRIGGER_PIN 13// Broche TRIGGER
#define ECHO_PIN 12 // Broche ECHO

#define DISPLAY_MESURE 0

/* Constantes pour le timeout */
#define  MEASURE_TIMEOUT    25000UL // 25ms = ~8m à 340m/s
#define  SOUND_SPEED        340.0 / 1000/* Vitesse du son dans l'air en mm/us */
#define  distanceMin_mm     300
#define  distanceMax_mm     400
#define  delayDetection_ms  600
#define  delayMesure_ms     200

static unsigned long currentTime;
static unsigned long previousTimeDetection;
static unsigned long previousTimeMesure;

void scanInit()
{
  /* Initialise les broches */
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIGGER_PIN,0); // La broche TRIGGER doit être à 0 au repos
}

int scanPresence(void) {
  
  currentTime = millis();
  if ((currentTime - previousTimeDetection) > delayMesure_ms) /* Délai d'attente pour éviter de mesurer et d'afficher trop de résultats à la seconde */
  {
    previousTimeDetection = currentTime;
    /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
    long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);

    /* 3. Calcul la distance à partir du temps mesuré */
    float distance_mm = measure / 2.0 * SOUND_SPEED;

    /* Affiche les résultats en mm, cm et m */
    #ifdef DISPLAY_MESURE
    Serial.print(F("Distance: "));
    Serial.print(distance_mm);
    Serial.print(F("mm ("));
    Serial.print(distance_mm / 10.0, 2);
    Serial.print(F("cm, "));
    Serial.print(distance_mm / 1000.0, 2);
    Serial.println(F("m)"));
    #endif

    if ((distance_mm > distanceMin_mm) && (distance_mm < distanceMax_mm))
    {
      currentTime = millis();//Ce if millis pourrait être modifié par un compteur pour lancer l'action de bras de robot au bout de x mesures dans l'intervale de distance 
      if ((currentTime - previousTimeMesure) > delayDetection_ms)
      {
        previousTimeMesure = currentTime;     
        return 1;//action bras de robot
      }
    }
    else currentTime = 0;
  }
  return 0;
}