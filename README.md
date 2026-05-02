# VO2-Max-Respirator
Portable VO2 max respirator to measure biometric VO2. Reads airflow from respirator mask, sends output to phone via BLE. 
## Overview

This project implements a real-time embedded system for estimating **oxygen consumption (VO₂)** using a **Venturi-based airflow sensor** and an **analog O₂ sensor**, running on the Raspberry Pi Pico (RP2040).

The system performs high-rate data acquisition, signal processing, and wireless transmission over **Bluetooth Low Energy (BLE)**. It is designed with a focus on deterministic timing, low-level hardware control, and robust signal conditioning.

* Custom PCB for MCU, sensors with buffers, charge and power circuitry
* Real-time airflow estimation using Venturi differential pressure
* Analog O₂ concentration measurement and calibration
* Exponential moving average (EMA) filtering for noise reduction
* BLE GATT server for wireless data transmission
* Dual-core architecture (sampling + communication separation)
* Interrupt-driven sampling (timer-based)
* Modular firmware design (ADC, DSP, BLE layers)

---

## System Architecture

### Core 1 (Data Path)

* Timer-triggered sampling (100 Hz)
* ADC acquisition (pressure + O₂)
* Signal conversion (ADC → physical units)
* Airflow calculation (Venturi equation)
* Digital filtering (EMA, noise rejection)
* VO₂ estimation

### Core 0 (Communication)

* BLE stack (BTstack)
* GATT server implementation
* Periodic notification of computed VO₂ data

---

## Hardware

* Custom PCB
* Raspberry Pi Pico (RP2040)
* Analog pressure sensor (MP3V5010DP)
* Analog oxygen sensor (SGX-40X with custom amplifier buffer)
* External ADC (MCP3204)
* Charge circuitry and battery powering (TP-4056 charger IC)

---

## Key Algorithms

### Venturi Flow Equation

Airflow is computed using:

* Differential pressure input
* Known inlet/outlet diameters
* Air density assumptions

The implementation accounts for:

* Unit conversion (ΔkPa → mL/min)
* Numerical stability (NaN/invalid handling)

---

### DSP

Converted ADC data streams of O2 and pressure sensors to usable information.

* Used EMA for smoothing noisy signals
* Included noise rejection to filter out brief spikes
* Ensure signal reading includes "memory" of past inputs
* Modular design


---

## BLE Communication

* Custom GATT profile
* Periodic notifications using `ATT_EVENT_CAN_SEND_NOW`
* Data transmitted as 16-bit values for efficiency


## Author

Arthur
Embedded / Firmware Engineer