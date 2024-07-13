#include "Arduino.h"
#include "def.h"
#include "Sensors.h"
#include "CarController.h"
#include "AS5600.h"
#include <Encoder.h>

// int wheelPins[] = wheelSensPins;
static uint8_t PCInt_RX_PINS[PCINT_PIN_COUNT] = {PCINT_RX_BITS};
volatile uint16_t rcValue[4] = {0,0,0,0};
int16_t rcChannels[4] = {0,0,0,0};
long newPosition = 0;
long oldPosition = 0;
uint16_t prevTime, currTime;
uint8_t currRpm, rotations;
uint8_t steeringAngle;

void rcInit(){
    DDRK = 0;

    for(uint8_t i = 0; i < PCINT_PIN_COUNT; i++){
        PCINT_RX_PORT |= PCInt_RX_Pins[i];
        PCINT_RX_MASK |= PCInt_RX_Pins[i];
    }
    PCICR = PCIR_PORT_BIT;
}

void encoderInit(){
    Encoder myEnc(18, 19);
    Serial.println("Encoder initialized");
}

void steeringEncInit(){
    AS5600 as5600;
    as5600.begin(20);       //set to port 20
    as5600.setDirection(AS5600_CLOCK_WISE);
}

#define RX_PIN_CHECK(pin_pos, rc_value_pos) \
  if (mask & PCInt_RX_Pins[pin_pos]) {      \                      
    if (!(pin & PCInt_RX_Pins[pin_pos])) {  \
      if ((cTime-edgeTime[pin_pos]) > 1){ \                     
        rcValue[rc_value_pos] = cTime-edgeTime[pin_pos];  \
      } \                            
    } else edgeTime[pin_pos] = cTime; \                           
   }


ISR(RX_PC_INTERRUPT){
    unit8_t mask;
    uint8_t pin;
    uint16_t cTime, dTime;
    static uint16_t edgeTime[8];
    static uint8_t PCintLast;

    pin = RX_PCINT_PIN_PORT;

    mask = pin ^ PCintLast;
    cTime = micros();
    PCintLast = pin;

    #if (PCINT_PIN_COUNT > 0)
        RX_PIN_CHECK(0,0);
    #endif
    #if (PCINT_PIN_COUNT > 1)
        RX_PIN_CHECK(1,1);
    #endif
    #if (PCINT_PIN_COUNT > 2)
        RX_PIN_CHECK(2,2);
    #endif   
    #if (PCINT_PIN_COUNT > 3)
        RX_PIN_CHECK(3,3);
    #endif
    #if (PCINT_PIN_COUNT > 4)
        RX_PIN_CHECK(4,4);
    #endif
    #if (PCINT_PIN_COUNT > 5)
        RX_PIN_CHECK(5,5);
    #endif
    #if (PCINT_PIN_COUNT > 6)
        RX_PIN_CHECK(6,6);
    #endif
    #if (PCINT_PIN_COUNT > 7)   
        RX_PIN_CHECK(7,7);
    #endif
}


void updateRcValues(){
    rcChannels[0] = rcValue[0];
    rcChannels[1] = rcValue[1];
    rcChannels[2] = rcValue[2];
    rcChannels[3] = rcValue[3];
    rcValue[0] = 0;
    rcValue[1] = 0;
    rcValue[2] = 0;
    rcValue[3] = 0;
}


void updateMotorRpm(){
    newPosition = myEnc.read();
    currTime = micros();
    currRpm = ((newPosition - oldPosition) % 2400) / (currTime - prevTime)

    oldPosition = newPosition;
    prevTime = currTime;

    if (newPosition >= 48000) {
        myEnc.write(newPosition % 2400);
        oldPosition = oldPosition % 2000;
    }
}

void updateSteeringAngle(){
    steeringAngle = as5600.rawAngle() * AS5600_RAW_TO_DEGREES;
}