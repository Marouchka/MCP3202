/*
 * MCP3202 library -- 12-bit ADC
 * Author: Marie Hébert
 * Last modified: 2019-03-31
 * 
 * Based on the Mcp3208 library by Patrick Rogalla
 * 
 */

 #ifndef MCP3202_H
 #define MCP3202_H

#include <Arduino.h>
#include <SPI.h>

class MCP3202
{
  public: 
  // Setup SPI outside of this class
  MCP3202(uint16_t vRef, uint8_t ssPin); // Constructor, ssPin mode must be already defined as output

  // Public member variables
  static const uint8_t kResBits = 12; // ADC resolution in bits
  static const uint16_t kRes = (1 << kResBits); // ADC resolution
  
  // To create command 
  enum CHANNEL
  {
    SINGLE_0 = 0b1010, // Single channel 0
    SINGLE_1 = 0b1110, // Single channel 1
    DIFF_PN = 0b0010, // Differential CH0 + CH1 -
    DIFF_NP = 0b0110, // Differential CH0 - CH1 0
  };

  // Public member functions
  uint16_t read(CHANNEL ch);
  uint16_t toAnalog(uint16_t rawBits);
  uint16_t toDigital(uint16_t vValue);

  protected: 
  // Private member variables
  /**
   * Defines 16 bit SPI data. The structure implements an easy
   * access to each byte.
   */
  union twoBytes 
  {
    uint16_t value; // Value
    struct 
    {
      uint8_t loByte;  // Low byte
      uint8_t hiByte;  // High byte
    };
  };

  uint16_t vRef; // [mV]
  uint8_t ssPin;
  uint32_t SPISpeed;
  
  // Private member functions
  twoBytes createCmd(CHANNEL ch);
  uint16_t transfer(twoBytes cmd);
  
};
 #endif // MCP3202_H
