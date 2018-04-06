#include "robot.h"

void setup()
{
  Serial.begin(38400);
  Serial.println("Boot");
  rbt_init();  
  delay(5000);
  rbt_move(FWD,100);
}

uint16_t lleft,lmid,lright;
boolean wleft,wmid,wright;
uint16_t avoid_count=0;
void loop()                     
{
  rbt_sns(&lleft,&lmid,&lright,&wleft,&wmid,&wright);
  /*if the wall is sensed, go forward
  * the wall is sensed if the right sensor is on but the mid
  * sensor is off.
  */
  if(wright && !wmid)
  {
    rbt_move(FWD,100);
  }
  /*likely going towards the wall
  * not sure how close so turn as fast
  * as we can
  */
  if(wright && wmid)
  {
    rbt_move(HARD_LEFT,100);
  }
  /*going away from the wall
  * slowly turn back towards the wall
  */
  if(!wright && !wmid)
  {
    rbt_move(RIGHT,130);
  }
  /*likely at a corner or coming in at an angle to the wall*/
  if(!wright && wmid)
  {
    rbt_move(HARD_LEFT,100);
  }
}


