//////////////////////////////////////////////////////////////////////////////
// Ficha Prática 4 - 1. Compreender de forma genérica a diferença entre máquinas de
//estados de Moore e Mealy.
//2. Representação de sistemas com recurso a diagramas de estados e diagramas de fluxo.
//3. Implementação de máquinas de estados no Atmega328p.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
// Ficha Prática 4:2. FSM - Exercícios - Exercício3Tudo
//////////////////////////////////////////////////////////

/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 3-Exercícios Link Trinkercad: https://www.tinkercad.com/things/2QZQ446yxne/editel?sharecode=Wc65unOZd03-FTnc9q0IGJfAbgnWbPXWu3JFiBimsco
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ex3
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pinos ARDUINO
#define LedR 13 // LED Vermelho
#define LedY 12 // LED Amarelo
#define LedG 11 // LED Verde
#define Btn1 2  // Botão on/off
#define Btn2 3  // Botão de excesso de velocidade

// Definição dos estados
enum state { ST_OFF, ST_GREEN, ST_YELLOW, ST_RED };
state Estado = ST_OFF;

// Variáveis de controlo de estado e temporização
volatile bool yellowBlinkToggle = false; // Para piscar o LED amarelo no estado OFF
volatile bool transitionFlag = false;    // Flag para transições entre estados
unsigned int timerDuration = 0;          // Controle de tempo em cada estado

void setup() {
  // Configuração dos pinos
  pinMode(LedR, OUTPUT);
  pinMode(LedY, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(Btn1, INPUT);
  pinMode(Btn2, INPUT);

  // Configuração do Timer1
  TCCR1A = 0;                            // Modo normal
  TCCR1B = (1 << CS12) | (1 << CS10);    // Prescaler de 1024
  TIMSK1 = (1 << TOIE1);                 // Ativar interrupção de overflow do Timer1

  // Porta serial para debug
  Serial.begin(9600);
}

void loop() {
  // Máquina de estados
  switch (Estado) {
    case ST_OFF:
      // Piscar o LED amarelo a cada 250 ms
      if (yellowBlinkToggle) {
        yellowBlinkToggle = false;
        digitalWrite(LedY, !digitalRead(LedY)); // Alterna o LED amarelo
      }

      // Transição para ST_GREEN quando Btn1 é pressionado
      if (digitalRead(Btn1) == HIGH) {
        Estado = ST_GREEN;
        digitalWrite(LedY, LOW); // Apaga o LED amarelo
        digitalWrite(LedG, HIGH); // Liga o LED verde
        Serial.println("Controle de velocidade ligado");
      }
      break;

    case ST_GREEN:
      // Manter o LED verde aceso
      digitalWrite(LedG, HIGH);
      digitalWrite(LedY, LOW);
      digitalWrite(LedR, LOW);

      // Transição para ST_YELLOW quando Btn2 é pressionado (excesso de velocidade)
      if (digitalRead(Btn2) == HIGH) {
        Estado = ST_YELLOW;
        timerDuration = 1000; // Configura para 1 segundo
        TCNT1 = 49911; // Valor inicial para aproximadamente 1 segundo com prescaler 1024
        transitionFlag = false;
        digitalWrite(LedG, LOW);
        digitalWrite(LedY, HIGH);
        Serial.println("Excesso de velocidade detectado, transição para amarelo");
      }

      // Transição para ST_OFF se Btn1 for pressionado
      if (digitalRead(Btn1) == HIGH) {
        Estado = ST_OFF;
        digitalWrite(LedG, LOW); // Apaga o LED verde
        Serial.println("Controle de velocidade desligado");
      }
      break;

    case ST_YELLOW:
      // Manter o LED amarelo aceso por 1 segundo
      if (transitionFlag) {
        Estado = ST_RED;
        timerDuration = 3000; // Configura para 3 segundos
        TCNT1 = 49911; // Valor inicial para 3 segundos
        transitionFlag = false;
        digitalWrite(LedY, LOW);
        digitalWrite(LedR, HIGH);
        Serial.println("Transição para vermelho");
      }
      break;

    case ST_RED:
      // Manter o LED vermelho aceso por 3 segundos
      if (transitionFlag) {
        Estado = ST_GREEN;
        digitalWrite(LedR, LOW);
        digitalWrite(LedG, HIGH);
        transitionFlag = false;
        Serial.println("Transição para verde");
      }
      break;
  }
}

// Interrupção do Timer1 para controlar os tempos de transição
ISR(TIMER1_OVF_vect) {
  // Controle para piscar o LED amarelo no estado OFF
  if (Estado == ST_OFF) {
    yellowBlinkToggle = true;
    TCNT1 = 64536; // Configura para 250 ms
  } 
  // Controle de transição de estados
  else if (Estado == ST_YELLOW || Estado == ST_RED) {
    // Decrementa o tempo restante
    if (timerDuration > 0) {
      timerDuration -= 250; // Intervalo de 250 ms
      if (timerDuration <= 0) {
        transitionFlag = true; // Sinaliza que o tempo para transição foi atingido
      }
    }
    TCNT1 = 64536; // Configura para 250 ms
  }
}