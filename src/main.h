#ifndef MAIN_H
#define MAIN_H

#include "mbed.h"
#include "Helpers/helpers.h"


/*maximum voltage of battery packs, adjust this to
 *6.0f V if you only use one battery pack
*/ 
const float voltage_max = 12.0f;

/*gear ratio*/ 
const float gear_ratio_M1 = 100.0f;
const float gear_ratio_M2 = 100.0f;

/*[rpm/V] */
const float kn_M1 = 180.0f / 12.0f; 
const float kn_M2 = 180.0f / 12.0f;  

const float speed_multiplier = 0.5f;

/*carefull, these values might differ from servo to servo*/

const float servo_D0_ang_min = 0.0350f; 
const float servo_D0_ang_max = 0.120f;
const float servo_D1_ang_min = 0.0350f; 
const float servo_D1_ang_max = 0.120f;

const float servo_D0_UP = 0.90;   // works fine
const float servo_D0_DOWN = 0.1;  // works fine.
const float servo_D1_UP = 0.900;
const float servo_D1_DOWN = 0.000;

const float RAISED = 1;
const float LOWERED = 0;

const float ABS_REVS_T0_90 = 0.6f;
const float REVS_TO_TURN_90 = 0.55f;
const float REVERSE_REVS = 0.2;
const float REVS_TO_CLEAR_BRIDGE_T1 = 1.2;
const float CROSS_BRIDGE_REVS = 3.8;
const float REVERSE_REVS_T2 = 0.8;
const float REVS_TO_FINISH = 2.0;

const float REVS_TO_TURN_180 = 1.15f;
const float REVS_TO_PICK_BRIDGE = 0.2f;

//const float ABS_REVS_T0_TABLE2_EDGE =8.0f;
//const float ABS_REVS_T0_CROSS_BRIDGE = 8.0f;
//const float ABS_REVERSE_AT_TABLE2_EDGE = 7.0f;
//const float ABS_REVS_TO_CLEAR_TABLE2_EDGE = 11.0f;


typedef enum{
BOOTING,
MOVE_TO_90_POS,
TURN_90,
MOVE_TO_TABLE1_EDGE,
GET_CLOSE_TO_TABLE_EDGE,
REVERSE_TO_LOWER_BRIDGE_SAFELY,
LOWER_BRIDGE,
REVERSE_FROM_BRIDGE,
CROSS_BRIDGE,
REVERSE_TO_BRIDGE,
LIFT_BRIDGE,
MOVE_FROM_TABLE2_EDGE,
SLEEP,
ROTATE_180,
LOWER_FRONT_FORK,
DRIVE_INTO_BRIDGE,
LIFT_BRIDGE_FRONT,
REVERSE_TO_FINISH


}Robot_States;

#endif





