//////////////////////////////////////////////////////////////////////////////
// 
// Ficha Prática 7 - Comunicação I2C (One Master-Multiple Slave)
// 
// SLAVE B   3(Arduinjo Uno R3)
//
// Pinos I2C no Arduino Uno:
//  - SDA: pino A4
//	- SCL: pino A5
//	- GND: Não esquecer de ligar o sinal de ground, entre os diversos dispositivos
//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//Nome:Eduardo Junqueira nº30241 / Jorge Pereira n30248
//Link:https://www.tinkercad.com/things/99C1HY0QTAX-copy-of-fp7ex2i2c/editel?sharecode=gO0-ID300ixwQR2dVuoIaDRYobs0GQI1uYfxE-y7HdU&sharecode=5EtyftTD0uhpJwhqBXyiUcsJprZATlMpiZ9YUlsDpGY
///////////////////////////////////////////////////////////////////////////////


#include <Wire.h>
#define SLAVE_B 0x05
#define sensor A0

//Slave_B mede a humidade através do sensor de umidade do solo
void setup() {
	Serial.begin(9600);
  	Wire.begin(SLAVE_B); // Inicializa o módulo de hardware i2c como SLAVE com o endereço SLAVE_A
	Wire.onRequest(requestEvent);
    Wire.onReceive(receiveEvent);
  	pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN,HIGH);
	}

void loop() {

	}
float readtemp(){
  float volt=(analogRead(sensor) * 0.004882814);
	float temp = (volt - 0.5) * 100.0;
  return temp;
}
// requestEvent Handler that executes whenever data is requested by master
void requestEvent() {
  	int c=(int)readtemp()+40;
  Serial.println(c);
  	Wire.write(c); 
	}

// receiveEvent Handler that executes whenever data is received from the master
void receiveEvent(int numBytes) {
	digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    while (Wire.available()) {
    	char c = Wire.read();
    	Serial.print(c);
     	}
	}