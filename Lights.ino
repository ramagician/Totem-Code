#include <LinkedList.h> // https://github.com/ivanseidel/LinkedList
#include "FastLED.h"    // https://github.com/FastLED/FastLED

// Input and Output Pin Definitions
#define LED_STRIP_DATA_PIN 7
#define DIAL0 0
#define DIAL1 1
#define DIAL2 2
#define SWITCH0 2
#define SWITCH1 3
#define SWITCH2 4
#define SWITCH3 5

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 120
#define HALF_NUM_LEDS 60
#define BRIGHTNESS 125 // 'Universal'- brightness.


CRGB leds[NUM_LEDS];
// 32 point, 0x40 amplitude sine look up table.
unsigned short sin64[32] =
  {0x20,0x26,0x2c,0x32,0x37,0x3b,0x3e,0x3f,
  0x40,0x3f,0x3e,0x3b,0x37,0x32,0x2c,0x26,
  0x20,0x1a,0x14,0xe,0x9,0x5,0x2,0x1,
  0x0,0x1,0x2,0x5,0x9,0xe,0x14,0x1a};

byte readDialByte(int dial) {
  int sensor = analogRead(dial);
  return sensor >> 2;
}

double readDialDouble(int Dial) {
  int sensor = analogRead(Dial);
  return ((double) sensor) / 1024.0;
}

void setup() {
  delay(3000); // initial delay of a few seconds is recommended
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, LED_STRIP_DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // initializes LED strip
  FastLED.setBrightness(BRIGHTNESS);// global brightness
  pinMode(SWITCH0, INPUT);
  pinMode(SWITCH1, INPUT);
  pinMode(SWITCH2, INPUT);
  pinMode(SWITCH3, INPUT);
}

// switches off all LEDs
void allBlack(long delayTime) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(delayTime);
}

// Shifts multiple pixel from the start of strip to the end. The color of each pixel is randomized.
// delayTime: indicates how long the pixels are shown on each LED
void showProgramShiftMultiPixel(long delayTime) {
  for (int i = 0; i < NUM_LEDS; ++i) { 
    for (int j = i; j > 0; --j) {
      leds[j] = leds[j-1];
    }
    CRGB newPixel = CHSV(random8(), 255, 255);
    leds[0] = newPixel;
    FastLED.show();
    delay(delayTime);
  }
}

// Ball of color specified by DIAL0 moves to end of strip at speed specified by DIAL1.
void ball() {
  int hue = 0;
  int delayTime = 0;
  int ball[5] = {85, 170, 255, 170, 85};
  int brightness;
  for (int i = 0; i < NUM_LEDS; ++i) {
    hue = readDialByte(DIAL0);
    delayTime = readDialByte(DIAL1);
    for (int j = i; j > 0; --j)
      leds[j] = leds[j-1];
    if (i < 5)
      brightness = ball[i];
    else
      brightness = 0;
    CRGB newPixel = CHSV(hue, 255, brightness);
    leds[0] = newPixel;
    FastLED.show();
    delay(delayTime);
  }
}

// Random new color comes in from beginning of strip and moves to end of strip at speed decided by DIAL0.
void sweep() {
  int ball[5] = {175, 200, 255, 200, 175};
  int brightness = 255;
  int colorBall = random8();
  int colorSweep = random8();
  byte delayTime = readDialByte(DIAL0);
  for (int i = 0; i < NUM_LEDS; ++i) {
    for (int j = NUM_LEDS - 1; j > 0; --j)
      leds[j] = leds[j-1];
    CRGB newPixel;
    if (i < 5) {
      brightness = ball[i];
      newPixel = CHSV(colorBall, 255, brightness);
    }
    else
      newPixel = CHSV(colorBall, 255, 150);
    leds[0] = newPixel;
    FastLED.show();
    delayTime = readDialByte(DIAL0);
    delay(delayTime);
  }
}

void shimmer() {
  int delayTime = readDialByte(DIAL0);
  for (int i = NUM_LEDS - 1; i > 0; --i) {
    leds[i] = leds[i-1];
  }
  CRGB colorPink = CRGB(255, 192, 203);
  CRGB colorBlue = CRGB(255, 20, 147);
  if (random8() % 3)
    leds[0] = colorPink;
  else
    leds[0] = colorBlue;
  FastLED.show();
  delay(delayTime);
}

