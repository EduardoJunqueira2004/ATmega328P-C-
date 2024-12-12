//////////////////////////////////////////////////////////////////////////////
// Ficha Prática 4 - 1. Compreender de forma genérica a diferença entre máquinas de
//estados de Moore e Mealy.
//2. Representação de sistemas com recurso a diagramas de estados e diagramas de fluxo.
//3. Implementação de máquinas de estados no Atmega328p.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
// Ficha Prática 4:2. FSM - Exercícios - Exercício4Tudo
//////////////////////////////////////////////////////////

/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 4-Exercícios Link Trinkercad: https://www.tinkercad.com/things/9A1Fpu9iyqt-fp4ex4/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard%2Fdesigns%2Fcircuits&sharecode=RxLvz-3koOOkunzfzDcVHXkD61HFDCawsVGqKHl7Oq8
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ex4
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Definição dos pinos dos LEDs individualmente
#define Led1 3  // LED mais à esquerda
#define Led2 4
#define Led3 5
#define Led4 6
#define Led5 7
#define Led6 8  // LED mais à direita

// Array para os pinos dos LEDs em ordem
const int ledPins[6] = {Led1, Led2, Led3, Led4, Led5, Led6};

// Estados e variáveis
enum State { INIT, BLINK };
State estadoAtual = INIT;

int currentLed = 5;                 // Índice do LED que está a piscar (começa com o LED mais à direita)
volatile bool ledToggle = false;     // Controle de piscar do LED no estado BLINK
volatile bool buttonPressed = false; // Controle de pressão do botão
bool ledState = false;               // Estado atual do LED ativo

void setup() {
  Serial.begin(9600);
  
  // Configuração dos pinos dos LEDs como saídas
  for (int i = 0; i < 6; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // Apaga todos os LEDs inicialmente
  }
  pinMode(Btn, INPUT); // Configuração do botão como entrada

  // Configuração do Timer1 com prescaler de 1024
  TCCR1A = 0;
  TCCR1B = (1 << CS12) | (1 << CS10); // Prescaler de 1024
  TIMSK1 = (1 << TOIE1); // Ativar interrupção de overflow do Timer1
  TCNT1 = 49911; // Valor inicial para gerar overflow a cada 500 ms (aproximado)
  
  attachInterrupt(digitalPinToInterrupt(Btn), btnISR, FALLING); // Interrupção no botão (pressionado)
}

void loop() {
  switch (estadoAtual) {
    case INIT:
      // Fase de inicialização: acende cada LED de esquerda para a direita
      for (int i = 0; i < 6; i++) {
        digitalWrite(ledPins[i], HIGH);
        delay(500);              // Aguarda 500 ms antes de passar para o próximo LED
        digitalWrite(ledPins[i], LOW);
      }
      estadoAtual = BLINK;       // Após inicialização, muda para o estado de piscar
      currentLed = 5;            // Começa a piscar com o LED mais à direita
      break;

    case BLINK:
      // Piscar o LED atual a cada 500 ms
      if (ledToggle) {
        ledToggle = false;   // Reset do flag
        ledState = !ledState; // Alterna o estado do LED
        digitalWrite(ledPins[currentLed], ledState);
      }

      // Verifica se o botão foi pressionado para mudar o LED que pisca
      if (buttonPressed) {
        buttonPressed = false; // Reset do flag
        digitalWrite(ledPins[currentLed], LOW); // Apaga o LED atual

        // Move o LED que pisca para a esquerda
        currentLed--;

        // Se o LED que pisca alcançou o mais à esquerda, recomeça da direita
        if (currentLed < 0) {
          currentLed = 5;
        }

        Serial.print("Mudando para o LED: ");
        Serial.println(currentLed);
        
        // Reinicia o estado de piscar para o novo LED
        ledState = false;
      }
      break;
  }
}

// Interrupção do Timer1 para controlar o tempo de piscar (500 ms)
ISR(TIMER1_OVF_vect) {
  ledToggle = true;  // Define o flag para alternar o estado do LED
  TCNT1 = 49911; // Reinicia o temporizador para 500 ms
}

// Interrupção do botão com debounce
void btnISR() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  // Debounce simples: só processa se passaram mais de 200 ms desde a última interrupção
  if (interruptTime - lastInterruptTime > 200) {
    buttonPressed = true; // Marca que o botão foi pressionado
    lastInterruptTime = interruptTime; // Atualiza o tempo da última interrupção
  }
}