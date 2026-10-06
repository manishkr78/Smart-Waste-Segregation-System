# Working Flow

1. Arduino starts and initializes sensors and motors.
2. The system waits for waste/object detection.
3. A proximity/IR sensor detects an object.
4. The moisture sensor is sampled three times.
5. The readings are converted to an approximate moisture percentage.
6. If moisture is greater than 20%, the system selects the wet-waste route.
7. Otherwise, it selects the dry-waste route.
8. The stepper motor moves the sorting mechanism.
9. The servo opens the gate and releases the waste.
10. The mechanism returns to its home position.
11. The system waits for the next object.
