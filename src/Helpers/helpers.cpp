#include "helpers.h"


/* ir distance sensor*/

/*define a variable to store measurement (in mV)*/
float ir_distance_mV = 0.0f; 
float ir_distance_cm = 0.0f;
float ir_distance_cm_candidate = 0.0f;

float ir_sensor_compensation(float ir_distance_mV)
{
    /* insert values that you got from the MATLAB file*/
    static const float a = 1.517e+04f;
    static const float b = 102.1f;

    /* avoid division by zero by adding a small value to the denominator*/
    if (ir_distance_mV + b == 0.0f)
        ir_distance_mV -= 0.001f;

    return a / (ir_distance_mV + b);
}

void init_motors(DCMotor* motor_M1,DCMotor* motor_M2,Servo* servo_D0,Servo* servo_D1,const float speed_multiplier,
const float servo_D0_ang_min,const float servo_D0_ang_max,const float servo_D1_ang_min,const float servo_D1_ang_max,float servo_D0_UP,float servo_D1_UP)
{
    
    /* enable the motion planner for smooth movement*/
    motor_M1->enableMotionPlanner(true);
    motor_M2->enableMotionPlanner(true);

    /* limit max. velocity to half physical possible velocity*/
    motor_M1->setMaxVelocity(motor_M1->getMaxPhysicalVelocity() * speed_multiplier);
    motor_M2->setMaxVelocity(motor_M2->getMaxPhysicalVelocity() * speed_multiplier);

    motor_M1->setMaxAcceleration(motor_M1->getMaxAcceleration()*0.5f);
    motor_M2->setMaxAcceleration(motor_M2->getMaxAcceleration()*0.5f);

    servo_D0->calibratePulseMinMax(servo_D0_ang_min, servo_D0_ang_max);
    servo_D1->calibratePulseMinMax(servo_D1_ang_min, servo_D1_ang_max);

    if (!servo_D0->isEnabled()){
        servo_D0->enable();
        servo_D0->setNormalisedPulseWidth(servo_D0_UP);
    }
    if (!servo_D1->isEnabled()){
        servo_D1->enable();
        servo_D1->setNormalisedPulseWidth(servo_D1_UP);
    }

}
float get_ir_distance(AnalogIn* ir_analog_in)
{
     ir_distance_mV = 1.0e3f * ir_analog_in->read() * 3.3f;
     ir_distance_cm_candidate = ir_sensor_compensation(ir_distance_mV);

     if (ir_distance_cm_candidate > 0.0f) {
         ir_distance_cm = ir_distance_cm_candidate;
      }

    return ir_distance_cm;  
}

uint8_t linear_drive(DCMotor* motor_M1,DCMotor* motor_M2,float Abs_position_in_revs,float direction,float* currentPos_M1,float* currentPos_M2,float M1_compensation,float M2_compensation)
{

    motor_M1->setRotation(Abs_position_in_revs + M1_compensation);
    motor_M2->setRotation(Abs_position_in_revs + M2_compensation);
    
    if(direction == FORWARD)
    {
        
        if((motor_M1->getRotation() >= (Abs_position_in_revs+M1_compensation)) && (motor_M2->getRotation() >= (Abs_position_in_revs+M2_compensation))){
            
            *currentPos_M1 = motor_M1->getRotation();
            *currentPos_M2 = motor_M2->getRotation();
            return 1;
        }
       
    }
    else if(direction == BACKWARD)
    {
       
        if((motor_M1->getRotation() <= (Abs_position_in_revs+M1_compensation)) && (motor_M2->getRotation() <= (Abs_position_in_revs+M2_compensation))){
            
            *currentPos_M1 = motor_M1->getRotation();
            *currentPos_M2 = motor_M2->getRotation();

            return 1;
        }
        
    }

    return 0;
}
uint8_t turn_90(DCMotor* motor_M1,DCMotor* motor_M2,float currentPos_M1,float currentPos_M2, float revs_to_turn_90)
{
    motor_M1->setRotation(currentPos_M1 + revs_to_turn_90);
    motor_M2->setRotation(currentPos_M2 - revs_to_turn_90);

    if((motor_M1->getRotation()>=(currentPos_M1 + revs_to_turn_90)) && (motor_M2->getRotation()<=(currentPos_M2 - revs_to_turn_90))){

        return 1;
    }

    return 0;
}
// uint8_t move_bridge(Servo* servo_D0,Servo* servo_D1,uint8_t front_or_back,uint8_t direction)
// {
//     if(front_or_back == FRONT)
//     {
//         if(direction == DOWN)
//         {
            
