Librería creada para controlar la salida de un MCP4525
Todos los métodos pueden trabajar con voltaje (valores float) o con RAW (valores de 0 a 4095)
Los métodos son los siguientes:
 - void begin(uint8_t I2C_ADDR = 0x60);
   Este método se inicializa el MCP. Se puede especificar la dirección I2C o dejar la que viene por defecto
   
 - void setVoltageRange(float minVoltage = 0.0, float maxVoltage = 5.0);
   Este método establece los valores mínimos y máximos de funcionamiento especificando el voltaje. Por defecto de 0V a 5V
 - void setOutputVoltage(float voltage);
   Este método establece el valor de salida especificndo el voltaje
 - float getOutputVoltage();
   Devuelve el valor en voltios de la salida
 
 - void setValueRange(uint16_t minValue = 0, uint16_t maxValue = 4095);
   Este método esablece los valores mínimos y máximos de funcionamiento con valor RAW. Por defecto de 0 a 4095
 - void setOutputValue(uint16_t value);
   Este método establece el valor de salida especificando el valor RAW
 - uint16_t getOutputValue();
   Devuelve el valor en RAW de la salida

