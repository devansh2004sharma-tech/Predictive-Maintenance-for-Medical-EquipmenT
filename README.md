# Predictive Maintenance for Medical Equipment

## 📌 Project Overview

This project is an **IoT-based predictive maintenance and condition-monitoring prototype** designed to monitor the operational health of motor-driven medical equipment in real time.

The system uses an **ESP32** as the central controller to continuously monitor:

* ⚡ Motor Current
* 📳 Motor Vibration
* 🌡️ Temperature
* 💧 Humidity
* ⚙️ Motor Operating Status

Sensor data is displayed locally using an **I2C LCD** and remotely through the **Blynk IoT platform**.

The system can detect abnormal operating conditions and automatically stop the motor when the measured current exceeds a predefined safety threshold.

> **Note:** The current implementation is a condition-monitoring prototype with threshold-based fault detection. It does not yet implement machine-learning-based failure prediction.

---

## 🎯 Problem Statement

Medical equipment often contains electrically driven components such as:

* Motors
* Pumps
* Fans
* Compressors
* Actuators

Failure of these components can interrupt equipment operation, increase maintenance costs, reduce equipment availability, and potentially affect the reliability of medical services.

Traditional maintenance approaches include:

### Corrective Maintenance

```text
Equipment Failure
       ↓
Fault Identification
       ↓
Repair
```

### Preventive Maintenance

```text
Scheduled Maintenance
       ↓
Equipment Servicing
       ↓
Continued Operation
```

This project demonstrates a third approach:

### Predictive / Condition-Based Maintenance

```text
Continuous Monitoring
        ↓
Abnormal Condition Detection
        ↓
Warning / Protection
        ↓
Prevent or Reduce Equipment Failure
```

---

## 🎯 Objectives

The main objective is to develop a **low-cost IoT-enabled monitoring and protection system** for motor-driven medical equipment.

### Specific Objectives

1. Monitor motor current in real time.
2. Detect abnormal vibration.
3. Monitor temperature and humidity.
4. Display sensor readings locally.
5. Send equipment data to an IoT dashboard.
6. Detect potentially abnormal operating conditions.
7. Automatically stop the motor when current exceeds a predefined threshold.
8. Provide fault indication to the user.
9. Demonstrate the concept of condition-based predictive maintenance using embedded hardware and IoT.

---

## 🏗️ System Architecture

```text
                    ┌──────────────────────┐
                    │   Medical Equipment  │
                    │    / DC Motor Load   │
                    └──────────┬───────────┘
                               │
                         Motor Parameters
                               │
             ┌─────────────────┼─────────────────┐
             │                 │                 │
             ▼                 ▼                 ▼
      ┌─────────────┐   ┌─────────────┐   ┌─────────────┐
      │   ACS712    │   │   SW-420    │   │    DHT11    │
      │   Current   │   │  Vibration  │   │ Temperature │
      │   Sensor    │   │   Sensor    │   │ & Humidity  │
      └──────┬──────┘   └──────┬──────┘   └──────┬──────┘
             │                 │                 │
             └─────────────────┼─────────────────┘
                               ▼
                       ┌──────────────┐
                       │    ESP32     │
                       │              │
                       │ Data         │
                       │ Processing   │
                       │ Fault Logic  │
                       └───────┬──────┘
                               │
                  ┌────────────┼────────────┐
                  │                         │
                  ▼                         ▼
           ┌─────────────┐          ┌─────────────┐
           │   I2C LCD   │          │  Blynk IoT  │
           │    Local    │          │    Mobile   │
           │  Monitoring │          │  Monitoring  │
           └─────────────┘          └─────────────┘
                               │
                               ▼
                       ┌──────────────┐
                       │    L298N     │
                       │ Motor Driver │
                       └───────┬──────┘
                               │
                               ▼
                           DC Motor
```

---

## 🔧 Hardware Components

| Component              | Purpose                                   |
| ---------------------- | ----------------------------------------- |
| **ESP32**              | Main controller and IoT communication     |
| **ACS712**             | Motor current measurement                 |
| **SW-420**             | Vibration detection                       |
| **DHT11**              | Temperature and humidity measurement      |
| **L298N**              | DC motor driver                           |
| **12V DC Motor**       | Represents the motor-driven equipment     |
| **I2C LCD**            | Local display of sensor values and status |
| **12V Battery/Supply** | Motor power source                        |

---

## ⚙️ Working Principle

The system operates continuously in a monitoring loop.

### 1. System Initialization

The ESP32 initializes:

* Sensors
* I2C LCD
* Motor driver
* Wi-Fi connection
* Blynk IoT connection

