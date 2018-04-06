#ifndef _ROBOT_H
#define _ROBOT_H

#include "Arduino.h"

/*DRV8835*/
const int BPHASE = 5;
const int APHASE = 3;
const int AEN = 4;
const int BEN = 6;
const int DRV_MODE = 2;
#define MOTOR_REV  LOW
#define MOTOR_FWD  HIGH

/*reflection sensor interface*/
const int OUT1 = 33;
const int OUT2 = 32;
const int OUT3 = 31;

/*wall sensor interface*/
const int WALL_LEFT_EN = 15;
const int WALL_LEFT = 14;
const int WALL_RIGHT_EN = 19;
const int WALL_RIGHT = 18;
const int WALL_MID_EN = 17;
const int WALL_MID = 16;

/*robot interface*/
typedef enum{
  LEFT,
  RIGHT,
  FWD,
  REV,
  BRAKE,
  HARD_LEFT,
}direction_t;

void rbt_move(direction_t new_dir, uint8_t speed);
void rbt_sns( uint16_t *line_left,
              uint16_t *line_mid,
              uint16_t *line_right,
              boolean  *wall_left,
              boolean  *wall_mid,
              boolean  *wall_right);
void rbt_init();

#endif /*_ROBOT_H*/
