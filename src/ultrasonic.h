#ifndef ULTRASONIC_H
#define ULTRASONIC_H

struct US_data {
  int dist_A;
  int dist_B;
};

US_data US_measure();

#endif