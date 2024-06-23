#ifndef HELPERS_H
#define HELPERS_H

#include "mbed.h"

// drivers
#include "pm2_drivers/PESBoardPinMap.h"
#include "pm2_drivers/DebounceIn.h"
#include "pm2_drivers/Servo.h"
#include "pm2_drivers/FastPWM/FastPWM.h"
#include "pm2_drivers/DCMotor.h"

/*gear ratio*/
const float gear_ratio_M3 = 100.0f;
/*motor constant [rpm/V] */
const float kn_M3 = 140.0f / 12.0f;  

const float ir_distance_min = 4.0f;
const float ir_distance_max = 30.0f;

const float FORWARD = 1;
const float BACKWARD = 0;

const uint8_t FRONT = 1;
const uint8_t BACK = 0;
const uint8_t UP = 1;
const uint8_t DOWN = 0;



float ir_sensor_compensation(float ir_distance_mV);
void init_motors(DCMotor* motor_M1,DCMotor* motor_M2,Servo* servo_D0,Servo* servo_D1,const float speed_multiplier,const float servo_D0_ang_min,const float servo_D0_ang_max,
const float servo_D1_ang_min,const float servo_D1_ang_max,float servo_D0_UP,float servo_D1_UP);
float get_ir_distance(AnalogIn* ir_analog_in);

uint8_t linear_drive(DCMotor* motor_M1,DCMotor* motor_M2,float Abs_position_in_revs,float direction,float* currentPos_M1,float* currentPos_M2,float M1_compensation,float M2_compensation);
uint8_t turn_90(DCMotor* motor_M1,DCMotor* motor_M2,float currentPos_M1,float currentPos_M2, float revs_to_turn_90);
//uint8_t move_bridge(Servo* servo_D0,Servo* servo_D1,uint8_t front_or_back,uint8_t direction);
void move_bridge(Servo* servo_D0,Servo* servo_D1,uint8_t front_or_back,uint8_t direction,float servo_input);

#endif