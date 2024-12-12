//////////////////////////////////////////////////////////////////////////////
// Ficha Prática 4 - 1. Compreender de forma genérica a diferença entre máquinas de
//estados de Moore e Mealy.
//2. Representação de sistemas com recurso a diagramas de estados e diagramas de fluxo.
//3. Implementação de máquinas de estados no Atmega328p.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
// Ficha Prática 4:2. FSM - Exercícios - Exercício2Tudo
//////////////////////////////////////////////////////////

/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2-Exercícios-Ex2 Link Trinkercad: https:https://www.tinkercad.com/things/9PTpDIAWESh-fp42ex2/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard%2Fdesigns%2Fcircuits&sharecode=SdaURk1b76ZiaqB-g3DnPfpHSKqUinPoiKb8525BPIY
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//Exercício 2 Pretende-se implementar uma aplicação que interage com um utilizador 
//através de um LED e um botão de pressão que deverão ser configurados para operar como pinos digitais, saída e entrada, 
//respetivamente, de forma a cumprir com os seguintes requisitos de aplicação:
//• Enquanto o utilizador estiver a clicar no botão o LED deverá estar aceso, 
//caso contrário o LED deverá estar apagado.
//• Se o utilizador clicar no botão por um longo período ( > 1 segundo ), 
//o LED deverá piscar indefinidamente com um período de aproximadamente 200 ms.
//////////////////////////////////////////////////////////////////

// Definição dos pinos
const int LedPin = 13;
const int BtnPin = 2;

// Definição dos estados
enum state { ST_OFF, ST_ON, ST_BLINK };
state Estado = ST_OFF;

// Variáveis de controle
volatile bool buttonPressed = false;
volatile bool longPressDetected = false;
volatile bool timeElapsed = false;
volatile bool blinkTimeElapsed = false;

// Definições de tempo para o Timer1
const unsigned int longPressCount = 1;  // Contagem de 1 segundo para pressão longa
const unsigned int blinkCount = 200;    // Intervalo de 200 ms para piscar
volatile unsigned int timerCount = 0;

void setup() {
  // Configuração dos pinos
  pinMode(LedPin, OUTPUT);
  pinMode(BtnPin, INPUT);

  // Configuração da interrupção externa no botão
  attachInterrupt(digitalPinToInterrupt(BtnPin), btnISR, CHANGE);

  // Configuração do Timer1 com prescaler de 1024
  TCCR1A = 0;
  TCCR1B = (1 << CS12) | (1 << CS10); // Prescaler de 1024
  TIMSK1 = (1 << TOIE1); // Ativar interrupção de overflow do Timer1
  TCNT1 = 49911; // Valor inicial para 1 segundo (aproximado) com prescaler 1024

  // Porta série para debug
  Serial.begin(9600);
}

void loop() {
  // Máquina de estados
  switch (Estado) {
    case ST_OFF:
      digitalWrite(LedPin, LOW);
      break;

    case ST_ON:
      digitalWrite(LedPin, HIGH);
      if (longPressDetected) {
        Estado = ST_BLINK;
        timerCount = 0;
        TCNT1 = 63536;  // Configura o Timer1 para piscar a cada 200 ms
      }
      break;

    case ST_BLINK:
      if (blinkTimeElapsed) {
        blinkTimeElapsed = false; // Reset do flag
        digitalWrite(LedPin, !digitalRead(LedPin));  // Alterna o estado do LED
      }
      break;
  }
}

// Interrupção para detectar eventos no botão
void btnISR() {
  buttonPressed = digitalRead(BtnPin) == HIGH;

  if (buttonPressed) {
    if (Estado == ST_OFF) {
      Estado = ST_ON;
      timerCount = 0; // Reiniciar contagem para pressão longa
      TCNT1 = 49911;  // Configura Timer1 para 1 segundo
      longPressDetected = false;
    }
  } else {
    if (Estado == ST_BLINK || Estado == ST_ON) {
      Estado = ST_OFF;
    }
    longPressDetected = false;
  }
}

// Interrupção do Timer1 para controlar os tempos de 1 segundo e 200 ms
ISR(TIMER1_OVF_vect) {
  if (Estado == ST_ON) {
    timerCount++;
    if (timerCount >= longPressCount) { // 1 segundo para pressão longa
      longPressDetected = true;
      timerCount = 0;
      TCNT1 = 63536;  // Reconfigura para piscar a cada 200 ms no estado BLINK
    }
  } else if (Estado == ST_BLINK) {
    blinkTimeElapsed = true; // Define flag para alternar o LED
  }

  TCNT1 = (Estado == ST_ON) ? 49911 : 63536; // Reinicia o temporizador com o valor correto
}