//             if(servo_D0_DOWN > servo_D0_UP)
//             {
//                 float i = 0;
//                 for(i =servo_D0_UP;i <= servo_D0_DOWN; i = i + 0.005){

//                     servo_D0->setNormalisedPulseWidth(i);
//                     ThisThread::sleep_for(20ms);
//                 }
                
//             }
//             else if(servo_D0_UP > servo_D0_DOWN)
//             {
//                 float i = 0;
//                 for(i = servo_D0_UP;i>= servo_D0_DOWN;i=1-0.005){
//                     servo_D0->setNormalisedPulseWidth(i);
//                     ThisThread::sleep_for(20ms);
//                 }
//             }
            
//             return 1;
//         }

//         if(direction == UP)
//         {
//             if(servo_D0_DOWN > servo_D0_UP)
//             {
//                 float i = 0;
//                 for(i =servo_D0_DOWN;i >= servo_D0_UP; i = i - 0.005){

//                     servo_D0->setNormalisedPulseWidth(i);
//                     ThisThread::sleep_for(20ms);
//                 }
                
//             }
//             else if(servo_D0_UP > servo_D0_DOWN)
//             {
//                 float i = 0;
//                 for(i = servo_D0_DOWN;i<= servo_D0_UP;i=1+0.005){
//                     servo_D0->setNormalisedPulseWidth(i);
//                     ThisThread::sleep_for(20ms);
//                 }
//             }
            
//             return 1;
//         }
//     }

//     if(front_or_back == BACK)
//     {
//        if(direction == DOWN)
//         {
            
//             if(servo_D1_DOWN > servo_D1_UP)
//             {
//                 float i = 0;
//                 for(i =servo_D1_UP;i <= servo_D1_DOWN; i = i + 0.005){

//                     servo_D1->setNormalisedPulseWidth(i);
//                     ThisThread::sleep_for(20ms);
//                 }
                
//             }
//             else if(servo_D1_UP > servo_D1_DOWN)
//             {
//                 float i = 0;
//                 for(i = servo_D1_UP;i>= servo_D1_DOWN;i=1-0.005){
//                     servo_D1->setNormalisedPulseWidth(i);
//                     ThisThread::sleep_for(20ms);
//                 }
//             }
            
//             return 1;
//         }

//         if(direction == UP)
//         {
//             if(servo_D1_DOWN > servo_D1_UP)
//             {
//                 float i = 0;
//                 for(i =servo_D1_DOWN;i >= servo_D1_UP; i = i - 0.005){

//                     servo_D1->setNormalisedPulseWidth(i);
//                     ThisThread::sleep_for(20ms);
//                 }
                
//             }
//             else if(servo_D1_UP > servo_D1_DOWN)
//             {
//                 float i = 0;
//                 for(i = servo_D1_DOWN;i<= servo_D1_UP;i=1+0.005){
//                     servo_D1->setNormalisedPulseWidth(i);
//                     ThisThread::sleep_for(20ms);
//                 }
//             }
            
//             return 1;
//         }
//     }
//     return 0;
// }

void move_bridge(Servo* servo_D0,Servo* servo_D1,uint8_t front_or_back,uint8_t direction,float servo_input)
{
    if(front_or_back == FRONT)
    {
        if(direction == DOWN)
        {
            servo_D0->setNormalisedPulseWidth(servo_input);
            
        }
        else if(direction == UP)
        {
            servo_D0->setNormalisedPulseWidth(servo_input);
            
        }     
           // return 1;
    }  


    if(front_or_back == BACK)
    {
        if(direction == DOWN)
        {
            servo_D1->setNormalisedPulseWidth(servo_input);
            
        }
        else if(direction == UP)
        {
            servo_D1->setNormalisedPulseWidth(servo_input);
          
        }    
            //return 1;
    }   
}