### 2. Sensor Data Acquisition

The ESP32 periodically reads:

```text
Current
Vibration
Temperature
Humidity
```

### 3. Data Processing

The sensor outputs are processed by the ESP32.

For the ACS712:

```text
ACS712 Analog Output
        ↓
     ESP32 ADC
        ↓
 Current Calculation
        ↓
 Current Value (A)
```

The measured values are then compared against predefined conditions.

### 4. Fault Detection

The primary protection mechanism is excessive motor current.

```text
Current Measurement
        ↓
Is Current > Threshold?
        │
     ┌──┴──┐
    NO     YES
     │      │
     ▼      ▼
Continue   Fault
Operation  Detected
              ↓
         Stop Motor
              ↓
       Fault Indication
              ↓
        Update LCD/Blynk
```

### 5. Local Monitoring

The I2C LCD displays sensor measurements and system status, such as:

```text
Current: 0.65 A
Temp: 28°C
Humidity: 60%
```

or:

```text
Motor Running
```

During a fault condition:

```text
Motor Stopped
Fault Detected
```

### 6. Remote Monitoring

The ESP32 sends sensor information through Wi-Fi to the Blynk IoT platform.

```text
Sensors
   ↓
 ESP32
   ↓
 Wi-Fi
   ↓
Blynk Cloud
   ↓
Mobile Dashboard
```

---

## ⚡ Current-Based Fault Detection

Motor current is one of the primary parameters used for equipment condition monitoring.

An abnormal increase in current may be associated with conditions such as:

* Mechanical overload
* Increased friction
* Motor obstruction
* Bearing-related problems
* Excessive mechanical load
* Electrical abnormalities

The basic protection logic is:

```text
Normal Operation
      ↓
Current within Safe Range
      ↓
Motor Continues Running
```

Whereas:

```text
Abnormal Condition
      ↓
Motor Current Increases
      ↓
ESP32 Detects Threshold Violation
      ↓
Motor Stops
      ↓
Fault Indication
```

---

## 📳 Vibration Monitoring

The **SW-420 vibration sensor** is used to detect abnormal mechanical vibration.

Excessive vibration can potentially indicate:

* Mechanical imbalance
* Loose components
* Misalignment
* Bearing-related problems
* External mechanical disturbances

The ESP32 reads the sensor's digital output and incorporates the information into the equipment monitoring system.

---

## 🌡️ Temperature & Humidity Monitoring

The **DHT11** measures:

* Temperature
* Relative Humidity

Temperature monitoring can provide useful information about equipment operating conditions.

An increase in temperature may potentially be associated with:

* Increased motor loading
* Excessive current
* Poor ventilation
* Continuous operation
* Abnormal operating conditions

Humidity provides additional environmental information around the equipment.

---

## 📱 IoT Monitoring with Blynk

The project uses **Blynk IoT** for remote monitoring.

The ESP32 connects to Wi-Fi and sends sensor information to the Blynk platform.

This allows the user to monitor equipment parameters remotely through a mobile dashboard.

### Monitoring Concept

```text
              ┌──────────────┐
              │    Sensors   │
              └──────┬───────┘
                     ↓
              ┌──────────────┐
              │     ESP32    │
              └──────┬───────┘
                     ↓
                  Wi-Fi
                     ↓
              ┌──────────────┐
              │ Blynk Cloud  │
              └──────┬───────┘
                     ↓
              ┌──────────────┐
              │    Mobile    │
              │   Dashboard  │
              └──────────────┘
```

---

## 🔌 Motor Control

The **L298N motor driver** provides the interface between the ESP32 and the DC motor.

The ESP32 cannot directly supply the current required by the motor, so the motor driver is used between the controller and motor.

```text
ESP32
  ↓
Control Signals
  ↓
L298N Motor Driver
  ↓
Motor Supply
  ↓
12V DC Motor
```

The motor driver is used for:

* Motor start/stop control
* Direction control
* Enable/disable operation

---

## 🧠 Fault Detection Logic

The simplified control algorithm is:

```text
START
  │
  ▼
Initialize ESP32
  │
  ▼
Initialize Sensors
  │
  ▼
Connect to Wi-Fi / Blynk
  │
  ▼
Read Sensor Data
  │
  ▼
Check Motor Current
  │
  ├───────────────┐
  │               │
Normal          Abnormal
  │               │
  ▼               ▼
Motor          Stop Motor
Continues          │
                   ▼
             Fault Detected
                   │
                   ▼
             Update LCD/Blynk
                   │
                   ▼
              Repeat Loop
```

---

## 🛠️ Technology Stack

