# glitchsink

Glitchsink is a tool demonstrating a new technique I've developed in order to perform voltage glitching, bypassing microcontroller bootloader/debugger protections in order to extract protected firmware and access memory debugging features, *without* target board modification. No bypass capacitor removal is required as it exploits internal charge drain by acting as larger, external parasitic capacitance during glitching.

Additionally, while I initially built glitchsink in Verilog using the Artix-7 XC7A35T FPGA, I've ported it to a ~$20 microcontroller in C++ with ARM assembly for the precise timing components, demonstrating that costly FPGA boards are not required for state-of-the-art glitching techniques against modern microprocessors.

[Hackaday 2019 Talk - FPGA Glitching & Side Channel Attacks](https://www.youtube.com/watch?v=oGndiX5tvEk) briefly discussing this project and other side channel attacks.

-----------------------------

Initially developed in Verilog for the Entirely non-invasive Teensy-based (3.x, 4.0) Voltage Glitcher for bypassing instructions & bootloader protection for flash dumping. Was first designed to extract secret keys from protected chips inside of acccess control locks without breaking plastic or altering PCBs and works across a wide range of manufacturers and models, examples such as LPC1343, ATmega328P, nRF52, and more.

-samy kamkar, 2019/10/14

Upon mux'ing to glitch voltage level, glitch side sinks current from target device (bypass caps & MCU) via variable capacitance + resistance, depleting it enough to allow proper instruction skipping.

It's likely that if bypass capacitors were removed from target, the attack would simply happen "slightly later" (clock cycle or more?)

Simplest setup uses MAX4619 + 470uF cap on Y1, or variable resistance with digital potentiometer
