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
// 2-Exercícios-Ex3:https://www.tinkercad.com/things/7fqAiNAWgNx/editel?sharecode=F0QoIKEfLd_SP1NA7Pb6cX360Q2Jp74kun32B9J8LHc
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//Ex3/3.1/3.2/3.3/3.4:
//Exercício 3 Pretende-se implementar um Device Driver 
//para um display de 7-Segmentos: O display de sete segmentos 
//é um tipo de mostrador de baixo custo, sendo utilizado amplamente
//em sistemas eletrónicos. Este tipo de dispositivo é normalmente 
//utilizado para interface com o utilizador permitindo exibir 
//informação alfanumérica. Internamente é composto por um conjunto 
//de LEDs sendo que a cada um deles corresponde a um segmento. 
//A sua ligação a um MCU pode ser feita utilizando portas GPIO do 
//MCU como saída permitindo escrever no display através da 
//manipulação do valor lógico dos diferentes pinos 
//ligados a cada um dos LEDs do display.
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// 
// Ficha Prática 2 7 Seg. Display
//
// A - Protótipos das funções Arduino C a utilizar para efectuar Digital I/O:
//   void pinMode(int pin_ID, bool INPUT/OUTPUT) - define a direcção do fluxo INPUT/OUTPUT
//   void digitalWrite(int pin_ID, bool LOW/HIGH)- escreve na porta digital pin_ID o valor LOW/HIGH
//   int digitalRead(int pin_ID) - lê o valor lógico (LOW/HIGH) da porta digital pin_ID
// 
// B - Datasheet do ATMEGA328
//  http://ww1.microchip.com/downloads/en/devicedoc/atmel-42735-8-bit-avr-microcontroller-atmega328-328p_datasheet.pdf
//
//////////////////////////////////////////////////////////////////////////////

/////////////////////////////////
//
//• Após a chamada da função num2display, o valor passado como 
//parâmetro de entrada num deverá surgir no mostrador.
//• Se o valor de num for menor do que 0 e maior do que 9, 
//apenas o segmento G deverá ficar aceso.
//• O device driver deverá ser implementado a partir do seguinte 
//protótipo de função:
//
/////////////////////////////////
#define dW digitalWrite

#define	A 13 
#define	B 12
#define	C 11
#define	D 10
#define	E 9
#define	F 8
#define	G 7
#define	DP 6
#define Btn1 2
#define Btn2 3

int cnt=0;
bool isPaused = false;
bool lastBtn2State = HIGH;  // Estado anterior do botão de pausa

