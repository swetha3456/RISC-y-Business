# LiFi Lock
## Team RISC-y Business
- Swetha Murali
- PVS Sukeerthi

## Introduction
Most lockers today have a single layer of protection such as a keypad or padlock. This can lead to potential security breaches and theft. We propose a simple multi-step authentication protocol for lockers that can ensure protection without the need for excessive hardware/software equipment. Our idea uses LiFi technology and a simple smartphone app to unlock the locker.

## Overview
When the app is opened, the user is prompted to verify their biometrics and/or enter a security pin. The authentication method and pin can be set by the user while locking the locker. If authentication is successful, the app switches on the flashlight for a secret fixed amount of time. This duration is precise to the millisecond and hence cannot be replicated manually. The sensor detects this light and opens the locker only if the duration matches exactly. This makes the locker secure as well as cost-effective.

## Components Required
1. VSDSquadron Mini board
2. 2 LEDs (for indicating locked/unlocked state)
3. 470 ohm resistor
4. LDR module
5. 12V power source
6. Servomotor
7. Piezo Buzzer

## Circuit Connection Diagram

![image](https://github.com/swetha3456/RISC-y-Business/assets/116883043/ec83738e-5fd6-4168-8dbf-c796f7fe334c)

## Table of Pin Connections

| LDR Module    | VSDSquadron Mini |
| -------- | ------- |
| D0  | PD1    |
| VCC | 5V     |
| GND    | GND    |

| Servomotor    |  VSDSquadron Mini  |
| -------- | ------- |
| Signal  | PD2    |
| VCC | 5V     |
| GND    | GND    |

|  VSDSquadron Mini    |   |
| -------- | ------- |
| PD3  | +ve terminal of green LED |
| PD4  | +ve terminal of red LED   |
| PD5  | +ve terminal of piezo buzzer   |

## Demo
### Correct Duration
[https://github.com/swetha3456/RISC-y-Business/assets/122366969/5087e0fe-6683-40c8-ac32-68366dc6bc78](https://github.com/swetha3456/RISC-y-Business/assets/116883043/3f962ec2-b862-41dd-9524-dfbab11fd508)

### Incorrect Duration
[https://github.com/swetha3456/RISC-y-Business/assets/116883043/c7000a63-5290-435b-a3f7-7c70f2ba1ccf](https://github.com/swetha3456/RISC-y-Business/assets/116883043/3f962ec2-b862-41dd-9524-dfbab11fd508)




