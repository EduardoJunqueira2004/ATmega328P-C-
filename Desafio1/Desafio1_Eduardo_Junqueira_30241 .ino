//////////////////////////////////////////////////////////////////////////////
// 
// Desafio 1 - Nov'14 2024
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// 
// Eduardo Miguel Moreira Junqueira ERSC nº30241 3ano Turma B
//
//link Trinkercad:https://www.tinkercad.com/things/8ZsrGOqhGES/editel?sharecode=yKUSDT26VXurxIqaBJqq9_jDP0zb4j1cHzf9ATJ2zOA
//////////////////////////////////////////////////////////////////////////////

// Define os pinos do display de 7 segmentos
#define A 13
#define B 12
#define C 11
#define D 10
#define E 9
#define F 8
#define G 7
#define DP 6

// Define os pinos do LED e do Botão
#define LED 3
#define BTN 2

#define dW digitalWrite //Atalho para digitalWrite
#define F_CPU 16000000  // Frequência do clock para o Timer1

// Variáveis globais
volatile bool timerFlag = false;// Flag para indicar que o temporizador terminou, começa com false 
volatile bool buttonPressed = false;
int countValue = 1;
int buttonPressCount = 0;//Contador de pressão do Button BTN
int timer1_period = 15624;//Valor inicial do temporizador quantidade que é possível ler numeros de cliques em 1 segundo

//Estados Possíveis 3
enum Estado { COUNTING, LED_ON, LED_BLINK };
Estado estadoAtual = COUNTING; //Estado inicial de contagem 

//Protótipos das funções
void setupTimer1(float frequency);
void turnOffDisplay();
void numToDisplay(int num);

//Função Setup de inicialização
void setup() {
    Serial.begin(9600);
    Serial.print("In setup function!");//Apenas para verificar se está a passar pela função de setup
    setupTimer1(1);  

    pinMode(LED, OUTPUT);   // Define o pino do LED como saída
    pinMode(BTN, INPUT);    // Define o pino do botão como entrada

 // Define os pinos do display de 7 segmentos como saída
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    pinMode(E, OUTPUT);
    pinMode(F, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(DP, OUTPUT);
// Inicializa o estado do LED que é LOW e do display
    dW(LED, LOW);
    turnOffDisplay();
    attachInterrupt(digitalPinToInterrupt(BTN), buttonISR, RISING); // Interrupção para o botão
}

//Função  Loop de  Controlo 
void loop() {

    if (timerFlag) { // Se o temporizador terminou
        timerFlag = false;// Reseta a flag

//3 estados Switch Case:
        switch (estadoAtual) {
            case COUNTING:
                if (countValue <= 5) {
                    numToDisplay(countValue);// Mostra o número no display
                    countValue++;// Decrementa o número em oredem crescente até 5
                } else {
                    estadoAtual = LED_ON;// Passa para o estado ON
                    dW(LED, HIGH);// Liga o LED
                    countValue = 4; // Reseta num para 4 segundos
                    //Falta a parte de clicar 9 vezes no botão e ele fazer Reset
                }
                break;

            case LED_ON:
                if (countValue > 0) {
                    countValue--;// Decrementa o tempo de 4 segundos
                } else {
                    Serial.print("Pressionado: ");
                    Serial.println(buttonPressCount);//Mostra o número de pressões na linha de Comandos
                    numToDisplay(buttonPressCount);// Mostra o número de pressões no display
                    estadoAtual = LED_BLINK;// Passa para o estado BLINK
                    countValue = buttonPressCount;//Guarda em countValue o valor de cliques pressionados
                }
                break;

            case LED_BLINK:
                 // Pisca o LED com uma frequência baseada em cnt
                for (int i = 0; i < buttonPressCount; i++) {
                    dW(LED, HIGH);
                    delayMicroseconds(1000000);// 100ms ligado  
                    dW(LED, LOW);
                    delayMicroseconds(1000000);  // 100ms desligado
                }
                estadoAtual = COUNTING;// Reinicia o ciclo volta para o estado inicial COUNTING
                countValue = 1;//Reinicia o valor da constante para 1
                buttonPressCount = 0;//Reinicia o valor de cliques 
                break;
        }
    }
// Interrupção externa para o botão
    if (buttonPressed) {
        buttonPressed = false;

        if (estadoAtual == COUNTING) // Verifica se o estado atual é o inicial COUNTING
        {
            countValue = 1;
        } else if (estadoAtual == LED_ON) {
            buttonPressCount++;
            if (buttonPressCount == 9) // Verifica se o botão foi pressionado 9 vezes
            {
                estadoAtual = COUNTING;
                dW(LED, LOW);
            }
        }
    }
}

void setupTimer1(float frequency) {
    noInterrupts();// Desativa interrupções
    TCCR1A = 0;// Modo normal
    TCCR1B = 0;// Limpa o registrador
    TCNT1  = 0;// Zera o contador TCNT1

// Configura o valor de comparação para o temporizador no modo CTC
    OCR1A = (F_CPU / (1024 * frequency)) - 1;

// Configura o prescaler para 1024 e modo CTC
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

// Habilita a interrupção do comparador A
    TIMSK1 |= (1 << OCIE1A);

    interrupts(); // Ativa interrupções
}

// Interrupção para o comparador A do TIMER1
ISR(TIMER1_COMPA_vect) {
    timerFlag = true;// Define a flag para indicar que o tempo expirou limpando a flag a 1
}

// Interrupção externa para o botão
void buttonISR() {
    buttonPressed = true;
}

// Desliga todos os LEDs
void turnOffDisplay() {
    dW(A, LOW);
    dW(B, LOW);
    dW(C, LOW);
    dW(D, LOW);
    dW(E, LOW);
    dW(F, LOW);
    dW(G, LOW);
}

// Exibe o número 'num' no display
void numToDisplay(int num) {
    turnOffDisplay();
    switch (num) {
        case 0: dW(A, HIGH); dW(B, HIGH); dW(C, HIGH); dW(D, HIGH); dW(E, HIGH); dW(F, HIGH); break;
        case 1: dW(B, HIGH); dW(C, HIGH); break;
        case 2: dW(A, HIGH); dW(B, HIGH); dW(D, HIGH); dW(E, HIGH); dW(G, HIGH); break;
        case 3: dW(A, HIGH); dW(B, HIGH); dW(C, HIGH); dW(D, HIGH); dW(G, HIGH); break;
        case 4: dW(B, HIGH); dW(C, HIGH); dW(F, HIGH); dW(G, HIGH); break;
        case 5: dW(A, HIGH); dW(C, HIGH); dW(D, HIGH); dW(F, HIGH); dW(G, HIGH); break;
        case 6: dW(A, HIGH); dW(C, HIGH); dW(D, HIGH); dW(E, HIGH); dW(F, HIGH); dW(G, HIGH); break;
        case 7: dW(A, HIGH); dW(B, HIGH); dW(C, HIGH); break;
        case 8: dW(A, HIGH); dW(B, HIGH); dW(C, HIGH); dW(D, HIGH); dW(E, HIGH); dW(F, HIGH); dW(G, HIGH); break;
        case 9: dW(A, HIGH); dW(B, HIGH); dW(C, HIGH); dW(D, HIGH); dW(F, HIGH); dW(G, HIGH); break;
        default: dW(G, HIGH); break;
    }
}