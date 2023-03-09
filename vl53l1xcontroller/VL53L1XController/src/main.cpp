// ! Read
// https://www.st.com/resource/en/user_manual/um2555-vl53l1x-ultra-lite-driver-multiple-zone-implementation-stmicroelectronics.pdf

#include <Arduino.h>
#include <SPI.h>
#include <VL53L1X.h>
#include <Wire.h>

constexpr uint8_t TRANSACTION_CONTINUE = 0;
constexpr uint8_t TRANSACTION_START = 1;

static volatile double distance = 0.0;
static volatile bool sendDataFlag = false;
static volatile uint8_t curBytePos = 0;

static VL53L1X sensor;

// SPI interrupt routine
ISR(SPI_STC_vect) {
    uint8_t rec = SPDR;

    // master starting a new transaction; reset to first byte
    if (rec == TRANSACTION_START) {
        sendDataFlag = true;
        curBytePos = 0;
        SPDR = *((uint8_t*)(&distance));
        return;
    }

    if (!sendDataFlag) {
        SPDR = 0;   // send nothing if inactive
        return;
    }

    // send the next byte
    SPDR = *((uint8_t*)(&distance + curBytePos));
    // stop after 8 bytes
    if (++curBytePos >= sizeof(double)) {
        curBytePos = 0;
        sendDataFlag = false;
    }
}

void setup() {
    Serial.begin(115200);
    Wire.begin();
    pinMode(MISO, OUTPUT);
    // pull line high by default
    digitalWrite(MISO, HIGH);
    // turn on SPI in slave mode
    SPCR |= _BV(SPE);
    // turn on interrupts
    SPI.attachInterrupt();

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
        noInterrupts();
        distance = (double)range;
        interrupts();
    }
}