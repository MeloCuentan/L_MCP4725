#include "L_MCP4725.h"

L_MCP4725::L_MCP4725(uint8_t I2C_ADDR) {
  _I2C_ADDR = I2C_ADDR;
  Wire.begin();
}
// *********************************   SECCIÓN PARA TRABAJAR CON VALORES   *********************************
void L_MCP4725::setValueRange(uint16_t minValue, uint16_t maxValue) {
  if (minValue >= maxValue || maxValue > 4095) return;
  _minVoltage = dacToVoltage(minValue);
  _maxVoltage = dacToVoltage(maxValue);
}

void L_MCP4725::setOutputValue(uint16_t value) {
  if (value > 4095) value = 4095;  // Asegurar que no exceda el valor máximo de 12 bits
  writeDAC(value);
}

uint16_t L_MCP4725::getOutputValue() {
  Wire.requestFrom((uint8_t)_I2C_ADDR, (uint8_t)3);  // Pedir 3 bytes a la dirección I2C

  uint32_t startMillis = millis();        // Control de tiempo para evitar bloqueo de funcionamiento
  while (Wire.available() < 3) {          // Esperar a recibir 3 bytes
    if (millis() - startMillis >= 100) {  // 100 ms timeout
      return 0;                           // Retorna 0 si no ha leído en el tiempo especificado
    }
  }

  uint8_t readCommand = Wire.read();  // Leer el primer byte (no se utilizará)
  uint8_t msb = Wire.read();          // Leer el segundo byte
  uint8_t lsb = Wire.read();          // Leer el tercer byte

  uint16_t value = ((uint16_t)msb << 4) | (lsb >> 4);  // utilizamos el msb completo y lso 4 primeros bites de lsb

  return value;
}

void L_MCP4725::setEEPROMValue(uint16_t value) {
  Wire.beginTransmission(_I2C_ADDR);
  Wire.write(0x60);                 // Comando para escribir en EEPROM y DAC
  Wire.write((value >> 4) & 0xFF);  // Enviar los 8 bits más significativos (12-bit resolution shifted by 4 bits)
  Wire.write((value << 4) & 0xF0);  // Enviar los 4 bits menos significativos con los 4 bits menos significativos del DAC a cero
  Wire.endTransmission();
}

// *********************************   SECCIÓN PARA TRABAJAR CON VOLTAJE   *********************************
void L_MCP4725::setVoltageRange(float minVoltage, float maxVoltage) {
  if (minVoltage >= maxVoltage || minVoltage < 0.0 || maxVoltage > 5.0) return;
  _minVoltage = minVoltage;
  _maxVoltage = maxVoltage;
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

void L_MCP4725::setEEPROMVoltage(float voltage) {
  uint16_t value = voltageToDAC(voltage);  // Convertimos el float a uint16_t
  setEEPROMValue(uint16_t(value));
}

// *********************************    SECCIÓN PARA FUNCIONES PRIVADAS    *********************************
void L_MCP4725::writeDAC(uint16_t value) {
  Wire.beginTransmission(_I2C_ADDR);
  Wire.write((value >> 8) & 0x0F);  // Enviar los 4 bits más significativos
  Wire.write(value & 0xFF);         // Enviar los 8 bits menos significativos
  Wire.endTransmission();
}

float L_MCP4725::dacToVoltage(uint16_t value) {
  return ((float)value * (_maxVoltage - _minVoltage) / 4095) + _minVoltage;
}

uint16_t L_MCP4725::voltageToDAC(float voltage) {
  return (uint16_t)((voltage - _minVoltage) * 4095 / (_maxVoltage - _minVoltage));
}
