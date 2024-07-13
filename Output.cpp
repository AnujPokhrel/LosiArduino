#include "def.h"
#include "CarController.h"
#include "util.h"
#include "Output.h"
#include "Servo.h"

Servo steeringServo;
Servo ESC;

int prev_error1 = 0;
int prev_error2 = 0;

//steering servo, esc servo
int servo_values[2] = {0 , 0};

void initServos(){
    steeringServo.attach(st_svr);
    ESC.attach(esc);


    //need to figure this one out again
    servo_values[0] = 1500;
    servo_values[1] = 1500;

    steeringServo.writeMicroseconds(1200);
    ESC.writeMicroseconds(servo_values[1]);
    delay(500);
    steeringServo.writeMicroseconds(1800);
    delay(500);
    steeringServo.writeMicroseconds(1500);
    delay(300);
}


void writeServos() {
    steeringServo.writeMicroseconds(servo_values[0]);
    ESC.writeMicroseconds(servo_values[1]);
}


void failSafeActive() {
    steeringServo.writeMicroseconds(1500);
    ESC.writeMicroseconds(1850);
}

int throttlePID(int c_rpm, int set_speed){
    int esc_out = 0;
    float output = 0;

    int error = set_speed - c_rpm;
    prev_error1 = prev_error1 + error;
    prev_error2 = error;
    output = int(set_speed + kp * error + ki * (prev_error1 + error) + kd * (error - prev_error2));

    if (prev_error1 < -70){
        prev_error1 = -70;
    }

    if (prev_error1 > 180){
        prev_error2 = 180;
    }

    esc_out = (int)fmap(output, 0.0, 100.0, 0.0, 490.0);

    if (esc_out < 0){
        esc_out = 0
    }

    if (esc_out > 490){
        esc_out = 490;
    }

    return esc_out;
}


