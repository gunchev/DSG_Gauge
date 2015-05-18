#include <dsg_gauge.h>

#define GAUGE_PIN A0
#define RAW_MIN 16
#define RAW_0 512
#define RAW_MAX 1007
#define OUT_MIN 16
#define OUT_0 0
#define OUT_MAX -16

DSG_Gauge gauge(GAUGE_PIN); // RAM: +12 bytes, pin A0

void setup() {
    // Serial port
    Serial.begin(9600, SERIAL_8N1);  // RAM: +173 bytes

    // Gauge
    gauge.begin();
    gauge.calibrate(RAW_MIN, RAW_0, RAW_MAX, OUT_MIN, OUT_0, OUT_MAX);
}

void loop() {
    gauge.read(); // RAM: +2 bytes
    Serial.print(F("Gauge: raw = "));
    Serial.print(gauge.getRaw());
    Serial.print(F(", calibrated = "));
    Serial.println(gauge.get());
    delay(100);
}
