#include "main.h"
#include <cstdio>

/*
 *this variable will be toggled via the user button (blue button) and
 * decides whether to execute the main task or not
*/
bool do_execute_main_task = false; 

/*
 * this variable is used to reset certain variables and objects and
 * shows how you can run a code segment only once
*/
bool do_reset_all_once = false;    

/*create DebounceIn object to evaluate the user button
 *falling and rising edge
*/

DebounceIn user_button(USER_BUTTON); 
/* custom function which is getting executed when user
 * button gets pressed, definition below
*/  

void toggle_do_execute_main_fcn();   
/**/
DigitalOut led1(PB_9);


DigitalOut enable_motors(PB_ENABLE_DCMOTORS);
DCMotor motor_M1(PB_PWM_M1, PB_ENC_A_M1, PB_ENC_B_M1, gear_ratio_M1, kn_M1, voltage_max);
DCMotor motor_M2(PB_PWM_M2, PB_ENC_A_M2, PB_ENC_B_M2, gear_ratio_M2, kn_M2, voltage_max);

/*create FastPWM object to command motor M1*/
FastPWM pwm_M1(PB_PWM_M1); 
/*create FastPWM object to command motor M2*/
FastPWM pwm_M2(PB_PWM_M2); 

Servo servo_D0(PB_D0);
Servo servo_D1(PB_D1);   /*@TODO CONFIRM PIN OF SECOND SERVO ON PES BOARD*/

/*create AnalogIn object to read in the infrared distance sensor
 *0...3.3V are mapped to 0...1
*/ 
AnalogIn ir_analog_in(PC_2); 

/* Store Motor Pos before we initiate 90 turn */
float currentPos_M1;
float currentPos_M2;

uint8_t reached_90_pos = 0;
uint8_t turned_90 = 0;
uint8_t reached_t1_edge = 0;
uint8_t moved_close_to_edge = 0;
uint8_t  reversed_to_safe = 0;
uint8_t bridge_lowered = 0;
uint8_t reversed_t1_edge = 0;
uint8_t crossed_bridge = 0;
uint8_t reversed_t2_edge = 0;
uint8_t bridge_raised = 0;
uint8_t task_finished = 0;

uint8_t servo_D0_state  = RAISED;
uint8_t servo_D1_state  = RAISED;

float servo_rate = 0.005f;
float servo_input1 = 0.000f;
float servo_input2 = 0.000f;

/* Approach revs per second */
float Approach_bridge_speed = 0.05f; 
float Abs_revs_to_t1_edge = 0.0f;


float ABS_REVERSE_AT_TABLE1_EDGE =0.0f ;
float ABS_REVS_T0_CROSS_BRIDGE = 0.0f;
float ABS_REVERSE_AT_TABLE2_EDGE = 0.0f;
float ABS_REVS_TO_CLEAR_TABLE2_EDGE = 0.0f;
float REVERSE_TO_CLEAR_BRIDGE = 0.0f;

