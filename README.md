# RTOS-Based Smart Ventilation & Health Monitoring System

## Overview

This project is a real-time embedded monitoring and smart ventilation system developed using **ESP32** and **FreeRTOS**.

The system continuously monitors:

- SpO2
- Heart Rate
- Temperature
- Humidity
- Pressure
- Altitude
- Gas Level

Based on real-time biomedical conditions, the system dynamically controls a ventilation motor using PWM and activates safety alerts during abnormal conditions.

---

# Key Features

- FreeRTOS Multi-Tasking System
- Real-Time Sensor Monitoring
- PWM-Based Smart Ventilation
- Biomedical Monitoring
- Alert & Safety System
- Scheduling Algorithm Comparison
- CSV Serial Telemetry
- Mutex, Queue, and Semaphore Synchronization

---

# FreeRTOS Task Architecture

| Task ID | Task Name | Function | Period | Priority |
|----------|------------|-----------|---------|----------|
| T1 | Safety Monitor | Emergency condition monitoring | 200 ms | 6 |
| T2 | SpO2 & Heart Rate | MAX30100 biomedical acquisition | 500 ms | 5 |
| T3 | Environmental Sensors | BME280, DHT11, MQ135 acquisition | 1000 ms | 4 |
| T4 | Motor Control | PWM ventilation control | 1500 ms | 3 |
| T5 | Serial Communication | CSV telemetry transmission | 2000 ms | 2 |
| T6 | Scheduler Comparison | RMS, EDF, FCFS, RR evaluation | 2500 ms | 1 |

---

# Sensors Used

| Sensor | Purpose |
|--------|---------|
| MAX30100 | SpO2 & Heart Rate |
| BME280 | Temperature, Humidity, Pressure |
| DHT11 | Backup Temperature/Humidity |
| MQ135 | Air Quality Monitoring |

---

# Hardware Used

- ESP32
- MAX30100
- BME280
- DHT11
- MQ135
- L298N Motor Driver
- DC Motor
- LEDs
- Buzzer

---

# Scheduling Algorithms Compared

The project evaluates:

- Rate Monotonic Scheduling (RMS)
- Earliest Deadline First (EDF)
- First Come First Serve (FCFS)
- Round Robin (RR)

Performance Metrics:
- Response Time
- CPU Utilization
- Stability
- Alert Responsiveness

RMS achieved the best overall deterministic performance.

---

# Motor Control Logic

| SpO2 Range | Motor Duty |
|------------|------------|
| 95–100% | 30% |
| 91–94% | 55% |
| 90–91% | 75% |
| <90% | 100% |

---

# RTOS Concepts Implemented

- Preemptive Scheduling
- Rate Monotonic Scheduling
- Task Prioritization
- Mutex Synchronization
- Queue Communication
- Semaphore Protection
- Periodic Tasks
- Multi-Core Task Pinning

---

# Serial Output Format

```csv
DATA,TEMP,HUM,PRES,ALT,GAS,SPO2,HR,BEAT,ALERT,MOTOR
```

Example:

```csv
DATA,30.50,62.10,1012.20,15.10,185,97.0,74.5,1,0,30
```

---

# Software Used

- Embedded C
- Arduino IDE
- FreeRTOS
- PWM
- I2C
- UART Serial Communication

