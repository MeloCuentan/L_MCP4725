#include <Wire.h>
#include "L_MCP4725.h"

float VCCreal = 3.97; // idealmente son 5V

L_MCP4725 dac(0x60); // Dirección I2C predeterminada del MCP4725

void setup() {
    Serial.begin(9600);

    dac.setVoltageRange(0.0, VCCreal); // Configurar rango de voltaje
    dac.setValueRange(0, 4095);  // Configurar rango de valores DAC

    Serial.println("\nConfigurando valor del DAC a 1000");
    dac.setOutputValue(1000); // establece el valor del dac en 1000 (valor RAW)
  //  dac.setEEPROMValue(2000); // Guardamos en la eeprom el valor especificado
    delay(100); // Dar tiempo para que el valor se establezca

    uint16_t outputValue = dac.getOutputValue(); // Leer el valor del DAC
    Serial.print("Valor leído: ");
    Serial.println(outputValue); // Imprimir el valor leído
}

void loop() {
  if(Serial.available() != 0) {
    String rx = Serial.readStringUntil('\n');
    uint16_t valor = rx.toInt();
    cambiarValor(valor);
  }
}

void cambiarValor(uint16_t valor) {
  if (valor > 4095) {
    Serial.println("Valor fuera de rango");
    return;
  }
  Serial.print("Ajustar valor a :" + String(valor));
  dac.setOutputValue(valor); // Configurar valor del DAC
  Serial.print("    Valor leído: " + String(dac.getOutputValue()));

//  float valorConvertido = float(valor) / 100.0;
//  Serial.print("Ajustar voltios a :" + String(valorConvertido));
//  dac.setOutputVoltage(valorConvertido);
//  Serial.println("    Voltaje leído: " + String(dac.getOutputVoltage()));
}
