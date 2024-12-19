//////////////////////////////////////////////////////////////////////////////
// 
// Ficha Prática 7 - Comunicação I2C (One Master-Multiple Slave)
// 
// MASTER 1(Arduino Uno R3)
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


#include <Wire.h> //bliblioteca necessária para I2C

#define SLAVE_A 0x03
#define SLAVE_B 0x05

void setup() 
{
  Serial.begin(9600);  	 			// start serial for output
  Wire.begin();        		 		// join i2c bus (address optional for master)
}

void loop() 
{
  
  //////////////////////////////  
  // MASTER WRITES to SLAVE_A 
  //////////////////////////////
  Wire.beginTransmission(SLAVE_A);  // begin transmission to Slave A
  Wire.write("MASTER > SLAVE_A\n"); 
  Wire.endTransmission(); 			// stop transmitting
 
  
  //////////////////////////////
  // delay de 1 segundo 
  //////////////////////////////
  delay(1000);
  
  
  //////////////////////////////
  // MASTER READS from SLAVE_A
  //////////////////////////////
  Wire.requestFrom(SLAVE_A,17);
  while (Wire.available()) 
  {
    char c = Wire.read();
    Serial.print(c);
  }
  
  //////////////////////////////
  // delay de 1 segundo 
  //////////////////////////////
  delay(1000);
    
  //////////////////////////////  
  // MASTER WRITES to SLAVE_B 
  //////////////////////////////
  Wire.beginTransmission(SLAVE_B);    // begin transmission to Slave B
  Wire.write("MASTER > SLAVE_B\n");	  
  Wire.endTransmission(); 			   // stop transmitting
  
  
  //////////////////////////////
  // delay de 1 segundo 
  //////////////////////////////
  delay(1000);
  
  
  //////////////////////////////
  // MASTER READS from SLAVE_B
  //////////////////////////////
  Wire.requestFrom(SLAVE_B,17);
  while (Wire.available()) 
  {
    char c = Wire.read();
    Serial.print(c);
  }
  
  
  //////////////////////////////
  // delay de 1 segundo 
  //////////////////////////////
  delay(1000);

}

