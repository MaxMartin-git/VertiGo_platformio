#include "NewPing.h" //functions for ultrasonic
#include "../include/config.h"
#include "ultrasonic.h"

// Pin-Zuweisungen
#define trigPin_AR 8
#define echoPin_AR 8
#define trigPin_BR 7
#define echoPin_BR 7
#define max_distance 200 // Maximum distance to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm

//hier muss später Umschaltung rechts/links Messung -> Pins erfolgen
NewPing sonar_A(trigPin_AR, echoPin_AR, max_distance); // NewPing setup of pins and maximum distance.
NewPing sonar_B(trigPin_BR, echoPin_BR, max_distance);

US_data US_measure() {
  US_data d;
  //29ms should be the shortest delay between pings// more infos US: https://www.makerguides.com/de/hc-sr04-arduino-tutorial-de/

  unsigned int duration_A = sonar_A.ping(); //Ultraschall-Zeit hin und zurück in Mikrosekunden
  unsigned int duration_B = sonar_B.ping();
  
  d.dist_A = duration_A * 0.17; //[mm] //0.017 = v_Schall [mm/mikrosekunden] / 2
  d.dist_B = duration_B * 0.17;

  Serial.print("value A = ");
  Serial.print(d.dist_A); // Distance will be 0 when out of set max range.
  Serial.print(" mm, value B = ");
  Serial.print(d.dist_B);
  Serial.println(" mm");

  return d;
}
