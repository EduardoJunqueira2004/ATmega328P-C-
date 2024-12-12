//////////////////////////////////////////////////////////////////////////////
// Ficha Prática 1 - Introdução ATMEGA (IO Digitais)
//////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////
// Ficha Prática 2 I/O Ports - Exercícios:
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////
/////////////////////////////////////////////////
// 2-Exercícios-Ex4/4.1/4.2/4.3:https://www.tinkercad.com/things/kXCliC6pTGZ/editel?returnTo=&sharecode=-5Vc2yZbSfX6BdozGw1r8wBOjnCNOKhxcriuQY_GebU
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//Exercício 2 Pretende-se implementar um sistema que conte o número 
//de vezes que o botão de pressão é pressionado. 
//Sempre que o contador sofrer alterações o valor deve ser enviado
//via porta série para consola. O LED deve indicar se o valor do 
//contador é par ou ímpar (desligado quando o valor do 
//contador é par e ligado quando o valor do contador é ímpar).
//Exercício 4 Repita o exercício 2 recorrendo agora à utilização de interrupções externas.
////////////////////////////////////////////////////////////////// 

// Pinos ARDUINO
#define Led1 13  // LED1 pin
#define Btn1 2   // BTN1 pin (INT0)

// Variáveis globais
volatile int Cnt = 0;  // Contador de pressões de botão (volatile para uso em ISR)
bool lastBtnState = LOW;

//////////////////////////////////////////////////////////////////
// Configuração das interrupções externas
//////////////////////////////////////////////////////////////////
void initExternalInt() 
{ 
  noInterrupts();  // Desativa as interrupções globais

  // Configura a interrupção para reagir em borda de subida (LOW para HIGH)
  bitWrite(EICRA, ISC01, 1);  // Configura o bit ISC01 para INT0 (borda de subida)
  bitWrite(EICRA, ISC00, 1);  // Configura o bit ISC00 para INT0 (borda de subida)

  // Habilita a interrupção INT0
  bitWrite(EIMSK, INT0, 1);   // Habilita interrupção INT0 (pino 2)

  interrupts();  // Reabilita as interrupções globais
}

//////////////////////////////////////////////////////////////////
// ISR para interrupção INT0 (Botão de pressão no pino 2)
//////////////////////////////////////////////////////////////////
ISR(INT0_vect) 
{
  // Incrementa o contador quando o botão for pressionado
  Cnt++;  // Apenas incrementa o contador
}

//////////////////////////////////////////////////////////////////
// Função para atualizar o estado do LED com base no contador
//////////////////////////////////////////////////////////////////
void MudaLed() 
{
  if (Cnt % 2 == 0) 
  {
    digitalWrite(Led1, LOW);  // Se o contador é par, desliga o LED
    Serial.println("LED apagado (contador par).");
  } 
  else 
  {
    digitalWrite(Led1, HIGH);  // Se o contador é ímpar, acende o LED
    Serial.println("LED aceso (contador impar).");
  }
}

//////////////////////////////////////////////////////////////////
// Função Setup - Inicialização
//////////////////////////////////////////////////////////////////
void setup() 
{
  // Configura o pino do LED e o pino do botão como OUTPUT e INPUT, respetivamente
  Serial.begin(9600);
  pinMode(Led1, OUTPUT);
  pinMode(Btn1, INPUT);

  // Configura a interrupção externa para o botão (INT0)
  initExternalInt();

  Serial.println("In setup function");
}

//////////////////////////////////////////////////////////////////
// Função Loop - Controle baseado no contador
//////////////////////////////////////////////////////////////////
void loop() 
{
  // Envia o valor do contador para o monitor série
  Serial.print("Contador: ");
  Serial.println(Cnt);

  // Atualiza o estado do LED com base no valor do contador
  MudaLed();

  delay(1000);  // Espera 1 segundo entre as leituras
}