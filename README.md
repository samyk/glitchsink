# glitchsink
Voltage glitcher to bypass instructions/bootloader protections *without* target modification

-----------------------------

Entirely non-invasive Teensy-based (3.x, 4.0) Voltage Glitcher for bypassing instructions & bootloader protection for flash dumping.

Tested on LPC1343 & ATmega328P

-samy kamkar, 2019/10/14

Upon mux'ing to glitch voltage level, glitch side sinks current from target device (bypass caps & MCU) via variable capacitance + resistance, depleting it enough to allow proper instruction skipping.

It's likely that if bypass capacitors were removed from target, the attack would simply happen "slightly later" (clock cycle or more?)

Simplest setup uses MAX4619 + 10uF cap on Y1, or variable resistance with digital potentiometer
