#include <Arduino.h>

#define L1 PIN_PA3
#define L2 PIN_PA0
#define L3 PIN_PA2
#define L4 PIN_PA1
#define L5 PIN_PA7
#define L6 PIN_PA6
#define CYCLE 64
#define CYCLE_LOG 6 // logarithm (base 2) of CYCLE 
#define SHRT 3

unsigned int pow2(int n) {
  return 1U << n; // 1 shifted left by n positions
}

static uint16_t takt = 0;
uint8_t sta = 0;
uint16_t tmr = random(2000,8000);
uint8_t r1 = (uint8_t) pow2(random(0,CYCLE_LOG));
uint8_t r2 = (uint8_t) pow2(random(0,CYCLE_LOG));
uint8_t r3 = (uint8_t) pow2(random(0,CYCLE_LOG));
uint8_t r4 = (uint8_t) pow2(random(0,CYCLE_LOG));
uint8_t r5 = (uint8_t) pow2(random(0,CYCLE_LOG));
uint8_t r6 = (uint8_t) pow2(random(0,CYCLE_LOG));



void setup() {
  // put your setup code here, to run once:
pinMode(L1, OUTPUT);
pinMode(L2, OUTPUT);
pinMode(L3, OUTPUT);
pinMode(L4, OUTPUT);
pinMode(L5, OUTPUT);
pinMode(L6, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  ++takt;
  sta = (takt % CYCLE);     
  digitalWrite(L1,! (sta>=r1));
  digitalWrite(L2,! (sta>=r2+r1));
  digitalWrite(L3,! (sta>=r3+r2+r1));
  digitalWrite(L4,! (sta>=r4+r3+r2+r1));
  digitalWrite(L5,! (sta>=r5+r4+r3+r2));
  digitalWrite(L6,! (sta>=r6+r5+r4+r3));
  
  if (takt>tmr) {
    r1 = (r1 + (uint8_t) pow2(random(3,CYCLE_LOG))) >> 1;
    r2 = (r2 + (uint8_t) pow2(random(2,CYCLE_LOG))) >> 1;
    r3 = (r3 + (uint8_t) pow2(random(3,CYCLE_LOG))) >> 1;
    r4 = (r4 + (uint8_t) pow2(random(3,CYCLE_LOG))) >> 1;
    r5 = (r5 + (uint8_t) pow2(random(4,CYCLE_LOG))) >> 1;
    r6 = (r6 + (uint8_t) pow2(random(4,CYCLE_LOG))) >> 1;
    tmr = random(1500,4000);
    takt = 0;
  }

}

