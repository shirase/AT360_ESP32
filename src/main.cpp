#include <Arduino.h>
#include "main/position.h"
#include "main/direction.h"
#include "main/servos.h"

void setup() {
  xTaskCreate(
    positionTask,
    "positionTask",
    1000,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    directionTask,
    "directionTask",
    1000,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    servosTask,
    "servosTask",
    1000,
    NULL,
    1,
    NULL
  );
}

void loop() {
  // put your main code here, to run repeatedly:
}