#ifndef L_MCP4725_H
#define L_MCP4725_H

#include <Arduino.h>
#include <Wire.h>

class L_MCP4725 {
public:
    L_MCP4725(uint8_t I2C_ADDR = 0x60);
    
    void setValueRange(uint16_t minValue = 0, uint16_t maxValue = 4095);
    void setOutputValue(uint16_t value);
    uint16_t getOutputValue();
    void setEEPROM(uint16_t value);
    
    void setVoltageRange(float minVoltage = 0.0, float maxVoltage = 5.0);
    void setOutputVoltage(float voltage);
    float getOutputVoltage();
    void setEEPROM(float voltage);


private:
    float _minVoltage = 0.0;
    float _maxVoltage = 5.0;
    uint8_t _I2C_ADDR;
    uint16_t voltageToDAC(float voltage);
    float dacToVoltage(uint16_t value);
    void writeDAC(uint16_t value);
};

#endif
