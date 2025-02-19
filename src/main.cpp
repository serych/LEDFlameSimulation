/* Simple but better PWM candle flame simulation for cheap Chinese LED candles*/
#include <Arduino.h>
// define pins based on the used candle flame light so, that L1 is the bottom one and L6 the top one

/*  
// smaller 12V LED light
#define L1 PIN_PA3
#define L2 PIN_PA0
#define L3 PIN_PA2
#define L4 PIN_PA1
#define L5 PIN_PA7
#define L6 PIN_PA6
*/

// Bigger USB stick LED light
#define L1 PIN_PA1
#define L2 PIN_PA7
#define L3 PIN_PA6
#define L4 PIN_PA2
#define L5 PIN_PA0
#define L6 PIN_PA3

// We will use 64 steps PWM e.i. 6 EV flame dynamics
// PWM frequency is about 550 Hz
#define CYCLES 64
#define CYCLES_LOG 6 // logarithm (base 2) of CYCLES 
#define RTM_MIN 1536  // min and max borders of random time
#define RTM_MAX 4096  // for new random values calculation

uint8_t pow2(int n) {
  // function that calculates 2 ^ N by shifting bit 1 to the left
  return 1U << n; // 1 shifted left by n positions
}

uint8_t rnd_bri(uint8_t min, uint8_t max) {
  // random brightness calculation
  return pow2(random(min, max));
}

static uint16_t loop_cnt = 0; // loop counter
uint8_t pwm_reg = 0;  // PWM timer register
uint16_t recalc_tmr = random(RTM_MIN, RTM_MAX);  // random delay to recalculate values
// random brightness values initialization. We do it exponentialy as the human eye sees brightness logarithmicaly
// each step has double brightness of the previous step (or 1 EV increase)  
uint8_t r1 = pow2(random(0,CYCLES_LOG));
uint8_t r2 = pow2(random(0,CYCLES_LOG));
uint8_t r3 = pow2(random(0,CYCLES_LOG));
uint8_t r4 = pow2(random(0,CYCLES_LOG));
uint8_t r5 = pow2(random(0,CYCLES_LOG));
uint8_t r6 = pow2(random(0,CYCLES_LOG));

void setup() {
  // setting all the pins to be outputs
pinMode(L1, OUTPUT);
pinMode(L2, OUTPUT);
pinMode(L3, OUTPUT);
pinMode(L4, OUTPUT);
pinMode(L5, OUTPUT);
pinMode(L6, OUTPUT);
}

void loop() {
  // we run the loop for random times (recalc_tmr) with one set of brightness values
  // than we calculate new random values and the pwm loop continues  

  pwm_reg = (loop_cnt % CYCLES);  //PWM register cycles between 0 and CYCLES

  // PWM duty cycles calculation for all the LEDs. The bottom one runs on its own, but the higher LEDs
  // are dependent on the underlaying ones. It creates  illusion of flame licking from bottom to the top 
  digitalWrite(L1, !(pwm_reg >= r1));     // LOW state is active, so if the pwm_reg >= random value, switch the LED on
  digitalWrite(L2, !(pwm_reg >= r2+r1));  // smaller the random value brighter the LED
  digitalWrite(L3, !(pwm_reg >= r3+r2+r1));
  digitalWrite(L4, !(pwm_reg >= r4+r3+r2+r1));
  digitalWrite(L5, !(pwm_reg >= r5+r4+r3+r2));
  digitalWrite(L6, !(pwm_reg >= r6+r5+r4+r3));
  ++loop_cnt;
  if (loop_cnt > recalc_tmr) {  //recalculation of the random brightness values
    r1 = (r1 + rnd_bri(3, CYCLES_LOG)) >> 1; // to smooth the flame we calculate floating average of the previous and new value (prev + new)/2 
    r2 = (r2 + rnd_bri(2, CYCLES_LOG)) >> 1; // division by 2 is done by shifting result to the right
    r3 = (r3 + rnd_bri(3, CYCLES_LOG)) >> 1;
    r4 = (r4 + rnd_bri(3, CYCLES_LOG)) >> 1;
    r5 = (r5 + rnd_bri(4, CYCLES_LOG)) >> 1;
    r6 = (r6 + rnd_bri(4, CYCLES_LOG)) >> 1;
    recalc_tmr = random(RTM_MIN, RTM_MAX);   // new random recalculation time
    loop_cnt = 0; // reset of the loop counter
  }

}

