
/////////////////////////////////////////////////
// Ficha Prática 2 I/O Ports - Exercícios:
/////////////////////////////////////////////////
/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////
/////////////////////////////////////////////////
// 2-Exercícios-Ex6:https://www.tinkercad.com/things/7AWxLygK3Fx/editel?sharecode=ZwdVjJUFkmUZIfTZO0cqEya2boeqTvpSjALffB8qv5k
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//Ex6:
//Exercício 6 Pretende-se implementar um sistema que dê resposta 
//a dois eventos externos com origem em dois botões de pressão (BTN0 e BTN1) 
//ligados nas entradas INT0 e INT1 do MCU. Adicionalmente deverá ser utilizado um LED 
//a funcionar como dispositivo de saída. Do ponto de vista funcional o programa apresenta 
//os seguintes requisitos de aplicação:
//Sempre que o BTN0 for pressionado o LED deverá acender, 
//incrementar um contador cnt0 e imprimir na consola o respetivo valor do contador.
//Sempre que o BTN1 for pressionado LED deverá apagar, incrementar um contador cnt1 e imprimir na consola o respetivo valor do contador.
//////////////////////////////////////////////////////////////////
#define Btn0 2  // Pino ligado a INT0
#define Btn1 3  // Pino ligado a INT1
#define Led1 13 //Led adicional ligado por mim no pino 13

// Variáveis globais para os contadores
volatile int cnt0 = 0;  // Contador para BTN0 (INT0)
volatile int cnt1 = 0;  // Contador para BTN1 (INT1)

//////////////////////////////////////////////////////////////////
// Configuração das interrupções externas
//////////////////////////////////////////////////////////////////
void initExternalInt() 
{ 
  noInterrupts();  // Desativa as interrupções globais durante a configuração

  // Configura as interrupções INT0 e INT1 para reagirem em borda de subida
  bitWrite(EICRA, ISC01, 1);  // Borda de subida para INT0 (Btn0)
  bitWrite(EICRA, ISC00, 1);  // Borda de subida para INT0 (Btn0)

  bitWrite(EICRA, ISC11, 1);  // Borda de subida para INT1 (Btn1)
  bitWrite(EICRA, ISC10, 1);  // Borda de subida para INT1 (Btn1)

  // Habilita as interrupções INT0 e INT1
  bitWrite(EIMSK, INT0, 1);   // Habilita interrupção externa INT0
  bitWrite(EIMSK, INT1, 1);   // Habilita interrupção externa INT1

  interrupts();  // Reabilita as interrupções globais
}

//////////////////////////////////////////////////////////////////
// ISR para interrupção INT0 (BTN0 pressionado)
//////////////////////////////////////////////////////////////////
ISR(INT0_vect) 
{
  cnt0++;  // Incrementa o contador para BTN0
  digitalWrite(Led1, HIGH);  // Acende o LED ao pressionar BTN0
}

//////////////////////////////////////////////////////////////////
// ISR para interrupção INT1 (BTN1 pressionado)
//////////////////////////////////////////////////////////////////
ISR(INT1_vect) 
{
  cnt1++;  // Incrementa o contador para BTN1
  digitalWrite(Led1, LOW);  // Apaga o LED ao pressionar BTN1
}

//////////////////////////////////////////////////////////////////
// Função Setup - Inicialização
//////////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);  // Configura a porta serial para comunicação
  Serial.println("In setup function");

  // Configura os botões como entrada
  pinMode(Btn0, INPUT);
  pinMode(Btn1, INPUT);
  //Defenir o Led como saída output
   pinMode(Led1, OUTPUT);

  // Inicializa as interrupções externas
  initExternalInt();
}

//////////////////////////////////////////////////////////////////
// Função Loop - Principal loop de execução
//////////////////////////////////////////////////////////////////
void loop() 
{
  // Envia o valor do contador cnt0 para o monitor série
  Serial.print("BTN0 pressionado. Contador cnt0: ");
  Serial.println(cnt0);

  // Envia o valor do contador cnt1 para o monitor série
  Serial.print("BTN1 pressionado. Contador cnt1: ");
  Serial.println(cnt1);

  // Atraso para não sobrecarregar a porta série
  delay(1000);  // Atraso de 1 segundo entre as leituras
}