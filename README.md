# About
## About Hourglass
Hourglass provides a wireless turn management system among connected devices. The system consists of multiple peripheral devices operated locally by individual users and a single central device that processes data from the peripherals. The central device manages the system state and provides additional data required for display on the peripherals. The central device does not need to be actively operated and provides game configuration options

Prototype Demo: https://drive.google.com/file/d/1vfFwuXrZOG86haQg10q6Bc9yUWXI989V/view?usp=drive_link

## About Electrical 
The peripheral device utilizes an ESP32 microcontroller as its main processor. This unit establishes and maintains a data connection to the central device and updates the user interface accordingly. The device is powered by a 3.7V LiPo battery, which is regulated through a 5V boost converter to provide steady power to the controller. The battery is charged via a TP4056 charging circuit, with its output controlled by an on-off switch. The microcontroller powers and connects to the display interface while also handling input from a momentary push button, which serves as the user's primary input modality.

The display interface will eventually support multiple options, but currently, it consists of a 16-light ARGB LED display that communicates with the controller using the WS2812 protocol.

## About Embedded
The microcontroller software is primarily programmed in C++ using the Arduino framework. The program lifecycle begins by searching for available central devices over Bluetooth Low Energy (BLE) upon boot. The peripheral advertises a fixed characteristic known to the central device. Once a connection is established, the peripheral continuously reads device state data from the central device and updates the display accordingly.

The software handles device state processing, user input, and communication with the central unit. If the peripheral fails to establish a connection within a predefined time limit, it enters a deep sleep mode, which can be exited via the push button.

A simulator is also available to facilitate animation development for the 16-light display.

## About Mobile
The central device is an Android application (Hourglass Companion) responsible for turn sequencing and state management of peripheral devices. The app supports multiple turn sequencing modes (described below). Connections to peripheral devices are established before the game starts. Once all devices are connected, the user can configure and initiate the activity. During gameplay, the central device actively communicates with all peripherals and provides controls for managing the game state, including pausing and advancing turns. Game configurations can also be adjusted during an active session.

Turn Sequencing Modes:

Sequential: Turns progress in a fixed sequence, with one user/device active at a time. Completion of a turn automatically advances to the next user.

Simultaneous: All users are active simultaneously. The turn ends when all users have individually completed their turns.

Solo: The device acts as a standalone hourglass. No sequencing occurs between devices, and each operates independently.

Buzzer: Similar to the simultaneous mode, but turn completion is reported back to the central device in real-time and prioritized in order of completion. This mode is recommended for trivia or any activity that requires time based comparisons between user's actions

# Built With
- **Arduino** (Embedded development)
- **FastLED** (LED control)
- **OpenGL** (Simulator rendering)
- **GoogleTest** (Unit testing)

# Getting Started
There are a few steps that need to be followed to properly provision a development environemnt. The project uses PlatformIO to build and flash the microcontrollers. It is recommended to use the VS Code PlatformIO extension when configuring the environment
## Prerequisites
- Install the PlatformIO extension in VS Code
- (For the simulator) Ensure the C++ compiler is correctly configured for the desktop environment.
- (For the simulator) Download, build, and install the latest FreeGLUT library.
- 
### Installation
1. Clone the repository:
   ```sh
   git clone <repo_url>
   ```
2. Open the project in VS Code with PlatformIO installed.
3. Configure the microcontroller environment in `platformio.ini`.
4. Build and flash the firmware to the ESP32 device.
5. (For the simulator) Ensure FreeGLUT is installed and linked correctly in the build process.

## Usage
This project provides the embedded software for the ESP32 microcontroller. Once flashed, the program will start running automatically on the peripheral device.

## Roadmap
- Support for additional display interfaces
- Animation encoding/decoding
- Custom animation rendering
- Additional turn sequencing modes

## Contributing
Contributions are welcome! Please follow these steps:
1. Fork the repository.
2. Create a feature branch.
3. Commit changes.
4. Open a pull request.

For major changes, open an issue first to discuss your proposed updates.
