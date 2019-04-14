/*
 * MCP3202 library -- 12-bit ADC
 * Author: Marie Hébert
 * Last modified: 2019-03-31
 * 
 * Based on the Mcp3208 library by Patrick Rogalla
 * 
 */

 #include "MCP3202.h"

// Constructor
 MCP3202::MCP3202(uint16_t vRef, uint8_t ssPin)
{
  this->vRef = vRef;
  this->ssPin = ssPin; 
  
  return;
}

// Public member functions
uint16_t MCP3202::read(CHANNEL ch)
{
  return transfer(createCmd(ch));
}

uint16_t MCP3202::toAnalog(uint16_t rawBits)
{
  return (static_cast<uint32_t>(rawBits) * vRef) / (kRes - 1);
}

uint16_t MCP3202::toDigital(uint16_t vValue)
{
  return (static_cast<uint32_t>(vValue) * (kRes - 1)) / vRef;
}

// Private member functions
MCP3202::twoBytes MCP3202::createCmd(CHANNEL ch)
{
  // base command structure
  // 0b000001CCCC000000
  // C: Channel configuration bits from CHANNEL 
  return 
  {
    // Add channel to basic command structure
    // .value = static_cast<uint16_t>((0x0400 | (ch << 6)))
    .value = static_cast<uint16_t>((0x0100 | (ch << 4)))
  };
}

uint16_t MCP3202::transfer(twoBytes cmd)
{
  twoBytes valueADC;

  // Activate ADC with chip select
  digitalWrite(ssPin, LOW);

  // send first command byte
  SPI.transfer(cmd.hiByte);
  // send second command byte and receive first(msb) 4 bits
  valueADC.hiByte = SPI.transfer(cmd.loByte) & 0x0F; // or 0xF00
  // receive last(lsb) 8 bits
  valueADC.loByte = SPI.transfer(0x00);

  // deactivate ADC with slave select
  digitalWrite(ssPin, HIGH);

  return valueADC.value;
}
