//////////////////////////////////////////////////////////////////////////////
// 
// Ficha Prática 7 - Comunicação I2C (One Master-Multiple Slave)
// 
// MASTER como controlador do sistema embebido.
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


//Funcionamento MASTER
//Solicita a leitura da temperatura ao SLAVE_B (sensor).
//Atualiza o SLAVE_A (LED RGB) com base na tabela fornecida.
//Imprime o estado no Serial Monitor.

#include <Wire.h>

#define SLAVE_A 0x03
#define SLAVE_B 0x05

void setup() {
  Serial.begin(9600);
  Serial.println("MASTER iniciado!");
  Wire.begin();
}

void loop() {
  int temperature = 0; // Variável para armazenar a temperatura
  int colorCode = 0;   // Variável para armazenar o código da cor

  //////////////////////////////
  // MASTER REQUESTS TEMPERATURE FROM SLAVE_B
  //////////////////////////////
  Wire.requestFrom(SLAVE_B, 2); // Solicita 2 bytes do Slave B
  if (Wire.available() >= 2) {
    temperature = Wire.read() << 8 | Wire.read(); // Lê dois bytes (MSB e LSB)
    Serial.print("Temperatura lida: ");
    Serial.println(temperature);
  }

  //////////////////////////////
  // DETERMINAR CÓDIGO DE COR BASEADO NA TEMPERATURA
  //////////////////////////////
  if (temperature > 75 && temperature <= 125) {
    colorCode = 0;
  } else if (temperature > 50 && temperature <= 75) {
    colorCode = 1;
  } else if (temperature > 25 && temperature <= 50) {
    colorCode = 2;
  } else if (temperature > 10 && temperature <= 25) {
    colorCode = 3;
  } else if (temperature > 0 && temperature <= 10) {
    colorCode = 4;
  } else if (temperature > -20 && temperature <= 0) {
    colorCode = 5;
  } else if (temperature > -40 && temperature <= -20) {
    colorCode = 6;
  }

  //////////////////////////////
  // MASTER SENDS COLOR CODE TO SLAVE_A
  //////////////////////////////
  Wire.beginTransmission(SLAVE_A);
  Wire.write(colorCode); // Envia o código de cor para o Slave A
  Wire.endTransmission();
  Serial.print("Codigo de cor enviado ao Slave A: ");
  Serial.println(colorCode);

  delay(1000); // Aguarda 1 segundo
}
