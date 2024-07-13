#include "L_MCP4725.h"

L_MCP4725::L_MCP4725()
  : _I2C_ADDR(0x60), _minVoltage(0.0), _maxVoltage(5.0) {}

void L_MCP4725::begin(uint8_t I2C_ADDR) {
  _I2C_ADDR = I2C_ADDR;
  Wire.begin();
}

void L_MCP4725::setVoltageRange(float minVoltage, float maxVoltage) {
  _minVoltage = minVoltage;
  _maxVoltage = maxVoltage;
}

void L_MCP4725::setValueRange(uint16_t minValue, uint16_t maxValue) {
  // No se utiliza en esta implementación
}

void L_MCP4725::setOutputVoltage(float voltage) {
  uint16_t value = voltageToDAC(voltage);
  writeDAC(value);
}

float L_MCP4725::getOutputVoltage() {
  uint16_t value = getOutputValue();
  return dacToVoltage(value);
}

void L_MCP4725::setOutputValue(uint16_t value) {
  if (value > 4095) {
    value = 4095;  // Asegurarse de que no exceda el valor máximo de 12 bits
  }
  writeDAC(value);
}

void L_MCP4725::writeDAC(uint16_t value) {
  Wire.beginTransmission(_I2C_ADDR);
  Wire.write((value >> 8) & 0x0F);  // Enviar los 4 bits más significativos
  Wire.write(value & 0xFF);         // Enviar los 8 bits menos significativos
  Wire.endTransmission();
}

uint16_t L_MCP4725::getOutputValue() {
  Wire.requestFrom((uint8_t)_I2C_ADDR, (uint8_t)3);  // Pedir 2 bytes desde la dirección I2C

  unsigned long startMillis = millis();
  while (Wire.available() < 3) {
    if (millis() - startMillis >= 100) {  // 100 ms timeout
      return 0;
    }
  }

  uint8_t readCommand = Wire.read();  // Valor no utilizado
  uint8_t msb = Wire.read();          // Leer el primer byte
  uint8_t lsb = Wire.read();          // Leer el segundo byte

  uint16_t value = ((uint16_t)msb << 4) | (lsb >> 4);

  return value;
}

uint16_t L_MCP4725::voltageToDAC(float voltage) {
  return (uint16_t)((voltage - _minVoltage) * 4095 / (_maxVoltage - _minVoltage));
}

float L_MCP4725::dacToVoltage(uint16_t value) {
  return ((float)value * (_maxVoltage - _minVoltage) / 4095) + _minVoltage;
}
