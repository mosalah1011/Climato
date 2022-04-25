/*
  This example shows how to take simple range measurements with the VL53L1X. The
  range readings are in units of mm.
*/

#include <Wire.h>
#include <VL53L1X.h>

VL53L1X sensor;

const int numLectures = 15;

int Lectures[numLectures];      // the readings from the analog input
int lectureIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

void setup()
{
  //  while (!Serial) {}
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }

  // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
  // You can change these settings to adjust the performance of the sensor, but
  // the minimum timing budget is 20 ms for short distance mode and 33 ms for
  // medium and long distance modes. See the VL53L1X datasheet for more
  // information on range and timing limits.
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(50000);

  // Start continuous readings at a rate of one measurement every 50 ms (the
  // inter-measurement period). This period should be at least as long as the
  // timing budget.
  sensor.startContinuous(50);

  // initialize all the readings to 0:
  for (int initIndex = 0; initIndex < numLectures; initIndex++) {
    Lectures[initIndex] = 0;
  }
}

void loop()
{

  // subtract the last reading:
  total = total - Lectures[lectureIndex];
  // read from the sensor:
  Lectures[lectureIndex] = sensor.read();
  // add the reading to the total:
  total = total + Lectures[lectureIndex];
  // advance to the next position in the array:
  lectureIndex = lectureIndex + 1;

  // if we're at the end of the array...
  if (lectureIndex >= numLectures) {
    // ...wrap around to the beginning:
    lectureIndex = 0;
  }

  // calculate the average:
  average = total / numLectures;
  // send it to the computer as ASCII digits
  Serial.print(" distance moyenne: = ");      //afficher la distance moyenne stable
  Serial.print(average);
  delay(10);        // delay in between reads for stability

  Serial.print("    distance = ");            //afficher la distance instantanée
  Serial.print(sensor.read());
  Serial.println(" mm");

  if (sensor.timeoutOccurred()) {             //temps limite d'aquisition de donnée atteint
    Serial.println(" TIMEOUT");
  }

  Serial.println();

}
