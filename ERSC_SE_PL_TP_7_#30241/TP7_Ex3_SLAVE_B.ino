//////////////////////////////////////////////////////////////////////////////
// 
// Ficha Prática 7 - Comunicação I2C (One Master-Multiple Slave)
// 
// SLAVE B  Sensor de Temperatura análogo ao implementado no trabalho prático #6 com interface I2C.
//
// Pinos I2C no Arduino Uno:
//  - SDA: pino A4
//	- SCL: pino A5
//	- GND: Não esquecer de ligar o sinal de ground, entre os diversos dispositivos
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////
// Ficha Prática 7: Parte2 2. USART (Universal Synchronous and Asynchronous serial Receiver and Transmitter ) - Exercicio
//////////////////////////////////////////////////////////

/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ficha Prática nº7 -Exercícios Link Trinkercad https://www.tinkercad.com/things/iFjLh4HEk9L-fp7ex3/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard%2Fdesigns%2Fcircuits&sharecode=jz1HU2eme5ely30q9XJKjydbkYkDpWCekUKAM7acLro
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//Exercício 3
//Pretende-se implementar um sistema de controlo de temperatura usando 
//computação por passagem de mensagens de acordo com os seguintes requisitos:
//• O controlador (MASTER) deverá a cada segundo efetuar um request ao sensor
//(SLAVE_B) e de seguida atuar no led RGB (SLAVE_A).
//Tabela 1:
//75 < T ≤ 125 //0 - LED_RGB(255, 0, 0)
//50 < T ≤ 75 1 - LED_RGB(255, 127, 0)
//25 < T ≤ 50 2 - LED_RGB(255, 255, 0)
//10 < T ≤ 25 3 - LED_RGB( 0, 255, 0)
//0 < T ≤ 10 4 - LED_RGB( 0, 0, 255)
//-20 < T ≤ 0 5 - LED_RGB( 75, 0, 130)
//-40 < T ≤ -20 6 - LED_RGB(143, 0, 255)
//Mediante o valor da temperatura medida pelo sensor (SLAVE_B) o controlador (MASTER)
//do sistema deverá actuar o led RGB (SLAVE_A) de acordo com a tabela 1.;
//• O sensor de temperatura (SLAVE_B) deverá efetuar uma medida de temperatura sempre
//on request;
//• O controlador deverá imprimir o estado do sistema na consola sempre que for lido
//um novo valor do sensor (SLAVE_B) e sempre que o led RGB (SLAVE_A) for atualizado.;
//////////////////////////////////////////////////////////////////////////////

//Funcionamento do SLAVE_B:
//Retorna a temperatura simulada quando o Master solicita
#include <Wire.h>

#define SLAVE_B 0x05

void setup() {
  Serial.begin(9600);
  Serial.println("SLAVE_B iniciado!");
  Wire.begin(SLAVE_B);
  Wire.onRequest(requestEvent);
}

void loop() {
  // Loop vazio
}

void requestEvent() {
  int temperature = random(-40, 126); // Gera temperatura aleatória
  Wire.write(highByte(temperature)); // Envia o byte mais significativo
  Wire.write(lowByte(temperature));  // Envia o byte menos significativo
  Serial.print("Temperatura enviada: ");
  Serial.println(temperature);
}
