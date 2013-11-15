#include "MPL115A2.h"

/*
 * Reads coefficients data.
 */
float MPL115A2Class::readCoefficients(int totalBits, int fractionalBits, int zeroPad) {
  unsigned char msb, lsb;
  
  msb = Wire.read();
  lsb = Wire.read();
  
  return ((float) ((msb << 8) + lsb) / ((long)1 << 16 - totalBits + fractionalBits + zeroPad));
}

/*
 * Initialize the data for compensation.
 */
void MPL115A2Class::begin () {
  address = 0x60;                    // I2C address of the MPL115A2 is 0x60.
  Wire.begin();
  
  Wire.beginTransmission(address);
  Wire.write(0x04);                  // Request for the coefficient data.
  Wire.endTransmission();
  
  Wire.requestFrom(address, 12);     // Request 12 bytes.
  if (Wire.available() > 11) {
    a0 = readCoefficients(16, 3, 0);
    b1 = readCoefficients(16, 13, 0);
    b2 = readCoefficients(16, 14, 0);
    c12 = readCoefficients(14, 13, 9);
    c11 = readCoefficients(11, 10, 11);
    c22 = readCoefficients(11, 10, 15);
  }
}

/*
 * Read the data.
 */
unsigned int MPL115A2Class::readAdc() {
  unsigned char msb, lsb;
  msb = Wire.read();
  lsb = Wire.read();
  
  return (((unsigned int)msb << 8) + lsb) >> 6;
} 

/* 
 * Read pressure data and return in hPa.
 */
float MPL115A2Class::read() {
  float hpa;
  Wire.beginTransmission(address);
  Wire.write(0x12);      // Start both conversions(Pressure and Temperature)
  Wire.write(0x01);
  Wire.endTransmission();

  delay(5);

  Wire.beginTransmission(address);
  Wire.write((uint8_t)0x00);  // Read pressure and temperature
  Wire.endTransmission();

  Wire.requestFrom(address, 4); // Request 4 bytes

  if(Wire.available()) {
    unsigned int Padc = readAdc();
    unsigned int Tadc = readAdc();
    float Pcomp = a0 + (b1 + c11 * Padc + c12 * Tadc) * Padc + (b2 + c22 * Tadc) * Tadc;
    hpa = Pcomp * 650 / 1023 + 500;
  }
  
  return hpa;  
}

MPL115A2Class MPL115A2;

