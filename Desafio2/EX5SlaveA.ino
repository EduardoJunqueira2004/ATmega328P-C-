///////////////////////////////////////////////////////////////////////////////
//Nome:Eduardo Junqueira nº30241 / Jorge Pereira n30248
//Link:https://www.tinkercad.com/things/99C1HY0QTAX-copy-of-fp7ex2i2c/editel?sharecode=gO0-ID300ixwQR2dVuoIaDRYobs0GQI1uYfxE-y7HdU&sharecode=5EtyftTD0uhpJwhqBXyiUcsJprZATlMpiZ9YUlsDpGY
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////
// Visor de 7 segmentos  SLAVE A
/////////////////////////////////////////////////

#define dW digitalWrite

#define	A 13
#define	B 12
#define	C 11
#define	D 10
#define	E 9
#define	F 8
#define	G 7
#define	DP 6

//Botões

#define btn1 2
#define btn2 3


#define MAX_DISPLAY_NUM 9

int NumG=0;

void nointerrpts() {
  noInterrupts(); // Desactiva todas as Interrupts
}




void num2Display(int num) {
  

  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  digitalWrite(DP, LOW);

  // Verifica se o número está dentro do intervalo permitido.
  if (num >= 0 && num <= MAX_DISPLAY_NUM) {
    switch (num) 
    {
      case 0:
        digitalWrite(A, HIGH);
        digitalWrite(B, HIGH);
        digitalWrite(C, HIGH);
        digitalWrite(D, HIGH);
        digitalWrite(E, HIGH);
        digitalWrite(F, HIGH);
        digitalWrite(G, LOW);
        delay(750);
        break;

      case 1:
        digitalWrite(B, HIGH);
        digitalWrite(C, HIGH);
        delay(750);
        break;
      case 2:
        digitalWrite(A, HIGH);
        digitalWrite(B, HIGH);
        digitalWrite(G, HIGH);
        digitalWrite(E, HIGH);
        digitalWrite(D, HIGH);
      delay(750);
        break;

      case 3:
        digitalWrite(A, HIGH);
        digitalWrite(B, HIGH);
        digitalWrite(G, HIGH);
        digitalWrite(C, HIGH);
        digitalWrite(D, HIGH);
      delay(750);
        break;
      case 4:
        digitalWrite(F, HIGH);
        digitalWrite(G, HIGH);
        digitalWrite(B, HIGH);
        digitalWrite(C, HIGH);
      delay(750);
        break;

      case 5:
        digitalWrite(A, HIGH);
        digitalWrite(F, HIGH);
        digitalWrite(G, HIGH);
        digitalWrite(C, HIGH);
        digitalWrite(D, HIGH);
      delay(750);
        break;

      case 6:
        digitalWrite(A, HIGH);
        digitalWrite(F, HIGH);
        digitalWrite(G, HIGH);
        digitalWrite(C, HIGH);
        digitalWrite(D, HIGH);
        digitalWrite(E, HIGH);
      delay(750);
        break;

      case 7:
        digitalWrite(A, HIGH);
        digitalWrite(B, HIGH);
        digitalWrite(C, HIGH);
      delay(750);
        break;

      case 8:
        digitalWrite(A, HIGH);
        digitalWrite(B, HIGH);
        digitalWrite(C, HIGH);
        digitalWrite(D, HIGH);
        digitalWrite(E, HIGH);
        digitalWrite(F, HIGH);
        digitalWrite(G, HIGH);
      delay(750);
        break;

      case 9:
        digitalWrite(A, HIGH);
        digitalWrite(B, HIGH);
        digitalWrite(C, HIGH);
        digitalWrite(F, HIGH);
        digitalWrite(G, HIGH);
      delay(750);
        break;
        
    }
  }
}


void initTimer1Int(float Hz) {
  // Configuração da interrupção associada ao timer 1
        noInterrupts();    	// Turn off global interrupts

  // Configurar o TIMER1 para operar em modo CTC (Clear Timer on Compare Match)
  TCCR1A = 0; // Modo normal de operação
  TCCR1B = 0; // Limpe os bits de configuração
  TCNT1 = 0; // Inicialize o contador com zero
  
  // Comparação (OCR1A) Para Hz calculo:
  // para obter uma interrupção a cada 1 segundo (1 Hz)
  // Frequência do TIMER1 = Clock / (Prescaler * (1 + OCR1A))
  // Clock = 16 MHz (frequência do Arduino Uno)
  // Prescaler = 256 (definido para gerar 1 Hz)
  // OCR1A = (16000000 / (256 * 1)) - 1 = 62500
  
  if (Hz == 0.5) {
     OCR1A = 124999; //Calculado para 0.5Hz
    //OCR1A = (16000000 / (256 * 0.5)) - 1 =  124999
    
  } else if (Hz == 1 ) {
  OCR1A = 62499;  // Calculado para 1Hz
		 //OCR1A = (16000000 / (256 * 1)) - 1 =62499
   
  } else if (Hz ==2 ) {
    OCR1A = 31249;  // Calculado para 2Hz 
    //OCR1A = (16000000 / (256 * 2)) - 1 = 31249
    
  }
 
  

  // Configurar o TIMER1 para operar com o prescaler 256
  // Isso define a frequência em 1 Hz
  TCCR1B |= (1 << WGM12); // Ative o modo CTC
  TCCR1B |= (1 << CS12);  // Defina o prescaler 256
  TIMSK1 |= (1 << OCIE1A); // Ative a interrupção de comparação A

  interrupts();      	// Turn on global interrupts
}

// Rotina de interrupção do TIMER1
//Esta rotina é configurada para ser chamada automaticamente pelo Timer1 quando dá Overflow, o que ocorre a cada 1 segundo. 
ISR(TIMER1_COMPA_vect) {
  // (Re)inicialização do Timer 1
  TCNT1 = 0;  //para todas as frequências

  // Atualização do valor do display
  num2Display(NumG);

  // Incrementa o contador e garante que ele varie ciclicamente entre 0 e 9
  NumG = (NumG + 1) % 10;
}


/////////////////////////////////////////////////
// Função Setup - Inicialização
/////////////////////////////////////////////////

void setup() {				// Função de Inicialização 
    Serial.begin(9600);		// Serial Port Baud Rate = 9600 bps
	
  	
  

  // Inicialize os pinos do display como saída
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(DP, OUTPUT);
  // Inicialização dos inputs
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  
  Serial.begin(9600);
  noInterrupts(); // Desativa todas as Interrupções
}


/////////////////////////////////////////////////
// Função Loop 
/////////////////////////////////////////////////
void loop() {				
  
 // Apenas a rotina de refrescamento do display é executada neste loop

  for (int num = 0; num <= MAX_DISPLAY_NUM; num++) {
    num2Display(num);
    initTimer1Int(1); // 1HZ
    //initTimer1Int(0.5);  // 0.5Hz
   //initTimer1Int(2);  // 2Hz

  // Quando o botão 1 é pressionado, reinicia o contador
  if (digitalRead(btn1) == HIGH) {
    NumG = 0;
    delay(750);
  }
    // Quando for pressionado o botão 2, faz
    while (digitalRead(btn2) == HIGH) {
      NumG = num;
      delay(750);
    }
  }
}