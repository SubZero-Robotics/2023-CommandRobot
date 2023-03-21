// ! Read
// https://www.st.com/resource/en/user_manual/um2555-vl53l1x-ultra-lite-driver-multiple-zone-implementation-stmicroelectronics.pdf

#include <Arduino.h>
#include <VL53L1X.h>
#include <Wire.h>

// Must be pin 5 or 6
constexpr uint8_t outPin = 5;
constexpr uint8_t validPin = 7;
constexpr uint16_t maxDistance = 2000;

static uint16_t distance = 0;

static VL53L1X sensor;

void setup() {
    Serial.begin(115200);
    Wire.begin();

    pinMode(outPin, OUTPUT);
    pinMode(validPin, OUTPUT);

    digitalWrite(validPin, HIGH);

    sensor.setTimeout(500);
    if (!sensor.init()) {
        Serial.println("Failed to detect and initialize sensor!");
        while (1)
            ;
    }

    // Use long distance mode and allow up to 50000 us (50 ms) for a
    // measurement. You can change these settings to adjust the performance of
    // the sensor, but the minimum timing budget is 20 ms for short distance
    // mode and 33 ms for medium and long distance modes. See the VL53L1X
    // datasheet for more information on range and timing limits.
    sensor.setDistanceMode(VL53L1X::Long);
    sensor.setMeasurementTimingBudget(50000);
    sensor.setROISize(6u, 8u);
    sensor.setROICenter(199u);

    // Start continuous readings at a rate of one measurement every 50 ms (the
    // inter-measurement period). This period should be at least as long as the
    // timing budget.
    sensor.startContinuous(50);
}

void loop() {
    sensor.read();

    Serial.print("range: ");
    Serial.print(sensor.ranging_data.range_mm);
    Serial.print("\tstatus: ");
    Serial.print(
        VL53L1X::rangeStatusToString(sensor.ranging_data.range_status));
    Serial.print("\tpeak signal: ");
    Serial.print(sensor.ranging_data.peak_signal_count_rate_MCPS);
    Serial.print("\tambient: ");
    Serial.print(sensor.ranging_data.ambient_count_rate_MCPS);

    Serial.println();

    if (sensor.ranging_data.range_status == VL53L1X::RangeValid) {
        auto range = sensor.ranging_data.range_mm;
        distance = range <= maxDistance ? range : distance;
        analogWrite(outPin, map(distance, 0, maxDistance, 0, 255));
        digitalWrite(validPin, LOW);
    } else {
        digitalWrite(validPin, HIGH);
    }
}