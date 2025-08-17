# Execution plan

## Unity tests

- [X] nodeMCU heart beat
- [X] nodeMCU server and write to EEPROM
- [X] gyroscope
- [ ] mp3 player
- [X] LED strip
- [X] button

Heart beat and server and EEPROM went well without big problems.

Gyroscope was a little hard to configure and read. I may get a headache when trying to use this reading along with the mp3 player.

Button behaviour is not as I wanted it to be. I am far from an expert, but [it seems like I cannot get multithreading with ArduinoIDE](https://www.esp8266.com/viewtopic.php?f=32&t=22985). This will not allow stopping wake up and shut down routines on new button presses. I don't think it is a big problem, but still not optimal.

LED strip is drawing too much current, the button (intentionally left on the circuit from previous assemble) fades out in brightness as more pixels are activated on the strip. I will need to connect it directly to the power source once I buy it. Also, I just found out that what takes the most time is actually showing the pixels on the trip, so I removed the delay and implemented the speed control with more pixel being powered at a time.

I am now experimenting with the mp3 player module. I have already managed to make the sound work, but I cannot yet break down how I control which file is played.

## Current measures

After those are tested, I will measure the current drawn by each module to estimate the battery capacity I need.

After that, I will buy and test the battery charging modules and the battery monitor.

## Final assembly

With everything properly tested, I will assemble all the parts and develop the final code. Probably I will do it in several micro-steps.

