# STM32-Based Autonomous Fire Fighting Robot

An embedded firefighting system designed to detect flames, navigate using DC motors, position an extinguishing nozzle, and activate a water pump for automated fire suppression.

## Overview

The project focuses on developing an autonomous fire fighting robot using the STM32F407VGT6 microcontroller. Flame sensors detect fire and provide input to the controller, which coordinates robot movement, servo-based nozzle positioning, and water pump activation through a relay module.

The system also incorporates IoT-based monitoring and remote control features using Wi-Fi communication for displaying fire detection events and robot activity.

## Key Features

- Flame detection using flame sensor modules.
- STM32-based embedded control and decision-making.
- DC motor direction and speed control using the L293D motor driver.
- Servo-based nozzle positioning.
- Automatic water pump activation through a relay module.
- LED-based system status indication.
- IoT connectivity for monitoring and remote control.

## Hardware Components

- STM32F407VGT6 Microcontroller
- Flame Sensor Modules
- L293D Motor Driver
- DC Motors
- Servo Motors
- Relay Module
- Water Pump
- LEDs
- Power Supply

## Software Components

- Embedded C
- STM32 HAL
- STM32CubeIDE
- GPIO and PWM Control
- ADC-based Sensor Interface
- UART Communication

## IoT and Monitoring

- ESP32 / Wi-Fi Communication
- Cloud Database
- Web Dashboard
- Remote Monitoring and Control
- Sensor Data and Status Logging

## Working Principle

1. Flame sensors continuously monitor the surroundings for fire.
2. Sensor readings are processed by the STM32F407VGT6.
3. The controller manages DC motor movement through the L293D driver.
4. When fire is detected, the robot stops and positions the nozzle using servo motors.
5. The relay module activates the water pump to initiate fire extinguishing.
6. Status indicators and the monitoring interface provide information about system operation.

## Applications

- Industrial Fire Detection and Suppression
- Laboratories and Research Facilities
- Warehouses and Storage Areas
- Hazardous Environments
- Remote Fire Monitoring Systems

## Future Scope

Integration of obstacle avoidance, advanced flame localization, intelligent navigation, real-time IoT analytics, and improved autonomous fire suppression capabilities.

