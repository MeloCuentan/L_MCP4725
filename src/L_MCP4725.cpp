#include "L_MCP4725.h"

L_MCP4725::L_MCP4725(uint8_t I2C_ADDR) {
  _I2C_ADDR = I2C_ADDR;
  Wire.begin();
}

void L_MCP4725::setVoltageRange(float minVoltage, float maxVoltage) {
  if (minVoltage > 0.0 && minVoltage < maxVoltage) _minVoltage = minVoltage;
  if (maxVoltage > minVoltage && maxVoltage < 5.0) _maxVoltage = maxVoltage;
}

void L_MCP4725::setValueRange(uint16_t minValue, uint16_t maxValue) {
  if (minValue >= 0 && minValue < maxValue) _minVoltage = dacToVoltage(minValue);
  if (maxValue > minValue && maxValue < 4095) _maxVoltage = dacToVoltage(maxValue);
}

void L_MCP4725::setOutputVoltage(float voltage) {
  if (voltage > _maxVoltage) voltage = _maxVoltage;
  uint16_t value = voltageToDAC(voltage);
  writeDAC(value);
}

float L_MCP4725::getOutputVoltage() {
  uint16_t value = getOutputValue();
  return dacToVoltage(value);
}

void L_MCP4725::setOutputValue(uint16_t value) {
  if (value > 4095)  value = 4095;  // Asegurarse de que no exceda el valor máximo de 12 bits
  writeDAC(value);
}

void L_MCP4725::writeDAC(uint16_t value) {
  Wire.beginTransmission(_I2C_ADDR);
  Wire.write((value >> 8) & 0x0F);  // Enviar los 4 bits más significativos
  Wire.write(value & 0xFF);         // Enviar los 8 bits menos significativos
  Wire.endTransmission();
}

void L_MCP4725::writeDACEEPROM(uint16_t value) {
  if (value > 4095) return;
  Wire.beginTransmission(_I2C_ADDR);
  Wire.write(0x60);                 // Comando para escribir en EEPROM
  Wire.write((value >> 8) & 0x0F);  // Enviar los 4 bits más significativos
  Wire.write(value & 0xFF);         // Enviar los 8 bits menos significativos
  Wire.endTransmission();

  delay(50);  // Esperar a que la EEPROM termine de escribir (típicamente toma 25 ms)
}

void L_MCP4725::writeVoltageEEPROM(float voltage) {
  if (voltage > 5.0) return;
  uint16_t value = voltageToDAC(voltage); // Convertir el voltaje a un valor DAC
  writeDACEEPROM(value); // Guardar el valor DAC en la EEPROM
}

uint16_t L_MCP4725::getOutputValue() {
  Wire.requestFrom((uint8_t)_I2C_ADDR, (uint8_t)3);  // Pedir 2 bytes desde la dirección I2C

  uint32_t startMillis = millis();
  while (Wire.available() < 3) {
    if (millis() - startMillis >= 100) {  // 100 ms timeout
      return 0;                           // o algún valor de error apropiado
    }
  }

  uint8_t readCommand = Wire.read();  // Leer el primer byte. Este no se utilziará
  uint8_t msb = Wire.read();          // Leer el segundo byte
  uint8_t lsb = Wire.read();          // Leer el tercer byte

  uint16_t value = ((uint16_t)msb << 4) | (lsb >> 4); // utilizamos el msb completo y lso 4 primeros bites de lsb

  return value;
}

uint16_t L_MCP4725::voltageToDAC(float voltage) {
  return (uint16_t)((voltage - _minVoltage) * 4095 / (_maxVoltage - _minVoltage));
}

float L_MCP4725::dacToVoltage(uint16_t value) {
  return ((float)value * (_maxVoltage - _minVoltage) / 4095) + _minVoltage;
}
