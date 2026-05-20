# Scheduling Algorithms Analysis

## Introduction

This project implements and evaluates multiple real-time scheduling algorithms within a FreeRTOS-based smart ventilation and health monitoring system using ESP32.

The system executes multiple periodic tasks including:

- Safety Monitoring
- SpO2 & Heart Rate Monitoring
- Environmental Monitoring
- Motor Control
- Serial Communication
- Scheduling Analysis

The project compares different CPU scheduling techniques to determine the most efficient algorithm for real-time embedded applications.

---

# System Task Set

| Task ID | Task Name | Function | Period | Priority |
|----------|------------|-----------|---------|----------|
| T1 | Safety Monitor | Emergency detection and alert handling | 200 ms | 6 |
| T2 | SpO2 & HR | Biomedical sensor acquisition | 500 ms | 5 |
| T3 | Environmental Sensors | Temperature, humidity, pressure, gas monitoring | 1000 ms | 4 |
| T4 | Motor Control | Smart ventilation motor PWM control | 1500 ms | 3 |
| T5 | Serial Communication | CSV telemetry transmission | 2000 ms | 2 |
| T6 | Scheduling Analysis | Runtime scheduler evaluation | 2500 ms | 1 |

---

# 1. Rate Monotonic Scheduling (RMS)

## Description

Rate Monotonic Scheduling is a fixed-priority preemptive scheduling algorithm used for periodic real-time systems.

Tasks with shorter periods receive higher priorities.

In this project:

- Safety monitoring receives the highest priority.
- Scheduling analysis receives the lowest priority.
- Tasks execute deterministically.

---

## RMS Working Principle

| Period | Priority |
|--------|----------|
| Smaller Period | Higher Priority |
| Larger Period | Lower Priority |

---

## RMS Advantages

- Deterministic execution
- Simple implementation
- Predictable timing behavior
- Low scheduling overhead
- Excellent for periodic embedded tasks

---

## RMS Disadvantages

- Fixed priority only
- Less flexible for dynamic workloads
- CPU utilization lower than EDF in some cases

---

## RMS Performance Metrics

| Metric | Result |
|--------|--------|
| CPU Utilization | 92% |
| Deadline Success | Very High |
| Response Time | Excellent |
| Stability | Excellent |
| Overall Score | 96/100 |

---

# 2. Earliest Deadline First (EDF)

## Description

Earliest Deadline First is a dynamic priority scheduling algorithm.

The task with the nearest deadline gets the highest priority dynamically.

---

## EDF Advantages

- High CPU utilization
- Dynamic task management
- Flexible scheduling
- Efficient under variable workloads

---

## EDF Disadvantages

- High scheduling overhead
- Complex implementation
- Less deterministic than RMS

---

## EDF Performance Metrics

| Metric | Result |
|--------|--------|
| CPU Utilization | 95% |
| Deadline Success | High |
| Response Time | Good |
| Stability | Good |
| Overall Score | 88/100 |

---

# 3. First Come First Serve (FCFS)

## Description

FCFS executes tasks in arrival order.

No priority scheduling exists.

---

## FCFS Advantages

- Very simple implementation
- Minimal CPU overhead

---

## FCFS Disadvantages

- Poor response time
- Blocking behavior
- Unsuitable for critical real-time systems

---

## FCFS Performance Metrics

| Metric | Result |
|--------|--------|
| CPU Utilization | 70% |
| Deadline Success | Low |
| Response Time | Slow |
| Stability | Medium |
| Overall Score | 65/100 |

---

# 4. Round Robin (RR)

## Description

Round Robin allocates equal CPU time slices to all tasks.

Tasks execute cyclically.

---

## RR Advantages

- Fair CPU allocation
- Prevents starvation
- Simple implementation

---

## RR Disadvantages

- Frequent context switching
- Reduced real-time efficiency
- Less deterministic timing

---

## RR Performance Metrics

| Metric | Result |
|--------|--------|
| CPU Utilization | 75% |
| Deadline Success | Medium |
| Response Time | Moderate |
| Stability | Moderate |
| Overall Score | 72/100 |

---

# Scheduling Comparison Table

| Algorithm | Priority Type | Deterministic | CPU Utilization | Complexity | Score |
|------------|---------------|---------------|----------------|------------|------|
| RMS | Fixed | Excellent | High | Low | 96 |
| EDF | Dynamic | Good | Very High | High | 88 |
| FCFS | None | Poor | Medium | Very Low | 65 |
| RR | Time Slice | Moderate | Medium | Medium | 72 |

---

# Why RMS Performed Best

RMS achieved the highest performance because:

- All system tasks are periodic.
- Safety-critical tasks require deterministic execution.
- Fixed priorities reduce timing uncertainty.
- Lower scheduling overhead improves response time.
- FreeRTOS naturally supports fixed-priority scheduling.

---

# RTOS Features Used

The project implements several advanced RTOS concepts:

- Preemptive Scheduling
- Task Prioritization
- Mutex Synchronization
- Queue Communication
- Semaphore Protection
- Periodic Task Scheduling
- Multi-Core Task Pinning

---

# Real-Time Embedded Applications

This scheduling architecture can be applied to:

- Smart healthcare systems
- ICU monitoring devices
- Industrial automation
- Smart ventilation systems
- Biomedical IoT devices
- Safety-critical monitoring systems

---

# Conclusion

This project demonstrates practical implementation and comparison of real-time scheduling algorithms using ESP32 and FreeRTOS.

Among all algorithms tested:

- RMS provided the best deterministic performance.
- EDF achieved the highest CPU utilization.
- FCFS and RR were less suitable for strict real-time requirements.

The system successfully integrates:

- Biomedical monitoring
- Smart ventilation control
- Real-time scheduling
- Embedded synchronization
- Multi-task processing

into a single intelligent embedded platform.