void num2Display(char num)
{
   //Defenir como default tudo a Low
  digitalWrite(A,LOW);
  digitalWrite(B,LOW);
  digitalWrite(C,LOW);
  digitalWrite(D,LOW);
  digitalWrite(E,LOW);
  digitalWrite(F,LOW);
  digitalWrite(G,LOW);
  digitalWrite(DP,HIGH);
  // Verifica se o número está dentro do intervalo permitido.
  if(num>=1 && num<=9){
  switch(num)
  {
    case 0:
    digitalWrite(A, HIGH);
  	digitalWrite(B, HIGH);
  	digitalWrite(C, HIGH);
  	digitalWrite(D, HIGH);
  	digitalWrite(E, LOW);
  	digitalWrite(F, HIGH);
  	digitalWrite(G, LOW);
    digitalWrite(DP, LOW);
    break;
    case 1:
    digitalWrite(A, LOW);
  	digitalWrite(B, HIGH);
  	digitalWrite(C, HIGH);
  	digitalWrite(D, LOW);
  	digitalWrite(E, LOW);
  	digitalWrite(F, LOW);
  	digitalWrite(G, LOW);
  	digitalWrite(DP, LOW);
    break;
    case 2:
    digitalWrite(A, HIGH);
  	digitalWrite(B, HIGH);
  	digitalWrite(C, LOW);
  	digitalWrite(D, HIGH);
  	digitalWrite(E, HIGH);
  	digitalWrite(F, LOW);
  	digitalWrite(G, HIGH);
  	digitalWrite(DP, LOW);
  	break;
    case 3:
    digitalWrite(A, HIGH);
  	digitalWrite(B, HIGH);
  	digitalWrite(C, HIGH);
  	digitalWrite(D, HIGH);
  	digitalWrite(E, LOW);
  	digitalWrite(F, LOW);
  	digitalWrite(G, HIGH);
  	digitalWrite(DP, LOW);
    break;
    case 4:
    digitalWrite(A, LOW);
  	digitalWrite(B, HIGH);
  	digitalWrite(C, HIGH);
  	digitalWrite(D, LOW);
  	digitalWrite(E, LOW);
  	digitalWrite(F, HIGH);
  	digitalWrite(G, HIGH);
  	digitalWrite(DP, LOW);
    break;
    case 5:
    digitalWrite(A, HIGH);
  	digitalWrite(B, LOW);
  	digitalWrite(C, HIGH);
  	digitalWrite(D, HIGH);
  	digitalWrite(E, LOW);
  	digitalWrite(F, HIGH);
  	digitalWrite(G, HIGH);
  	digitalWrite(DP, LOW);
    break;
    case 6:
    digitalWrite(A, HIGH);
  	digitalWrite(B, LOW);
  	digitalWrite(C, HIGH);
  	digitalWrite(D, HIGH);
  	digitalWrite(E, HIGH);
  	digitalWrite(F, HIGH);
  	digitalWrite(G, HIGH);
  	digitalWrite(DP, LOW);
    break;
    case 7:
    digitalWrite(A, HIGH);
  	digitalWrite(B, HIGH);
  	digitalWrite(C, HIGH);
  	digitalWrite(D, LOW);
  	digitalWrite(E, LOW);
  	digitalWrite(F, LOW);
  	digitalWrite(G, LOW);
  	digitalWrite(DP, LOW);
    break;
    case 8:
    digitalWrite(A, HIGH);
  	digitalWrite(B, HIGH);
  	digitalWrite(C, HIGH);
  	digitalWrite(D, HIGH);
  	digitalWrite(E, HIGH);
  	digitalWrite(F, HIGH);
  	digitalWrite(G, HIGH);
  	digitalWrite(DP, LOW);
    break;
    case 9:
    digitalWrite(A, HIGH);
  	digitalWrite(B, HIGH);
  	digitalWrite(C, HIGH);
  	digitalWrite(D, LOW);
  	digitalWrite(E, LOW);
  	digitalWrite(F, HIGH);
  	digitalWrite(G, HIGH);
  	digitalWrite(DP, LOW);
    break;
  }
  }else
  {
    // Se o número não for válido, acende apenas o segmento G
      //Segmento G

  digitalWrite(A,HIGH);
  digitalWrite(C,HIGH);
  digitalWrite(D,HIGH);
  digitalWrite(E,HIGH);
  digitalWrite(F,HIGH);
  digitalWrite(G,HIGH);

  }
  
  
}

//////////////////////////////////////////////////////////////////
//Função Setup - Inicialização  é invocada quando o micro-controlador inicia
// A função setup() é executada apenas uma vez, após a placa ser alimentada ou ser feito um reset.
//////////////////////////////////////////////////////////////////
void setup() 				
{
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(DP, OUTPUT);
  pinMode(Btn1, INPUT);
  pinMode(Btn2, INPUT);
  

  
Serial.begin(9600);		// Serial Port Baud Rate = 9600 bps
  Serial.print("In setup fuction!");
  

}
/////////////////////////////////////////////////
// Função Loop - Loop de controlo coma utilização de Polling
/////////////////////////////////////////////////
void loop() {
  // Leitura do estado dos botões
  int btn1State = digitalRead(Btn1);  // Botão de reset
  int btn2State = digitalRead(Btn2);  // Botão de pausa/incremento

  // Se o botão 1 for pressionado, reseta o contador
  if (btn1State == HIGH) 
  {
    cnt = 0;
  }
// Verifica se houve mudança de estado no botão de pausa (Btn2)
  if (btn2State == HIGH && lastBtn2State == LOW) {
    isPaused = !isPaused;  // Alterna entre pausado e não pausado
  }
  lastBtn2State = btn2State;  // Atualiza o estado anterior

  
  // Se não estiver pausado, incrementa o contador
  if (!isPaused) 
  {
    cnt++;
    if (cnt > 9) 
    {  // Garante que o contador não passe de 9
      cnt = 0;
    }
  	}

  // Exibe o número atual no display
  num2Display(cnt);

  delay(1000);  // Atraso de 1 segundo
}
 
