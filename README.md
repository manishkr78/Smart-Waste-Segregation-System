# Smart Waste Segregation System

An Arduino-based automatic waste segregation prototype that detects waste, estimates its moisture level, and uses a stepper motor and servo motor to direct the waste toward the appropriate sorting position.

## Features

- Automatic object detection
- Moisture-based waste classification
- Wet/dry waste sorting logic
- Stepper motor positioning
- Servo-controlled waste gate
- Buzzer alerts
- Serial Monitor output for debugging
- Configurable sensor threshold and motor/servo settings

## How It Works

```text
Waste/Object
     |
     v
Proximity / IR Detection
     |
     v
Read Moisture Sensor
     |
     v
Calculate Moisture %
     |
     +---- Moisture > 20% ----> WET
     |                            |
     |                            v
     |                      Stepper + Servo
     |
     +---- Moisture <= 20% ---> DRY
                                  |
                                  v
                            Stepper + Servo
```

> The 20% value is a starting threshold. It should be calibrated with the actual moisture sensor and the types of waste used in the prototype.

## Hardware

| Component | Pin |
|---|---:|
| IR Sensor | D5 |
| Proximity Sensor | D6 |
| Servo Motor | D7 |
| Stepper IN1 | D11 |
| Stepper IN2 | D10 |
| Stepper IN3 | D9 |
| Stepper IN4 | D8 |
| Buzzer | D12 |
| Moisture/Soil Sensor | A0 |

## Software / Libraries

- Arduino IDE
- C/C++
- `Servo.h` — included with Arduino
- `CheapStepper` — install through Arduino IDE Library Manager

### Install CheapStepper

1. Open Arduino IDE.
2. Go to **Sketch → Include Library → Manage Libraries**.
3. Search for **CheapStepper**.
4. Install it.
5. Open `Arduino/Smart_Waste_Segregation/Smart_Waste_Segregation.ino`.

## Uploading the Code

1. Connect the Arduino board to the computer.
2. Open the `.ino` file.
3. Select the correct **Board** from `Tools → Board`.
4. Select the correct **COM Port** from `Tools → Port`.
5. Verify/compile the code.
6. Upload it to the Arduino.
7. Open **Serial Monitor** at **9600 baud**.

## Calibration

The moisture sensor is converted into an approximate percentage using:

- Wet raw value: `485`
- Dry raw value: `1023`
- Wet classification threshold: `20%`

These values are defined near the top of the Arduino sketch:

```cpp
constexpr int WET_THRESHOLD_PERCENT = 20;
constexpr int MOISTURE_RAW_DRY = 1023;
constexpr int MOISTURE_RAW_WET = 485;
```

Different moisture sensors can produce different readings. Calibrate these values using real samples before relying on the classification.

## Project Structure

```text
Smart-Waste-Segregation-System/
│
├── Arduino/
│   └── Smart_Waste_Segregation/
│       └── Smart_Waste_Segregation.ino
│
├── Documentation/
│   ├── circuit-diagram.png
│   ├── working-flowchart.png
│   └── project-report.pdf
│
├── Images/
│   └── Add project photos here
│
├── Demo/
│   └── demo-video-link.txt
│
├── .gitignore
├── LICENSE
└── README.md
```

## Future Improvements

- Add multiple waste categories such as plastic, metal, paper and organic waste.
- Add an LCD/OLED display.
- Add IoT monitoring.
- Add a camera and machine-learning based classification.
- Add bin-fill-level monitoring.
- Add cloud data logging.
- Improve sensor calibration and mechanical positioning.

## Author

**Manish Kumar**

This project was created as an academic/hackathon-style hardware project for learning Arduino, sensors, motor control and automation.
