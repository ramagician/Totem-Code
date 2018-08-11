# Lightning in a Bottle 2018 TotemCode

A few months ago, I went to the Lightning in a Bottle music festival. With the help of a few friends of mine, we made a cool totem (for those have never been to a festival, a totem is a large sign or object used as a beacon to help keep friends together in a large crowd) that would light up at night. Our totem comprised of an LED light strip inside a clear plastic tube, wrapped in light defraction paper. This repository has all the code used to drive the LED lights.

## Settings Datasheet

Selector (Dec) | Selector (Bin) | Function | Dial 0 | Dial 1 | Dial 2
--- | --- | --- | --- | --- | ---
0 | 0000 | Partition | Partition | position
1 | 0001 | Sweep | Speed
2 | 0010 | Shimmer | Speed
3 | 0011 | Ball	Hue | Speed
4 | 0100 | Time Rainbow | Speed
5 | 0101 | Hue Wave | Hue | Speed
6 | 0110 | Color Stream | Hue | Brightness | Speed
7 | 0111 | Color Solid | Hue | Brightness
8 | 1000 | Rainbow Stream | Speed | Rainbow width
9 | 1001 | Strobe | Hue | Brightness | Speed
10 | 1010 | Beads | Hue 1 | Hue 2 | Speed
11 | 1011 | Sliding Doors | Speed
12 | 1100 | Triples | Speed
13 | 1101 | Triples Sweep | Speed
14 | 1110 | Triples Two | Speed
15 | 1111 | Blocks | Transition time | Time between transitions

## Dependancies

In order to complile Lights.ino, you will need the [LinkedList](https://github.com/ivanseidel/LinkedList) and [FastLED](https://github.com/FastLED/FastLED) dependancies.

## Hardware Info

This program was tested on an Arduino Nano with a WS2812B 120 LED lightstrip. I choose to wire four single pull, single throw switches to digital pins [2:5] to control the light strip mode. I wired three potentiometers whose outputs were in the range of 0-5V to analog pins [0:2] to control each mode's analog parameters. Digital pin 7 was connected to the light strip's data input. This wiring configuration can be changed easily by modifying the #define statements at the top of Lights.ino.

## Power Info

We found that this entire setup requries anywhere between 5-10W of power to run smoothly, depending on what setting you use the lights on. Thus, you should be able to power this system using most decent portable power banks available on Amazon.