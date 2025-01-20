# About
## About Hourglass
Hourglass provides wireless turn management system among connected devices. The system operates using multiple peripheral devices that are operated locally by the individual user and a single central device that process the data provided by the peripherals. The central device controls the state of the device and provides any additional data needed for the device to display the commanded state. The central device does not need to be actively operated and provides game configuration options

## About Electrical 
The peripheral device uses an ESP32 microcontroller as the main processor. This unit is responsible for establishing and maintaining a data connection to the central device as well as displaying the current device state to the user using the display interface connected. The device is powered by a 3.7V LiPo battery whose output goes through a 5V boost converter to provide steady power to the controller. The battery is charged using a T4056 charging circuit. The battery output to the converter is controlled by a on-off switch. The microcontroller then provides power and connects to the display interface. Going into the microcontroller is a momentary push button that serves as the only input interface the user has to the device.

The display interface will be swappable at some point but for now, it is a 16 light ARGB LED display that interacts with the controller using the WS2812 protocol.

## About Embedded
The microcontroller software is programmed primarily in C++ and leverages the Arduino framework. The program lifecycle starts with looking for applicable central devices on boot over Bluetooth Low Energy. The peripheral advertises a fixed characteristic that is known by the central device. Once the central device establishes a connection with the peripheral, the program will continuously read device state data from the central and update the display accordingly. The software takes the device state and any supplemental data and displays the information using the display interface. The program is also responisble for handling user input and providing applicable user actions to the central unit. If the peripheral does not establish a connection within X minutes, it will enter a deep sleep mode that can be exited using the push button.

A simulator is also available to allow for animation development for the 16 light display

## About Mobile
The central device uses an Android app (HourglassCompanion) to perform the turn sequencing and provide device state to the peripheral devices. The app supports multiple turn sequencing modes (explained below). Connections to individual peripheral devices are initiated through the application prior to gactivityame start. Once all desired devices are connected, the activity can be configured to support the user's needs and then started. While the game is running, the central device actively communicates with all devices connected in the activity. The central device also offers controls to manage the state of the game such as pausing and advancing turn orders. Game configurations can also be updated while a game is active.

Turn Sequencing Modes:
- Sequential
    - Turn sequencing is performed sequentially where a single user/device is active at a time. Completion of a turn automatically advances the active user to the next user in the turn sequence
- Simulataneous
    - All users are active at the same time. The turn end processing will occur when all users have indivually completed their turn
- Solo
    - Device acts as a single hourglass in the traditional sense. There is no sequencing of turns between devices and all devices 
- Buzzer
    - Devices are activated in parallel as in Simulataneous mode. Turn completion is reported back to the central device and is prioritized in time based order. This mode is recommended for trivia or any activity that requires time based comparisons between user's actions

# Built With
- Arduino
- FastLED
- openGL

# Getting Started
There are a few steps that need to be followed to properly provision a development environemnt. The project uses PlatformIO to build and flash the microcontrollers. It is recommended to use the VS Code PlatformIO extension when configuring the environment
## Prerequisites
- Install the PlatformIO extension in VS Code
- (Simulator) Ensure the C++ compiler is properly configured for the desktop environment
- 
## Installation
1. Clone this repo
`git clone ....`
2. ...
3. ...
4. ...

# Usage
This project provides the embedded software that is installed on the microcontroller. Once this is installed and the controller is running, the program that is built should immediately start running on the peripheral device

# Roadmap
- Updated Display Interfaces
- Animation Encoding/Decoding
- Custom Animation Rendering
- Support for other turn sequencing modes

# Contributing