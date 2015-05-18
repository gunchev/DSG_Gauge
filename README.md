# Arduino one axis gauge classes

Includes raw and calibrated classes.

NB: Though DSG_Gauge inherits from DSG_RawGauge they have no virtual
destructor (to save RAM) and free-ing DSG_Gauge as DSG_RawGauge will
leak memory! Allocating and freeing memory on Arduino is not too hot
subject anyways...
    https://arduino.land/FAQ/content/4/20/en/can-i-use-new-and-delete-with-arduino.html

Classes: DSG_RawGauge, DSG_Gauge

Storage: 3 bytes RAM per DSG_RawGauge and 12 bytes RAM per DSG_Gauge.
