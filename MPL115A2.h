#ifndef MPL115A2_h
#define MPL115A2_h

#include <Arduino.h>
#include <Wire.h>

class MPL115A2Class {
  public:
    void begin();
    float read();
  private:
    int address;                      // I2C address
    float a0, b1, b2, c12, c11, c22;  // Compensation data
    float readCoefficients(int totalBits, int fractionalBits, int zeroPad);
    unsigned int readAdc();
};

extern MPL115A2Class MPL115A2;
#endif /* MPL115A2_h */