### Hardware

```text
ESP32
ACS712
SW-420
DHT11
L298N
12V DC Motor
I2C LCD
12V Battery/Supply
```

### Software

```text
Arduino IDE
Embedded C/C++
Blynk IoT
```

### Communication

```text
Wi-Fi
I2C
```

### IoT Platform

```text
Blynk
```

---

## 📁 Repository Structure

A recommended repository structure is:

```text
Predictive-Maintenance-Medical-Equipment/
│
├── README.md
│
├── Code/
│   └── predictive_maintenance.ino
│
├── Schematics/
│   └── system_schematic.png
│
├── Images/
│   ├── prototype.jpg
│   ├── circuit.jpg
│   └── lcd_display.jpg
│
├── Documentation/
│   └── Project_Report.pdf
│
└── LICENSE
```

If multiple versions of the code are available:

```text
Code/
│
├── Basic_Motor_Control/
├── Sensor_Monitoring/
└── Final_Predictive_Maintenance/
```

---

## 📸 Project Demonstration

Add photographs of your actual prototype here.

For example:

```text
### Hardware Prototype
![Prototype](Images/prototype.jpg)

### Circuit
![Circuit](Images/circuit.jpg)

### LCD Display
![LCD Display](Images/lcd_display.jpg)
```

---

## 🚀 Future Improvements

The current project provides the hardware and IoT foundation for a more advanced predictive-maintenance system.

### 1. Machine Learning Integration

Historical sensor data can be collected and used to train machine-learning models for equipment failure prediction.

Possible algorithms include:

* Regression
* Decision Trees
* Random Forest
* SVM
* Neural Networks
* Anomaly Detection

### 2. Cloud Data Storage

Store historical sensor readings in a database for long-term analysis and trend monitoring.

### 3. Advanced Vibration Analysis

Replace the basic vibration sensor with an accelerometer or industrial vibration sensor to obtain more detailed vibration information.

### 4. Remaining Useful Life Estimation

Develop a system capable of estimating the **Remaining Useful Life (RUL)** of equipment.

### 5. Multiple Equipment Monitoring

Extend the system to monitor multiple machines simultaneously.

### 6. Automated Maintenance Alerts

Implement notifications when equipment shows abnormal behavior or requires inspection.

### Future Architecture

```text
Sensors
   ↓
ESP32
   ↓
Historical Data
   ↓
Cloud / Database
   ↓
Machine Learning Model
   ↓
Failure Prediction
   ↓
Maintenance Alert
```

---

## 💡 Project Significance

This project combines multiple engineering domains:

```text
Electrical Engineering
        +
Embedded Systems
        +
Sensors
        +
Motor Control
        +
IoT
        +
Fault Detection
        ↓
Predictive Maintenance Prototype
```

It demonstrates how electrical parameters such as **motor current**, along with mechanical and environmental parameters such as **vibration, temperature and humidity**, can be used to monitor the health of motor-driven equipment.

---

## 📌 Key Skills Demonstrated

* ESP32 Programming
* Embedded Systems
* Sensor Interfacing
* Motor Control
* Current Measurement
* Vibration Monitoring
* Temperature & Humidity Monitoring
* Fault Detection
* IoT Communication
* Blynk IoT
* I2C Communication
* Arduino IDE
* Embedded C/C++

---

## 📄 Resume Description

**Predictive Maintenance for Medical Equipment | ESP32, ACS712, DHT11, SW-420, Blynk, L298N**

> Developed an IoT-enabled condition-monitoring prototype using ESP32 to continuously monitor motor current, vibration, temperature and humidity. Implemented threshold-based fault detection with automatic motor shutdown during excessive-current conditions and integrated LCD/Blynk dashboards for local and remote monitoring.

---

## 🏷️ GitHub Topics

```text
esp32
iot
predictive-maintenance
medical-equipment
arduino
blynk
embedded-systems
condition-monitoring
fault-detection
acs712
dht11
vibration-sensor
motor-monitoring
```

---

## ⚠️ Disclaimer

This project is an **academic/prototype implementation** intended to demonstrate IoT-based equipment condition monitoring and fault protection.

It is **not intended for direct deployment in safety-critical medical equipment without appropriate engineering validation, electrical safety testing, regulatory compliance, and medical-device certification**.

---

## 👨‍💻 Author

**Devansh Sharma**

B.Tech — Electrical & Electronics Engineering

---

## ⭐ Acknowledgement

This project was developed as an academic engineering project exploring the integration of **embedded systems, electrical monitoring, motor control, sensors, IoT, and predictive/condition-based maintenance**.