// One side of the strip is white, the other pink. Location of this split is decided by DIAL0.
void partition() {
  int sensor = analogRead(DIAL0);

  int partition = (long)sensor * (long)NUM_LEDS / 1023;
  Serial.print("Sensor: ");
  Serial.println(sensor);
  Serial.print("Partition: ");
  Serial.println(partition);
  for (int i = 0; i < NUM_LEDS; ++i) {
    if (i < partition)
      leds[i] = CRGB(255, 192, 203);
    else
      leds[i] = CRGB(255, 20, 147);
  }
  FastLED.show();
  delay(25);
}

void dividers() {
  int sensor = analogRead(DIAL0);
  int partitionSize = 2048/ (sensor + 1);
  Serial.print("Sensor: ");
  Serial.println(sensor);
  Serial.print("Partition Size: ");
  Serial.println(partitionSize);
  int l = 0;
  for (int i = 0; i < NUM_LEDS; ++i) {
    
    if ((i / partitionSize) % 2 == 0)
      leds[i] = CRGB(255, 192, 203);
    else
      leds[i] = CRGB(255, 20, 147);
  }
  FastLED.show();
  delay(25);
}

// Color "pulsates" from the data-in end of the strip. DIAL0 determines the hue, DIAL1 determines the speed.
void hueWave() {
  int sensor = analogRead(DIAL0);
  int delayTime = readDialByte(DIAL1);
  int hue = (long)sensor * (long)255 / 1023;
  unsigned static int saturation = 0;
  for (int i = NUM_LEDS - 1; i > 0; i--) {
    leds[i] = leds[i - 1];
  }

  leds[0] = CHSV(hue, 180 + sin64[saturation % 32], 255);
  FastLED.show();
  saturation++;
  delay(delayTime);
}

void colorStream() {
  int hue =        readDialByte(DIAL0);
  int value =      readDialByte(DIAL1);
  int delayTime =  readDialByte(DIAL2);
  leds[0] = CHSV(hue, 222, value);
  for (int i = NUM_LEDS - 1; i > 0; i--) {
    leds[i] = leds[i - 1];
  }
  FastLED.show();
  delay(delayTime);
}

void colorSolid(int delayTime) {
  int hue =        readDialByte(DIAL0);
  int value =      readDialByte(DIAL1);
  leds[0] = CHSV(hue, 200, value);
  for (int i = 1; i < NUM_LEDS; i++) {
    leds[i] = leds[0];
  }
  FastLED.show();
  delay(delayTime);
}

void RGBSolid() {
  int R = readDialByte(DIAL0);
  int G = readDialByte(DIAL1);
  int B = readDialByte(DIAL2);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(R, G, B);
  }
  FastLED.show();
  delay(25);
}

void strobe() {
  int delayTime = analogRead(DIAL2);

  //leds[0] = 0xFF4500;
  colorSolid(delayTime);
  allBlack(delayTime);
}

void timeRainbow() {
  byte delayTime = readDialByte(DIAL0);
  static byte hue = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, 255);
  }
  FastLED.show();
  delay(delayTime);
  hue += 3;
}

void rainbowStream() {
  //static byte hue;
  static double hue;
  byte delayTime = readDialByte(DIAL0);
  leds[0] = CHSV(hue, 255, 255);
  for (int i = NUM_LEDS - 1; i > 0; i--) {
    leds[i] = leds[i-1];
  }
  hue += .4 + 10 * readDialDouble(DIAL1);
  FastLED.show();
  delay(delayTime);
}

void allWhite() {
  int brightness = readDialByte(DIAL0);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(0, 0, brightness);
  }
  FastLED.show();
  delay(25);
}

void beads() {
  static byte sineVal = 0;
  byte color1 = readDialByte(DIAL0);
  byte color2 = readDialByte(DIAL1);
  byte offset = 127;//readDialByte(DIAL2);
  byte delayTime = readDialByte(DIAL2);
  for (int i = 0; i < NUM_LEDS; i++) {
    short pos = (i + sin64[sineVal]) % 80;
    if (pos > 13)
      leds[i] = CHSV(color1, 255, 255);
    else
      leds[i] = CHSV(color2, 255, 255);
  }
  FastLED.show();
  delay(25);
  sineVal++;
  sineVal = sineVal % 32;
}

void slidingDoors() {
  int ball[5] = {230, 210, 180, 210, 230};
  int brightness = 255;
  int colorBall = random8();
  int colorSweep = random8();
  byte delayTime = readDialByte(DIAL0);
  for (int i = 0; i < HALF_NUM_LEDS; ++i) {
    /*for (int j = i; j > 0; --j)
      leds[j] = leds[j-1];*/
    for (int ii = 0; ii < HALF_NUM_LEDS; ++ii) {
      leds[ii] = leds[ii + 1];
      leds[NUM_LEDS - ii - 1] = leds[NUM_LEDS - ii - 2];
    }
    CRGB newPixel;
    if (i < 5) {
      brightness = ball[i];
      newPixel = CHSV(colorBall, brightness, 255);
    }
    else
      newPixel = CHSV(colorBall, 255, 255);
    leds[HALF_NUM_LEDS] = newPixel;
    leds[HALF_NUM_LEDS + 1] = newPixel;
    FastLED.show();
    delayTime = readDialByte(DIAL0);
    delay(delayTime);
  }
}

