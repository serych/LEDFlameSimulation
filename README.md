# LED candle flame simulation
There is lot of cheep flame simulating LED lights on the chinese market. They usualy produce "random" flame pattern which is not very random and it is possible to see repeating elements in it very easily. You can see it in the short video below.

![Various types of candle flame simulating lights](https://github.com/user-attachments/assets/711d7fc9-7069-4b83-af7e-9afc33bd9796)

[Original flickering](https://github.com/user-attachments/assets/bbe8782f-01fa-4b69-ae37-fa6fe20877db)

I have found that all of the lamps I have bought are fitted with an unmarked chip that is pin compatible with the Attiny 412. I even originally thought it was the Attiny, but the programmer won't communicate with it at all, so it must be some other chip.   

![Chip1](https://github.com/user-attachments/assets/4e0f83d0-30fd-4265-b664-c92f9d0f3ac4)


![Chip2](https://github.com/user-attachments/assets/ae2d5016-fbf7-40c9-a961-65c1855aed66)

Since the chips are easily accessible and easy to disconnect with hot air, I decided to try replacing them with Attiny 412s with my own program. 

# Basic idea of the code

The Attiny 412 processor includes PWM outputs, but only two of them and usable only on specific pins. So I first tried a version of random flashing by simply "digitally" turning the LEDs on and off in random intervals. It's usable to some extent, but a PWM version would certainly be better, and the Chinese originals obviously use it.  

Later I thought that since the processor won't do anything else, it could handle the software PWM. 
And indeed it does. Using a PWM with a resolution of 64 states (2 ^ 6), the PWM frequency reaches about 550 Hz. Understandably, the frequency decreases when using higher resolution, and increases when using lower resolution, in a roughly linear fashion. So, for example, at a resolution of 32 states it is somewhere around 1.2 kHz. 
## Logarithmic perception of the eye
The human eye perceives light intensity logarithmically. This means that light stronger by one degree (photographers call it 1EV) is twice the intensity. Since we have a PWM with a resolution of 64 = 2 ^ 6, this version will give us 6 degrees of light intensity, which turns out to be perfectly adequate for this application.  

## Hardware issues
The code controls six LEDs labeled L1 to L6. It is assumed that L1 is located at the lowest (at the wick of the imaginary candle) while L6 is at the top of the highest flame. 
The Attiny 412 has pins PA0, PA1, PA2, PA3, PA6 and PA7.
The first thing you need to do for a particular Chinese candle is to find out which pin controls the LED at which flame position. Then in the definitions at the beginning of the code, assign L1 to L6 so that L1 would be controling lowest and L6 is highest LED.

![Attiny 412 pinout](https://github.com/user-attachments/assets/0fa73b6f-d530-423a-9e38-d5c77108f839)
### Current Load Capacity
All the Chinese candles I have tried are wired so that the anodes of the LEDs are directly connected to +5 V, while the cathodes are connected via a serial resistor to the controller pins. This means that the pins work in sink mode. The LED is lit when the value LOW (logic 0) is written to the pin.
There is written in the datasheet of Attiny that the total sum of continuous currents of the pins should not exceed 100mA (both in source and sink mode). 
Even though the instantaneous current is probably larger at some points, the Attiny has no problem lighting a candle containing even 6 fairly bright LEDs on each pin with a series resistance of only 39 ohms.
### Program description
The counter is incremented in the main loop of the program and its value modulo "CYCLES" forms the counter register for the PWM. Once every random time, new random values of the brightness of each LED are calculated. We want the bottom of the flame to change brightness only slightly and to be lit almost permanently. Conversely, the top flame should only occasionally lick the top LEDs. Therefore, different minima and maxima are used when generating the random values for each LED.
The flame licking is handled in such a way that the top LEDs depend on the state of the bottom LEDs (the more the lower LED is lit at any given moment, the more likely the flame will lick the higher LEDs). 
It is good to play with the values in these sections of the program and " tailor" the flame to your liking.  
To make the flame movement smoother, the newly generated values are always averaged with the previous ones. 
After the new values are generated, the program's main loop counter is reset new random time for new recalculation is generated and the process repeats endlessly. 



