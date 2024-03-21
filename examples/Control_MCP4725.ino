#include <Wire.h>
#include "control_MCP4725.h"

MCP4725 dac;

void setup() {
  dac.begin();

  dac.setVoltageRange(0.0, 5.0); // Valores en voltios
  //dac.setValueRange(0, 4095);  // Valores en RAW
}

void loop() {
  // Establecer el voltaje de salida a 1.5V
  dac.setOutputVoltage(1.5); // Valor en voltios
  delay(1000);


  // Obtener el valor de salida del DAC
  uint16_t dacValue = dac.getOutputValue(); // Devuelve el valor RAW
  Serial.print("Valor de salida del DAC: ");
  Serial.println(dacValue);

  // Convertir el valor del DAC a voltaje
  float outputVoltage = dac.getOutputVoltage();
  Serial.print("Voltaje de salida: ");
  Serial.println(outputVoltage);

  // Establecer el valor de salida del DAC directamente a la mitad del rango
  dac.setOutputValue(2048);
  delay(1000);
}
