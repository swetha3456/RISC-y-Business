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


