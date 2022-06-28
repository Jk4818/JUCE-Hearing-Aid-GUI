# Hearing Aid GUI

Cross-platform GUI application for controlling an adaptive steerable beamformer hearing aid (HA) through OSC.
Check out the HA system here: https://github.com/Jk4818/openMHA-HA-System

For a quick demo of the entire system in action click **[here](https://youtu.be/tEZiaqk7CAc)**.

Simple steps to run and modify this cross-platform application on mobile and desktop.


# Installation

The application runs using the JUCE framework. Please download it **[here](https://juce.com/get-juce)**.
Download or  **pull** the GitHub repository. It contains the source files that can be built on your device.

## Prerequisites
Projucer requires corresponding IDEs for modifying and building the application on each operating system.

### Windows 11 Desktop
Visual Studio 2022 is the most up to date IDE for windows development.  Make sure to retarget the project to the most up to date visual C++ platform toolset.

### Android
Android Studio is best for developing android applications. To run C++ and JUCE on an Android project ensure the latest NDK and SDK are installed.

### macOS & IOS
These systems have not been tested on, but building on them should be similar to other OS. The compass feature and sensor readings must be refactored for IOS devices.



# Usage Guide

To run the application on Windows 11 access the executable in: `SteerableBeamformerApp\Builds\VisualStudio2019\x64\Release\App`

Run the **"Hearing Aid App.exe"**.

> When running on any device make sure the application has firewall permission for network access, else it will not be able to receive any OSC messages.


## Connecting to HA

The application has a form field for typing the device address of the openMHA HA. It requires:

 - **Network device IP** (IPv4)
 - **OSC Port number** (Set through the osc2ac configuration variable)

The application runs on the device IP address and the **default port of the application is 7778**.

### OSC Addresses
Default OSC addresses for GUI sender:

 - `/steer/cal/out`
 - `/steer/fixbeam/out`
 - `/steer/source`

Default OSC addresses for GUI receiver:

 - `/main/out/rot`
 - `/tascarpos`

> Location sources are expected to be predefined via TASCAR


## Controls
After connecting the GUI to the openMHA system the controls are revealed.

To **change the beamformer angle** simply use the slider or drag the beamformer element on the beam dial.

The **Set North** button calibrates the current head tracker position as North. To use:

 - Point the head to north and press the set north button.
 - The head dial should reset to where north is currently positioned on the map.

Pressing **Fix Beam To Head** will stick the current position of the beamformer to the head. This means that the beam will always face the same direction wherever the head turns.




