#ifndef SENSORS_H_
#define SENSORS_H_

int pm3901Init();
int VL53L1xInit();
void rpmInit();

//Make sure on what to use in this
void updateMotorRpm();
void updateSteeringAngle();
//probably wont be using this
// void updateWheelRpm(); 



#endif