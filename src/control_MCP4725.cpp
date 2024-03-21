#include "control_MCP4725.h"

MCP4725::MCP4725() {
  _minVoltage = 0.0;
  _maxVoltage = 5.0;
}

void MCP4725::begin(uint8_t I2C_ADDR) {
  _MCP4725_ADDRESS = I2C_ADDR;
  Wire.begin();
}

void MCP4725::setVoltageRange(float minVoltage, float maxVoltage) {
  _minVoltage = minVoltage;
  _maxVoltage = maxVoltage;
}

void MCP4725::setValueRange(uint16_t minValue, uint16_t maxValue) {
  _minVoltage = dacToVoltage(minValue);
  _maxVoltage = dacToVoltage(maxValue);
}

void MCP4725::setOutputVoltage(float voltage) {
  if (voltage < _minVoltage) {
    voltage = _minVoltage;
  } else if (voltage > _maxVoltage) {
    voltage = _maxVoltage;
  }

  uint16_t dacValue = voltageToDAC(voltage);

  writeDAC(dacValue);
}

void MCP4725::setOutputValue(uint16_t value) {
  if (value > 4095) {
    value = 4095;
  }
  writeDAC(value);
}

float MCP4725::getOutputVoltage() {
  uint16_t dacValue = getOutputValue();
  return dacToVoltage(dacValue);
}

uint16_t MCP4725::getOutputValue() {
  Wire.requestFrom(_MCP4725_ADDRESS, 2);
  while(Wire.available() < 2);

  uint8_t msb = Wire.read();
  uint8_t lsb = Wire.read();

  // Combinar los bytes en un valor de 12 bits
  uint16_t value = ((msb & 0x0F) << 8) | lsb;

  return value;
}

uint16_t MCP4725::voltageToDAC(float voltage) {
  return static_cast<uint16_t>((voltage / _maxVoltage) * 4095);
}

float MCP4725::dacToVoltage(uint16_t value) {
  return static_cast<float>(value) * _maxVoltage / 4095.0;
}

void MCP4725::writeDAC(uint16_t value) {
  Wire.beginTransmission(_MCP4725_ADDRESS);
  Wire.write((value >> 8) & 0xFF);
  Wire.write(value & 0xFF);
  Wire.endTransmission();
}