void triples() {
  byte color0, color1, color2;
  color0 = random8();
  do {
    color1 = random8();
  } while (color1 + 25 > color0 && color1 - 25 < color0);
  do {
    color2 = random8();
  } while (color2 + 25 > color1 && color2 - 25 < color1);
  byte delayTime = readDialByte(DIAL0);
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < 40)
      leds[i] = CHSV(color0, 255, 255);
    else if (i < 80)
      leds[i] = CHSV(color1, 255, 255);
    else
      leds[i] = CHSV(color2, 255, 255);
  }
  FastLED.show();
  delay(delayTime * 25);
}

void triplesSweep() {
  byte newColor = random8();
  leds[80] = leds[41];
  leds[40] = leds[0];
  leds[0] = CHSV(newColor, 255, 255);
  for (int i = 1; i < 40; i++) {
    leds[i] = leds[i - 1];
    leds[i + 40] = leds[i + 39];
    leds[i + 80] = leds[i + 79];
  }
  byte delayTime = readDialByte(DIAL0);
  FastLED.show();
  delay(delayTime * 25);
}

void triplesTwo() {
  static byte color0, color1, color2;
  static byte pos = 0;
  pos++;
  if (pos % 3 == 0)
    color0 = random8();
  if (pos % 3 == 1)
    color1 = random8();
  if (pos % 3 == 2)
    color2 = random8();
  byte delayTime = readDialByte(DIAL0);
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < 40)
      leds[i] = CHSV(color0, 255, 255);
    else if (i < 80)
      leds[i] = CHSV(color1, 255, 255);
    else
      leds[i] = CHSV(color2, 255, 255);
  }
  FastLED.show();
  delay(delayTime * 25);
}

void testSwitches(int delayTime) {
  int R = digitalRead(SWITCH0) * 128 + digitalRead(SWITCH1) * 127;
  int G = 255 * digitalRead(SWITCH2);
  int B = 255 * digitalRead(SWITCH3);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(R, G, B);
  }
  FastLED.show();
  delay(delayTime);
}

// Break the strip up into 10 even blocks, randomly select a color, then replace each block with that color in
// a random order until the entire strip is the new color. DIAL0 determines the time it takes for this transition
// to occur, DIAL1 determines the time between transitions.
void blocks() {
  LinkedList<byte> sections = LinkedList<byte>();
  sections.add(0);
  sections.add(1);
  sections.add(2);
  sections.add(3);
  sections.add(4);
  sections.add(5);
  sections.add(6);
  sections.add(7);
  sections.add(8);
  sections.add(9);
  byte newColor = random8();
  int delayTime = readDialByte(DIAL0);
  while (sections.size()) {
    byte sectionOffset = 12 * sections.remove(random8() % sections.size());
    for (int i = sectionOffset; i < 12 + sectionOffset; i++) {
      leds[i] = CHSV(newColor, 255, 255);
    }
    FastLED.show();
    delay(delayTime);
  }
  delayTime = readDialByte(DIAL1);
  delay(delayTime * 10);
}

// main program
void loop() {
  int selector = digitalRead(SWITCH3) + 2 * digitalRead(SWITCH2) + 4 * digitalRead(SWITCH1) + 8 * digitalRead(SWITCH0);
  Serial.print("Selector :");
  Serial.println(selector);

  switch (selector) {
    case 0:
      partition();
      break;
    case 1:
      sweep();
      break;
    case 2:
      shimmer();
      break;
    case 3:
      ball();
      break;
    case 4:
      timeRainbow();
      break;
    case 5:
      hueWave();
      break;
    case 6:
      colorStream();
      break;
    case 7:
      colorSolid(25);
      break;
    case 8:
      rainbowStream();
      break;
    case 9:
      strobe();
      break;
    case 10:
      beads();
      break;
    case 11:
      slidingDoors();
      break;
    case 12:
      triples();
      break;
    case 13:
      triplesSweep();
      break;
    case 14:
      triplesTwo();
      break;
    case 15:
      blocks();
      break;
    default:
      allWhite();
      break;
  }
}
