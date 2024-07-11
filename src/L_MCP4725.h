#ifndef L_MCP4725_H
#define L_MCP4725_H

#include <Wire.h>

//#define MCP4725_ADDRESS 0x60 // Dirección predeterminada del MCP4725

class MCP4725 {
  public:
    MCP4725();
    void begin(uint8_t I2C_ADDR = 0x60);
    
    void setVoltageRange(float minVoltage = 0.0, float maxVoltage = 5.0);
    void setOutputVoltage(float voltage);
    float getOutputVoltage();

    void setValueRange(uint16_t minValue = 0, uint16_t maxValue = 4095);
    void setOutputValue(uint16_t value);
    uint16_t getOutputValue();

  private:
    float _minVoltage;
    float _maxVoltage;
    uint8_t _MCP4725_ADDRESS;
    uint16_t voltageToDAC(float voltage);
    float dacToVoltage(uint16_t value);
    void writeDAC(uint16_t value);
};

#endif