Robot_States TRENCHNATOR_STATE = BOOTING;
int main(){

    /*attach button fall function address to user button object,
     * button has a pull-up resistor
    */ 
    user_button.fall(&toggle_do_execute_main_fcn);

    /* */
    led1 = 0;
    /* define main task period time in ms e.g. 20 ms, there for
     * the main task will run 50 times per second
    */ 
    const int main_task_period_ms = 50;
    /* create Timer object which we use to run the main task
     * every main_task_period_ms
    */  
    Timer main_task_timer; 
    /* start timer */
    main_task_timer.start();
    /*Initialize all motors*/

    const int loops_per_seconds = static_cast<int>(ceilf(1.0f / (0.001f * static_cast<float>(main_task_period_ms))));

    init_motors(&motor_M1,&motor_M2,&servo_D0,&servo_D1,speed_multiplier,
    servo_D0_ang_min,servo_D0_ang_max,servo_D1_ang_min,servo_D1_ang_max,servo_D0_UP,servo_D1_UP);
    /*setting this once would actually be enough*/
    //enable_motors = 1; 


    

    while(true){

        main_task_timer.reset();

        if (do_execute_main_task){


            /* FRONT SERVO */
            move_bridge(&servo_D0,&servo_D1,FRONT,DOWN,servo_input1);

            if(std::fabs(servo_input1 - servo_D0_DOWN) < 0.002f)
                {
                   // printf("Bridge Lowered\n");
                    printf("servo_input: %f: \n",servo_input1);
                    //TRENCHNATOR_STATE = REVERSE_FROM_BRIDGE;
                }

                // if(servo_input1 == servo_D0_DOWN)
                // {
                //     printf("Bridge Lowered\n");
                //     TRENCHNATOR_STATE = REVERSE_FROM_BRIDGE;
                // }
                     
                if(servo_input1 > servo_D0_DOWN){ 
                    servo_input1 = servo_input1 - servo_rate;
                }

                if(servo_input1 < servo_D0_DOWN){ 
                    servo_input1 = servo_input1 + servo_rate;
                }
                    
                 //printf("servo_input: %f , servo0_DOWN: %f\n",servo_input1,servo_D0_DOWN);


             /* BACK SERVO */


        
            // /* toggling the user led*/
            // led1 = !led1;

            // switch(TRENCHNATOR_STATE) {
            //     case BOOTING:{
            //         printf("BOOTING\n");
            //         enable_motors = 1;
            //         TRENCHNATOR_STATE = MOVE_TO_90_POS;
                     
            //         break;
            //     }
            //     case MOVE_TO_90_POS:{

            //         printf("MOVE_TO_90\n");
            //         reached_90_pos = linear_drive(&motor_M1,&motor_M2,ABS_REVS_T0_90,FORWARD,&currentPos_M1,&currentPos_M2,0.0f,0.0f,0.5f);
                    
            //         if(reached_90_pos){
            //             reached_90_pos = 0;
            //             TRENCHNATOR_STATE = TURN_90;
                
            //         }
            //         break;
            //     }
            //     case TURN_90:{
            //         printf("TURN 90\n");
            //         turned_90 =  turn_90(&motor_M1,&motor_M2,currentPos_M1,currentPos_M2,REVS_TO_TURN_90);

            //         if(turned_90){
                        
            //             turned_90 = 0;
            //             Abs_revs_to_t1_edge = currentPos_M1 +REVS_TO_TURN_90;
            //             TRENCHNATOR_STATE = GET_CLOSE_TO_TABLE_EDGE;

            //         }

            //         break;
            //     }
                
            //     case GET_CLOSE_TO_TABLE_EDGE:{

            //         float threshold = 15.0f;
            //         float ir_distance_cm = get_ir_distance(&ir_analog_in);
            //         printf("Distance: %f\n",ir_distance_cm);

            //         if(ir_distance_cm < threshold){
            //             Abs_revs_to_t1_edge = Abs_revs_to_t1_edge + Approach_bridge_speed;
            //             moved_close_to_edge = linear_drive(&motor_M1,&motor_M2,Abs_revs_to_t1_edge,FORWARD,&currentPos_M1,&currentPos_M2,REVS_TO_TURN_90,-REVS_TO_TURN_90,0.5f);

            //         }
            //         if(ir_distance_cm > threshold){

            //             servo_input1 = servo_D0_UP;
            //             printf("Pos_at tabl1 edge: %f\n",Abs_revs_to_t1_edge );
            //             TRENCHNATOR_STATE = REVERSE_TO_LOWER_BRIDGE_SAFELY;
            //         }

            //         break;
            //     }
            //     case REVERSE_TO_LOWER_BRIDGE_SAFELY:{

            //         ABS_REVERSE_AT_TABLE1_EDGE = Abs_revs_to_t1_edge - REVERSE_REVS;

            //         printf("Reverse pos tabl1 edge: %f\n",  ABS_REVERSE_AT_TABLE1_EDGE);
            //         reversed_to_safe = linear_drive(&motor_M1,&motor_M2,ABS_REVERSE_AT_TABLE1_EDGE,BACKWARD,&currentPos_M1,&currentPos_M2,REVS_TO_TURN_90,-REVS_TO_TURN_90,0.5f);

            //         if(reversed_to_safe){
            //             TRENCHNATOR_STATE = LOWER_BRIDGE;
            //         }
                    
            //     }
            //     case LOWER_BRIDGE:{

            //         move_bridge(&servo_D0,&servo_D1,FRONT,DOWN,servo_input1);

            //         if(std::fabs(servo_input1 - servo_D0_DOWN) < 0.002f)
            //         {
            //             printf("Bridge Lowered\n");
            //             TRENCHNATOR_STATE = REVERSE_FROM_BRIDGE;
            //         }

            //         // if(servo_input1 == servo_D0_DOWN)
            //         // {
            //         //     printf("Bridge Lowered\n");
            //         //     TRENCHNATOR_STATE = REVERSE_FROM_BRIDGE;
            //         // }
                     
            //         if(servo_input1 > servo_D0_DOWN){ 
            //             servo_input1 = servo_input1 - servo_rate;
            //         }

            //         if(servo_input1 < servo_D0_DOWN){ 
            //             servo_input1 = servo_input1 + servo_rate;
            //         }
                    
            //         printf("servo_input: %f , servo0_DOWN: %f\n",servo_input1,servo_D0_DOWN);

                    
                    
            //         break;
            //     }
            //     case REVERSE_FROM_BRIDGE:{
                    
            //         //printf("Reversing\n");  

            //         REVERSE_TO_CLEAR_BRIDGE =  ABS_REVERSE_AT_TABLE1_EDGE - REVS_TO_CLEAR_BRIDGE_T1 ;
            //         reversed_t1_edge = linear_drive(&motor_M1,&motor_M2, REVERSE_TO_CLEAR_BRIDGE,BACKWARD,&currentPos_M1,&currentPos_M2,REVS_TO_TURN_90,-REVS_TO_TURN_90,0.25f);

            //         if(reversed_t1_edge){

            //             move_bridge(&servo_D0,&servo_D1,FRONT,UP,servo_input1);

            //             if(servo_input1 > servo_D0_UP){ 

            //                 servo_input1 = servo_input1 - servo_rate;
            //             }

            //             if(servo_input1 < servo_D0_UP){ 
            //                 servo_input1 = servo_input1 + servo_rate;
            //             }

            //             if(std::fabs(servo_input1 - servo_D0_UP) < 0.002f)
            //             {
            //                 TRENCHNATOR_STATE = CROSS_BRIDGE;
            //             }
                        
                        
            //         }

            //         break;
            //     }
            //     case CROSS_BRIDGE:{

            //         ABS_REVS_T0_CROSS_BRIDGE = currentPos_M1 + CROSS_BRIDGE_REVS;
            //         crossed_bridge = linear_drive(&motor_M1,&motor_M2,ABS_REVS_T0_CROSS_BRIDGE,FORWARD,&currentPos_M1,&currentPos_M2,REVS_TO_TURN_90,-REVS_TO_TURN_90,0.3f);

            //         if(crossed_bridge){

            //             servo_input2 = servo_D1_UP;
            //             TRENCHNATOR_STATE = REVERSE_TO_BRIDGE;
            //         }
                 
            //         break;
            //     }
            //     case REVERSE_TO_BRIDGE:{

            //         move_bridge(&servo_D0,&servo_D1,BACK,DOWN,servo_input2);

            //         if(std::fabs(servo_input2 - servo_D1_DOWN) < 0.002f){

            //             ABS_REVERSE_AT_TABLE2_EDGE = ABS_REVS_T0_CROSS_BRIDGE - REVERSE_REVS_T2;
            //             reversed_t2_edge = linear_drive(&motor_M1,&motor_M2,ABS_REVERSE_AT_TABLE2_EDGE,BACKWARD,&currentPos_M1,&currentPos_M2,REVS_TO_TURN_90,-REVS_TO_TURN_90,0.20f);
                        
            //             if(reversed_t2_edge){
            //                 TRENCHNATOR_STATE = LIFT_BRIDGE;
            //             }
            //         }

            //         if(servo_input2 > servo_D1_DOWN){ 
            //             servo_input2 = servo_input2 - servo_rate;
            //         }

            //         if(servo_input2 < servo_D1_DOWN){ 
            //             servo_input2 = servo_input2 + servo_rate;
            //         }

                    

            //         break;
            //     }
            //     case LIFT_BRIDGE:{

            //         move_bridge(&servo_D0,&servo_D1,BACK,UP,servo_input2);

            //         if(std::fabs(servo_input2 - servo_D1_UP) < 0.002f)
            //         {
            //             TRENCHNATOR_STATE = MOVE_FROM_TABLE2_EDGE;
            //         }

            //         if(servo_input2 > servo_D1_UP){ 
            //             servo_input2 = servo_input2 - servo_rate;
            //         }

            //         if(servo_input2 < servo_D1_UP){ 
            //             servo_input2 = servo_input2 + servo_rate;
            //         }
        
            //         break;
            //     }
            //     case MOVE_FROM_TABLE2_EDGE:{
                    
            //         ABS_REVS_TO_CLEAR_TABLE2_EDGE = ABS_REVERSE_AT_TABLE2_EDGE + REVS_TO_FINISH;
            //         task_finished = linear_drive(&motor_M1,&motor_M2,ABS_REVS_TO_CLEAR_TABLE2_EDGE,FORWARD,&currentPos_M1,&currentPos_M2,REVS_TO_TURN_90,-REVS_TO_TURN_90,0.5f);

            //         if(task_finished){
            //             TRENCHNATOR_STATE = SLEEP;
            //         }

            //         break;
            //     }
            //     case SLEEP:{

            //         toggle_do_execute_main_fcn();

            //         break;
            //     }
            //     default:

            //         break;
        
            // }

        } 
        else{
            if (do_reset_all_once){

                do_reset_all_once = false;

                /*reset variables and objects*/

                led1 = 0;
                enable_motors = 0;
                //ir_distance_cm = 0.0f;
                TRENCHNATOR_STATE = BOOTING;
                 

            }
        }
        

        /* read timer and make the main thread sleep for the remaining time span (non blocking)*/
        int main_task_elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(main_task_timer.elapsed_time()).count();
        thread_sleep_for(main_task_period_ms - main_task_elapsed_time_ms);
    
        
    }
}

void toggle_do_execute_main_fcn()
{
    /*toggle do_execute_main_task if the button was pressed*/
    do_execute_main_task = !do_execute_main_task;
    /* set do_reset_all_once to true if do_execute_main_task changed from false to true*/
    if (do_execute_main_task)
        do_reset_all_once = true;
}