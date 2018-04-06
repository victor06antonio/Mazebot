#include "robot.h"

void rbt_init()
{
  pinMode(BPHASE, OUTPUT);
  pinMode(APHASE, OUTPUT);
  pinMode(AEN, OUTPUT);
  pinMode(BEN, OUTPUT);
  pinMode(DRV_MODE, OUTPUT);
  pinMode(WALL_LEFT_EN, OUTPUT);
  pinMode(WALL_MID_EN, OUTPUT); 
  pinMode(WALL_RIGHT_EN, OUTPUT);
  pinMode(WALL_LEFT, INPUT);
  pinMode(WALL_MID, INPUT); 
  pinMode(WALL_RIGHT, INPUT);
  digitalWrite(WALL_LEFT_EN,LOW);
  digitalWrite(WALL_MID_EN,LOW);
  digitalWrite(WALL_RIGHT_EN,LOW);
    /*simplified drive mode*/
  digitalWrite(DRV_MODE, HIGH);
}

void rbt_move(direction_t new_dir, uint8_t speed)
{
  if(speed)
  {
    switch(new_dir){
      case LEFT:
        digitalWrite(BPHASE,MOTOR_FWD);
        digitalWrite(APHASE,MOTOR_FWD);
        analogWrite(AEN,speed);
        analogWrite(BEN,speed-speed/2);
        break;
      case RIGHT:
        digitalWrite(BPHASE,MOTOR_FWD);
        digitalWrite(APHASE,MOTOR_FWD);
        analogWrite(AEN,speed-speed/2);
        analogWrite(BEN,speed);
        break;
      case FWD:
        digitalWrite(BPHASE,MOTOR_FWD);
        digitalWrite(APHASE,MOTOR_FWD);
        analogWrite(AEN,speed);
        analogWrite(BEN,speed);
        break;
      case REV:
        digitalWrite(BPHASE,MOTOR_REV);
        digitalWrite(APHASE,MOTOR_REV);
        analogWrite(AEN,speed);
        analogWrite(BEN,speed);
        break;
      case HARD_LEFT:
        digitalWrite(BPHASE,MOTOR_FWD);
        digitalWrite(APHASE,MOTOR_FWD);
        analogWrite(AEN,speed);
        analogWrite(BEN,0);
        break;
      default:
        analogWrite(AEN,0);
        analogWrite(BEN,0);
        break;
    }
  }
  else
  {
    analogWrite(AEN,0);
    analogWrite(BEN,0);
  }
}

/*function takes 1ms to run*/
#define LOOP_ITER_CNT 2
void rbt_sns( uint16_t *line_left,
              uint16_t *line_mid,
              uint16_t *line_right,
              boolean  *wall_left,
              boolean  *wall_mid,
              boolean  *wall_right)
{
  *line_left=0;
  *line_mid=0;
  *line_right=0;
  uint16_t usec_timer=0;
  /*line sensor*/
  /*charge lines*/
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  digitalWrite(OUT1,HIGH);
  digitalWrite(OUT2,HIGH);
  digitalWrite(OUT3,HIGH);
  delayMicroseconds(3);
  /*set to Hi-Z to let cap discharge*/
  pinMode(OUT1, INPUT);
  pinMode(OUT2, INPUT);
  pinMode(OUT3, INPUT);
  /*enable first wall sensor*/
  digitalWrite(WALL_LEFT_EN,HIGH);
  while(1){
    /*each loop is about 2us at 48MHz*/
    usec_timer+=LOOP_ITER_CNT;
    /*increment counts for line sensors every us to track the decay of the capacitor*/
    if(digitalRead(OUT1) == 1)
    {
      (*line_left)+=LOOP_ITER_CNT;
    }
    if(digitalRead(OUT2) == 1)
    {
      (*line_mid)+=LOOP_ITER_CNT;
    }
    if(digitalRead(OUT3) == 1)
    {
      (*line_right)+=LOOP_ITER_CNT;
    }
    /*take turns reading wall sensors because they interfere with each other*/
    if(usec_timer == 300)
    {
      *wall_left = (digitalRead(WALL_LEFT) ? false:true);
      digitalWrite(WALL_LEFT_EN,LOW);
    }
    if(usec_timer == 400)
    {
      digitalWrite(WALL_MID_EN,HIGH);
    }
    if(usec_timer == 700)
    {
      *wall_mid = (digitalRead(WALL_MID) ? false:true);
      digitalWrite(WALL_MID_EN,LOW);
    }
    if(usec_timer == 700)
    {
      digitalWrite(WALL_RIGHT_EN,HIGH);
    }
    if(usec_timer>=1000)
    {
      *wall_right = (digitalRead(WALL_RIGHT) ? false:true);
      digitalWrite(WALL_MID_EN,LOW);
      return;
    }
  }
}
