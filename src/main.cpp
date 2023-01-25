#include <Arduino.h>
#include "targets/target.h"
#include "main/led.h"
#include "main/position.h"
#include "main/direction.h"
#include "main/servos.h"
#include "main/accgyro.h"

void setup() {
  xTaskCreate(
    ledTask,
    "ledTask",
    1000,
    NULL,
    5,
    NULL
  );

  xTaskCreate(
    positionTask,
    "positionTask",
    1000,
    NULL,
    3,
    NULL
  );

  xTaskCreate(
    directionTask,
    "directionTask",
    1000,
    NULL,
    3,
    NULL
  );

  xTaskCreate(
    servosTask,
    "servosTask",
    1000,
    NULL,
    3,
    NULL
  );

  xTaskCreate(
    accgyroTask,
    "accgyroTask",
    1000,
    NULL,
    3,
    NULL
  );
}

void loop() {
  // put your main code here, to run repeatedly:
}