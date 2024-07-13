#ifndef DEF_H_
#define DEF_H_

//status LED pin defination
#define comLed 22
#define modeLed 23
#define batLowLed 24
#define batOkLed 25 
#define rcLed 26  
#define gpsLed 27  
#define camLed 28 
#define lidarLed 29 
#define imuLed 30
#define spareLed 31
#define steeringInput 20

//servo pin defination
#define st_svr 5   //Steering Servo
#define esc 4      //Electronic Speed Controller/throttle
// #define g_svr 7    //Hi-LO gear servo
// #define fl_svr 9   //Front Differential Lock Servo
// #define rl_svr 11  //Rear Differential Lock Servo
// #define cm_svr 13  //Camera Tilt Servo

// Limits for the servo range
// TODO - need to redefine these values 

#define minSteering 1150 
#define maxSteering 1850 
#define minThrottle 1200
#define maxThrottle 1900


#define max_speed 70
#define brk_delay 500

//RPM sensor reading pin and interrupt definations
//Not sure what to do here, whether to use the encoder code or this
// #define PCINT_PIN_COUNT            6
// #define PCINT_RX_BITS              (1<<0),(1<<1),(1<<2),(1<<3),(1<<5),(1<<7)
// #define PCINT_RX_PORT              PORTK
// #define PCINT_RX_MASK              PCMSK2
// #define PCIR_PORT_BIT              (1<<2)
// #define RX_PC_INTERRUPT            PCINT2_vect
// #define RX_PCINT_PIN_PORT          PINK
// // #define wheelSensPins              {0, 1, 2, 3}
// #define motorSensPins              {4, 5}
// #define 

//Interrupts for the RC controller
#define PCINT_PIN_COUNT             4
#define PCINT_RX_BITS               (1<<0),(1<<1),(1<<2),(1<<3)
#define PCINT_RX_PORT               PORTK 
#define PCINT_RX_MASK               PCMSK2
#define PCINT_PORT_BIT              (1<<2)
#define RX_PC_INTERRUPT             PCINT2_vect
#define RX_PCINT_PIN_PORT           PINK


#define kp 2.35
#define ki 0.21
#define kd 0.32

#define pidTimer 20
#define rpmPubTimer 200
#define steeringPubTimer 200

#endif
