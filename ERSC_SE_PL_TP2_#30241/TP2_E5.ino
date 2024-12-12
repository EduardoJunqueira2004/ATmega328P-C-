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
// 2-Exercícios-Ex5:https://www.tinkercad.com/things/bJjgKpjSOK4/editel?returnTo=&sharecode=YZHjQ46rhB5SXg8qsPCWupANyXQQyBn-JnzMhKjCxFE
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//Ex5:Crie uma cópia do circuito utilizado no Exercício 3 e implemente a alínea 3.4 recorrendo agora à utilização de interrupções externas.
//As interrupções externas são mais eficientes, seguras e robustas 
//porque permitem que o microcontrolador responda imediatamente a eventos sem desperdiçar ciclos de processamento, 
//enquanto o polling é menos eficiente e pode resultar em tempos de resposta inconsistentes. 
//Em sistemas mais complexos ou críticos, as interrupções são a escolha preferida.
//Como pude verificar no exercício anterior vi que havia um certo delay ao executar o pooling pois clicava
//no botão  e não acontecia nada tinha de clicar várias vezes para continuar o contador.
//Neste Exercício 4 agora verifiquei que é instântanio e mais eficiente,agora percebo o porquê que
//a utilização de Interrupções Externas é importante e melhor que a utilização do Polling.
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

//////////////////////////////////////////////////////////////////
//Configuração das interrupções externas
//////////////////////////////////////////////////////////////////
void initExternalInt() 
{ 
  noInterrupts();  // Desativa as interrupções globais durante a configuração

  // Configura as interrupções INT0 e INT1 para reagirem em borda de subida
  EICRA |= (1 << ISC01) | (1 << ISC00);  // Borda de subida para INT0 (Btn1)
  EICRA |= (1 << ISC11) | (1 << ISC10);  // Borda de subida para INT1 (Btn2)

  // Habilita as interrupções INT0 e INT1
  EIMSK |= (1 << INT0);  // Habilita interrupção externa INT0
  EIMSK |= (1 << INT1);  // Habilita interrupção externa INT1


interrupts(); // Turn on global interrupts
}

///////////////////////////////////////////////////
//as rotinas de serviço de interrupção (ISRs) não precisam ser chamadas diretamente no loop(). 
//Na verdade, elas são chamadas automaticamente pelo hardware do microcontrolador 
//sempre que ocorre o evento que as despoleta, como uma mudança de estado em um pino associado a uma interrupção.
///////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
// ISR para interrupção INT0 (Botão de reset, Btn1)
//////////////////////////////////////////////////////////////////
ISR(INT0_vect) 
{
  
  cnt = 0;  // Reseta o contador ao pressionar Btn1
  
}

//////////////////////////////////////////////////////////////////
// ISR para interrupção INT1 (Botão de pausa/incremento, Btn2)
//////////////////////////////////////////////////////////////////
ISR(INT1_vect) 
{
  isPaused = !isPaused;  // Alterna entre pausado e não pausado ao pressionar Btn2
}

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
  
initExternalInt();
  
Serial.begin(9600);		// Serial Port Baud Rate = 9600 bps
  Serial.print("In setup fuction!");
  

}
/////////////////////////////////////////////////
// Função Loop - Loop de controlo coma utilização de Interrupções
/////////////////////////////////////////////////

void loop() 
{
  // Se não estiver pausado, incrementa o contador
  if (!isPaused) {
    cnt++;
    if (cnt > 9) {
      cnt = 0;  // Garante que o contador não passe de 9
    }
  }

  // Exibe o número atual no display
  num2Display(cnt);

  delay(1000);  // Atraso de 1 segundo
}
 
