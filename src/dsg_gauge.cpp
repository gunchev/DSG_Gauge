#include <Arduino.h>
#include "dsg_gauge.h"

DSG_RawGauge::DSG_RawGauge(uint8_t pin) : m_v(512), m_pin(pin) {
}

DSG_RawGauge::~DSG_RawGauge(void) {
    end();
}

void DSG_RawGauge::begin(void) {
    pinMode(m_pin, INPUT);
}

void DSG_RawGauge::end(void) {
    pinMode(m_pin, INPUT);
}

void DSG_RawGauge::read(void) {
    m_v = analogRead(m_pin);
}

// Copy-paste of the built-in map, saves 4 bytes RAM and 222 bytes ROM for me
inline static int8_t imap(int16_t x, int16_t in_min, int16_t in_max, int8_t out_min, int8_t out_max) {
    return int8_t((int32_t)(x - in_min) * (out_max - out_min) / (in_max - in_min)) + out_min;
}

DSG_Gauge::DSG_Gauge(uint8_t pin) :
        DSG_RawGauge(pin), m_cmin(-127), m_c0(0), m_cmax(127), m_rmin(0), m_r0(511), m_rmax(1023) {
}

void DSG_Gauge::calibrate(uint16_t rmin, uint16_t r0, uint16_t rmax, int8_t cmin, int8_t c0, int8_t cmax) {
    // Check if raw values are rmin < r0 < rmax < 1024
#ifdef DSG_DEBUG
    if ((rmin >= r0) or (r0 >= rmax) or (rmax > 1023)) {
        while (true) {
            Serial.println(F("Invalid calibration!"));
            delay(16000);
        }
    }
#endif

    // Because map(imap) truncates, adjust the raw calibration values to simulate rounding.
    // This consumes CPU once, doing it in imap will consume CPU every time.
    m_rmin = rmin - ((r0 - rmin) / (c0 > cmin ? c0 - cmin : cmin - c0) >> 1);
    m_r0 = r0 - ((rmax - rmin) / (cmax > cmin ? cmax - cmin : cmin - cmax) >> 1);
    m_rmax = rmax - ((rmax - r0) / (cmax > c0 ? cmax - c0 : c0 - cmax) >> 1);

    m_cmin = cmin;
    m_c0 = c0;
    m_cmax = cmax;
#ifdef DSG_DEBUG
    Serial.print(rmin);
    Serial.print(' ');
    Serial.print(r0);
    Serial.print(' ');
    Serial.print(rmax);

    Serial.print(' ');
    Serial.print('-');
    Serial.print('>');
    Serial.print(' ');

    Serial.print(m_rmin);
    Serial.print(' ');
    Serial.print(m_r0);
    Serial.print(' ');
    Serial.println(m_rmax);

    for (int16_t i = 0; i < 1024; ++i) {
        Serial.print(i);
        Serial.print('=');
        if (i <= m_rmin) {
            Serial.println(m_cmin);
        } else if (i >= m_rmax) {
            Serial.println(m_cmax);
        } else  if (i <= m_r0) {
            Serial.println(imap(i, m_rmin, m_r0, m_cmin, m_c0));
        } else {
            Serial.println(imap(i, m_r0, m_rmax, m_c0, m_cmax));
        }
    }
#endif
}

int8_t DSG_Gauge::get(void) const {
    if (static_cast<int16_t>(m_v) <= m_rmin) {
        return m_cmin;
    }
    if (static_cast<int16_t>(m_v) >= m_rmax) {
        return m_cmax;
    }
    if (static_cast<int16_t>(m_v) <= m_r0) {
        return imap(static_cast<int16_t>(m_v), m_rmin, m_r0, m_cmin, m_c0);
    }
    return imap(static_cast<int16_t>(m_v), m_r0, m_rmax, m_c0, m_cmax);
}
