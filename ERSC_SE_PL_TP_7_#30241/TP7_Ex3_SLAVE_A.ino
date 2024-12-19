//////////////////////////////////////////////////////////////////////////////
// 
// Ficha Prática 7 - Comunicação I2C (One Master-Multiple Slave)
// 
// SLAVE A  LED RGB  atuador 
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


//Funcionamento do SLAVE_A:
//Atualiza o LED RGB com base no comando recebido do Master.
#include <Wire.h>

#define SLAVE_A 0x03

#define PIN_R 6 // Pino PWM para o Vermelho
#define PIN_G 5 // Pino PWM para o Verde
#define PIN_B 3 // Pino PWM para o Azul

void setup() {
  Serial.begin(9600);
  Serial.println("SLAVE_A iniciado!");
  Wire.begin(SLAVE_A);
  Wire.onReceive(receiveEvent);

  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
}

void loop() {
  // Loop vazio
}

void receiveEvent(int numBytes) {
  int colorCode = Wire.read(); // Lê o código de cor enviado pelo Master
  Serial.print("Codigo de cor recebido: ");
  Serial.println(colorCode);

  switch (colorCode) {
    case 0: setRGB(255, 0, 0); break;    // Vermelho
    case 1: setRGB(255, 127, 0); break; // Laranja
    case 2: setRGB(255, 255, 0); break; // Amarelo
    case 3: setRGB(0, 255, 0); break;   // Verde
    case 4: setRGB(0, 0, 255); break;   // Azul
    case 5: setRGB(75, 0, 130); break;  // Índigo
    case 6: setRGB(143, 0, 255); break; // Violeta
    default: setRGB(0, 0, 0); break;    // Apaga o LED
  }
}

void setRGB(int r, int g, int b) {
  analogWrite(PIN_R, r);
  analogWrite(PIN_G, g);
  analogWrite(PIN_B, b);
}
