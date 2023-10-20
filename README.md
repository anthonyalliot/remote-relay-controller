
# README


- [Project Overview](#project-overview)
- [BOM](#bom)
- [Building and Programming](#building-and-programming)
- [Code Repo Organization](#code-repo-organization)
- [Program Concept](#program-concept)


## <a name="project-overview"></a>Project Overview

remote-relay-controller is a simple project froked from [mcu-relay-controller project](https://github.com/matt-garman/mcu-relay-controller/tree/main)
and using the [Intelligent Relay Bypass Module from PedalPcb](https://www.pedalpcb.com/product/relaybypass/).
My idea was to be able to toggle a guitar pedal (a wah pedal in my case) remotely thanks to the 
[G-lab Wah-Pad](https://glab.com.pl/product/wah-pad/). To do so, I had to change a little bit the 
way of using this pcb and to make some adjustments in the initial project in order to make it compatible with the 
PedalPCB pcb and with the ATTINY13A only.

**My need was very simple:**
- when the pad is pressed (either a short press or a long press) the effect is on
- when the pad is released the effect is bypassed

**Constraints:**
- It has to work remotely from the pedal with the [G-lab Wah-Pad](https://glab.com.pl/product/wah-pad/)
(so, it was impossible to use a [DPDT momentary footswitch](https://www.taydaelectronics.com/dpdt-compact-stomp-foot-pedal-switch-momentary.html))
- Use [Intelligent Relay Bypass Module from PedalPcb](https://www.pedalpcb.com/product/relaybypass/)


## <a name="bom"></a>BOM

- 1* [pcb from PedalPcb](https://www.pedalpcb.com/product/relaybypass/) + all the required [components](https://docs.pedalpcb.com/project/PedalPCB-IntelligentRelayBypass.pdf)
- 4* 1/4" mono jack sockets:
	- 2 for the input/output dedicated to the pedal you want to toggle remotely -> they are wired up to the in and out pads located in the middle of the pcb (don't forget to wire the jack socket's sleeve pin to ground)
	- 2 for the input/output for the signal chain -> they are wired up to the in and out pads located on the sides the of pcb (don't forget to wire the jack socket's sleeve pin to ground)
- 1* 1/4" stereo jack socket for the G-lab Wah-Pad -> the sleeve and the ring pins of the socket have to be wired up to the FS pads
- 1* 1590B enclosure -> containing the pcb + all the jack sockets. Note: it can be placed under a pedalboard
- 1* DC socket

**Required Hardware for microcontroller programming:**
- 1* Arduino
- 1* Breadboard
- Some jumper wires


## <a name="building-and-programming"></a>Building and Programming

**How to setup your development environment:**
- Follow this [tutorial](https://www.instructables.com/Updated-Guide-on-How-to-Program-an-Attiny13-or-13a/) for setting up your hardware (until step 2)
- Follow this [AVR GCC Toolchain tutorial](https://tinusaur.com/guides/avr-gcc-toolchain/) for installing all the tools you need

Since it's a small project, I used [notepad++](https://notepad-plus-plus.org/downloads/) as code editor.

**How to build the image and to upload it to the ATTINY13A:**

*Note:* skip the `avr-gcc` and `avr-objcopy` steps if you just want
to use the precompiled image located in the *build* folder.
```
avr-gcc -Os -std=gnu99 -DF_CPU=1000000UL -DATTINY13 -mmcu=attiny13 -o attiny13.elf mcu-relay-controller.c hardware-details/attiny.c
avr-objcopy -j .text -j .data -O ihex attiny13.elf attiny13.hex
avrdude -c usbtiny -p attiny85 -v -P usb -U flash:w:attiny13.hex
```


## <a name="code-repo-organization"></a>Code Repo Organization

Two source files constitute the core of the project:

- `mcu-relay-controller-iface.h`: this defines the hardware-abstracted
  interface for the project; it is essentially a high-level collection of
  hardware-independent functions (and macros) which can be used to build a
  microcontroller-driven relay circuit
- `mcu-relay-controller.c`: this defines the main() function, and uses the
  interfaces defined in the above h file

Top level directories:
- `hardware-details`: contain a .c and .h file for the ATTINY13A microcontroller
- `build`: contains precompiled images, ready to be flashed to an
  microcontroller


## <a name="program-concept"></a>Program Concept

See the documentation of the [initial project](https://github.com/matt-garman/mcu-relay-controller/blob/main/README.md#program-concept)