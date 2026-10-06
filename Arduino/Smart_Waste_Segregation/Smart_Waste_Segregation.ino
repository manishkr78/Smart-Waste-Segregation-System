#include <CheapStepper.h>
#include <Servo.h>


// Pin configuration
namespace Pins {
  constexpr uint8_t IR_SENSOR       = 5;
  constexpr uint8_t PROXIMITY       = 6;
  constexpr uint8_t SERVO           = 7;
  constexpr uint8_t STEPPER_IN1     = 11;
  constexpr uint8_t STEPPER_IN2     = 10;
  constexpr uint8_t STEPPER_IN3     = 9;
  constexpr uint8_t STEPPER_IN4     = 8;
  constexpr uint8_t BUZZER          = 12;
  constexpr uint8_t MOISTURE_SENSOR = A0;
}


// System configuration
namespace Config {
  constexpr int SERVO_CLOSED_ANGLE = 95;
  constexpr int SERVO_OPEN_ANGLE   = 180;

  constexpr int STEPPER_ROTATION_DEGREES = 240;
  constexpr int STEPPER_RPM = 17;

  // Moisture above this value is classified as wet.
  constexpr int WET_THRESHOLD_PERCENT = 20;

  // Raw moisture sensor calibration range.
  // Adjust these values after testing your actual sensor.
  constexpr int MOISTURE_RAW_DRY = 1023;
  constexpr int MOISTURE_RAW_WET = 485;

  constexpr uint8_t MOISTURE_SAMPLES = 3;

  constexpr unsigned long SERVO_SETTLE_MS = 1000;
  constexpr unsigned long SERVO_OPEN_MS   = 2000;
  constexpr unsigned long MOTOR_SETTLE_MS = 1000;
  constexpr unsigned long SENSOR_COOLDOWN_MS = 500;
}


// Hardware objects

Servo sortingServo;

CheapStepper stepper(
  Pins::STEPPER_IN1,
  Pins::STEPPER_IN2,
  Pins::STEPPER_IN3,
  Pins::STEPPER_IN4
);

bool systemBusy = false;


// Utility functions

void beep(unsigned int frequency, unsigned long duration) {
  tone(Pins::BUZZER, frequency, duration);
}

void setServoPosition(int angle) {
  sortingServo.write(angle);
  delay(Config::SERVO_SETTLE_MS);
}

void openWasteGate() {
  sortingServo.write(Config::SERVO_OPEN_ANGLE);
  delay(Config::SERVO_OPEN_MS);

  sortingServo.write(Config::SERVO_CLOSED_ANGLE);
  delay(Config::SERVO_SETTLE_MS);
}

int readMoisturePercent() {
  long total = 0;

  for (uint8_t sample = 0; sample < Config::MOISTURE_SAMPLES; sample++) {
    int rawValue = analogRead(Pins::MOISTURE_SENSOR);

    // Keep the reading inside the calibrated range.
    rawValue = constrain(
      rawValue,
      Config::MOISTURE_RAW_WET,
      Config::MOISTURE_RAW_DRY
    );

    int percentage = map(
      rawValue,
      Config::MOISTURE_RAW_WET,
      Config::MOISTURE_RAW_DRY,
      100,
      0
    );

    total += percentage;
    delay(75);
  }

  return total / Config::MOISTURE_SAMPLES;
}

void printMoisture(int moisturePercent) {
  Serial.print(F("Moisture: "));
  Serial.print(moisturePercent);
  Serial.println(F("%"));
}

void rotateToWetPosition() {
  Serial.println(F("Classification: WET"));
  Serial.println(F("Moving to wet-waste position..."));

  stepper.moveDegreesCCW(Config::STEPPER_ROTATION_DEGREES);
  delay(Config::MOTOR_SETTLE_MS);

  openWasteGate();

  stepper.moveDegreesCW(Config::STEPPER_ROTATION_DEGREES);
  delay(Config::MOTOR_SETTLE_MS);

  Serial.println(F("Returned to home position."));
}

void rotateToDryPosition() {
  Serial.println(F("Classification: DRY"));
  Serial.println(F("Moving to dry-waste position..."));

  stepper.moveDegreesCW(Config::STEPPER_ROTATION_DEGREES);
  delay(Config::MOTOR_SETTLE_MS);

  openWasteGate();

  stepper.moveDegreesCCW(Config::STEPPER_ROTATION_DEGREES);
  delay(Config::MOTOR_SETTLE_MS);

  Serial.println(F("Returned to home position."));
}

void classifyAndSort() {
  systemBusy = true;

  beep(1000, 500);
  delay(500);

  int moisturePercent = readMoisturePercent();
  printMoisture(moisturePercent);

  if (moisturePercent > Config::WET_THRESHOLD_PERCENT) {
    rotateToWetPosition();
  } else {
    rotateToDryPosition();
  }

  delay(Config::SENSOR_COOLDOWN_MS);
  systemBusy = false;

  Serial.println(F("--------------------------------"));
}

void handleProximityDetection() {
  int proximityState = digitalRead(Pins::PROXIMITY);

  // INPUT_PULLUP means LOW = detected.
  if (proximityState == LOW && !systemBusy) {
    Serial.println(F("Object detected by proximity sensor."));
    beep(1000, 1000);

    // stepper motor movement.
    stepper.moveDegreesCCW(Config::STEPPER_ROTATION_DEGREES);
    delay(Config::MOTOR_SETTLE_MS);

    openWasteGate();

    stepper.moveDegreesCW(Config::STEPPER_ROTATION_DEGREES);
    delay(Config::MOTOR_SETTLE_MS);
  }
}

void handleMoistureDetection() {
  int irState = digitalRead(Pins::IR_SENSOR);

  // IR sensor LOW = object detected.
  if (irState == LOW && !systemBusy) {
    Serial.println(F("Object detected by IR sensor."));
    classifyAndSort();
  }
}


// Arduino setup

void setup() {
  Serial.begin(9600);

  pinMode(Pins::PROXIMITY, INPUT_PULLUP);
  pinMode(Pins::IR_SENSOR, INPUT);
  pinMode(Pins::BUZZER, OUTPUT);

  sortingServo.attach(Pins::SERVO);

  stepper.setRpm(Config::STEPPER_RPM);

  // Initialize sorting gate.
  sortingServo.write(Config::SERVO_OPEN_ANGLE);
  delay(2000);

  sortingServo.write(Config::SERVO_CLOSED_ANGLE);
  delay(Config::SERVO_SETTLE_MS);

  Serial.println();
  Serial.println(F("================================"));
  Serial.println(F(" SMART WASTE SEGREGATION SYSTEM "));
  Serial.println(F("================================"));
  Serial.println(F("System initialized successfully."));
  Serial.print(F("Wet threshold: "));
  Serial.print(Config::WET_THRESHOLD_PERCENT);
  Serial.println(F("%"));
  Serial.println(F("Waiting for waste..."));
  Serial.println();
}


// Main loop

void loop() {
  handleProximityDetection();
  handleMoistureDetection();
}
