#ifndef _DSG_GAUGE_H_
#define _DSG_GAUGE_H_

#include <stdint.h>

/**
 * One axis raw gauge
 *
 * Storage: 3 bytes SRAM.
 */
class DSG_RawGauge {
public:
    DSG_RawGauge(uint8_t pin);

    ~DSG_RawGauge(void);

    /**
     * Enable the gauge (setup).
     *
     * Sets up the pin as INPUT.
     */
    void begin(void);

    /*
     * Disable the gauge, automatically called in the destructor.
     *
     * Sets up the pin as INPUT.
     */
    void end(void);

    /**
     * Read the current value and store it.
     */
    void read(void);

    /**
     * Get the last raw value read.
     */
    inline uint16_t getRaw(void) const __attribute__ ((pure)) {
        return m_v;  // The raw value
    }

protected:
    // Data (2 bytes, 1*2)
    uint16_t m_v; // The last value

    // Setup (1 bytes, 1*1)
    uint8_t m_pin; // The analog pin on which the gauge is connected
};

/**
 * One axis calibrated gauge
 *
 * Storage: 12 bytes SRAM.
 *
 * Returns raw uint16_t or int8_t calibrated values.
 * The calibration includes minimum, zero and maximum for input and output.
 * The output can be reversed - input of 0 can return up to 127 and 1023 can return -128.
 */
class DSG_Gauge: public DSG_RawGauge {
public:
    DSG_Gauge(uint8_t pin);

    /**
     * Setup the calibration data.
     *
     * If not used the defaults are for 10 bit ADC: 0, 511, 1023, -127, 0, 127.
     */
    void calibrate(uint16_t rmin, uint16_t r0, uint16_t rmax, int8_t cmin, int8_t c0, int8_t cmax);

    /**
     * Get the cooked/calibrated value.
     *
     * This method is "slow", uses division and multiplication, try to call it only once.
     */
    int8_t get(void) const __attribute__ ((pure));

protected:
    // 3 bytes from DsgRawGauge

    // Calibration (9 bytes, 3*1 + 3*2), = 12, helps align m_rmin, m_r0 and m_rmax (does not matters in 8 bit...)
    int8_t m_cmin;  // Cooked minimum
    int8_t m_c0;    // Cooked zero
    int8_t m_cmax;  // Cooked maximum

    int16_t m_rmin; // Raw minimum
    int16_t m_r0;   // Raw zero
    int16_t m_rmax; // Raw maximum
};

#endif /* _DSG_GAUGE_H_ */
