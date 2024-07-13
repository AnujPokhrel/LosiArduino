#include "def.h"
#include "Sensors.h"
#include "Output.h"
#include "CarController.h"
#include "Arduino.h"
#include "util.h"
#define USB_USBCON

ros::NodeHandle nodeHandle;

unsigned long pidtask = 0;
unsigned long rpmtask = 0;
unsigned long steeringtask = 0;
unsigned long flowtask = 0;
unsigned long last_msg_time = 0;

float Throttle_cmd;
flaot smt_Throttle;

bool flowErr1 = 0;
bool flowErr2 = 0;

void driveCallback( const std_msgs::Float32MultiArray& control_msg ){
  //timestamp the  last ros message
  last_msg_time = millis();

  //Handle for steering command
  //Map steering command to servo output
  float steer_cmd = fmap(control_msg.data[0], -1.0, 1.0, minSteering, maxSteering);
  if (steer_cmd < minSteering) {
    steer_cmd = minSteering;
  }
  if (steer_cmd > maxSteering) {
    steer_cmd = maxSteering ;
  }
  //write Servo output for steering
  servo_values[0] = steer_cmd;

  //Handle for throttle command
  if ((control_msg.data[1] == 0)){
    Throttle_cmd = 0;
    smt_Throttle = 0;
  }
  // ESC forward continue
  if ((control_msg.data[1] >= 0) && (prev_dir == 0)){
    Throttle_cmd = control_msg.data[1];
    smt_Throttle = smt_Throttle + 0.2 * (Throttle_cmd - smt_Throttle);
  }
  
  //ESC reverse continue 
  if ((control_msg.data[1] < 0 ) && (prev_dir == 1)){
    Throttle_cmd = control_msg.data[1];
    smt_Throttle = smt_Throttle + 0.2 * (Throttle_cmd - smt_Throttle);
  }

  //From forward to rev
  if ((control_msg.data[1] < 0 ) && (prev_dir == 0)){
    Throttle_cmd = fw_to_rev(control_msg.data[1]);
    smt_Throttle = Throttle_cmd;
  }

  //From rev to forward
  if ((control_msg.data[1] > 0 ) && (prev_dir == 1)){ 
    Throttle_cmd = rev_to_fw();
    smt_Throttle = Throttle_cmd;
  }
}


ros::Subcriber<std_msgs::Float32MultiArray> driveSubscriber("/cmd_vel1", &driveCallback);

std_msgs::Int16MultiArray rpmVal;
ros:publisher rpmVal_data("rpmVal_data", &rpmVal);

//not sure if we need this or we can piggy back on rpm val, we don't have much rpm vals
std_msgs::Int16MultiArray steeringVal;
ros:publisher strVal_data("steeringVal_Data", &steeringVal);

void setup(){
  Serial.begin(57600);

  //ROS initialization
  nodeHandle.initNode();

  //Ros Subscribers
  nodeHandle.subscribe(driveSubscriber);
  
  //ROS publishers
  nodeHandle.advertise(rpmVal_data);
  // nodeHandle.advertise(steering_angle);
  // nodeHandle.advertise(rc_values);

  rcInit();
  initServos();
  //TODO
  encoderInit();
  steeringEncInit(); 

  Serial.println("Initialization Complete ....");
}

void loop(){
  unsigned long curr_millis = millis();

  int velocity;
  if((millis() - last_msg_time) > 1000){
    failSafeActive();
    pidtask = curr_millis;
  }
  else{
    if(curr_millis - pidtask >= pidTimer){
      //will have to define this
      updateMotorRpm();
      velocity = (int)fmap(abs(smt_Throttle), 0, 1, 0, max_speed);
      //not sure how to finish this and its purpose
    }
  }

  if (millis() - rpmtask >= rpmPubTimer){
    rpmVal.data_length = 1;
    int16_t r_packet = 0;
    //publishing rpm task //todo

  }
  //publishing for the steering 
  if(millis() - steeringtask >= steeringPubTimer){
    //publishing for steerign task //todo
  }
}