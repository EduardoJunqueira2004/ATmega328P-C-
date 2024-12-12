/////////////////////////////////////////////////
// Ficha Prática 3 Timers - Exercícios:
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 2-Exercícios-Ex1 
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Ex2:https://www.tinkercad.com/things/9Og04U7c4pQ-fp3ex1/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard&sharecode=DvNESbSVaNM3Z1w3JFShTlbJHbDQNtC0V0dli9kfqMk
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
// Define I/0 Ports
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
// Define I/0 Ports
//////////////////////////////////////////////////////////////////

#define dW digitalWrite

#define	A 13 
#define	B 12
#define	C 11
#define	D 10
#define	E 9
#define	F 8
#define	G 7
#define	DP 6
#define Btn1 2  // Botão para aumentar intervalo de tempo
#define Btn2 3  // Botão para diminuir intervalo de tempo

int cnt = 0;
int timeInterval = 500; // Intervalo inicial de 500ms

////////////////////////////////////////////////////////////////////
// Função para escrever no display de 7 segmentos
////////////////////////////////////////////////////////////////////

void write2Display(int num) 
{
    const bool digits[10][7] = {
        {1, 1, 1, 1, 1, 1, 0},  // 0
        {0, 1, 1, 0, 0, 0, 0},  // 1
        {1, 1, 0, 1, 1, 0, 1},  // 2
        {1, 1, 1, 1, 0, 0, 1},  // 3
        {0, 1, 1, 0, 0, 1, 1},  // 4
        {1, 0, 1, 1, 0, 1, 1},  // 5
        {1, 0, 1, 1, 1, 1, 1},  // 6
        {1, 1, 1, 0, 0, 0, 0},  // 7
        {1, 1, 1, 1, 1, 1, 1},  // 8
        {1, 1, 1, 1, 0, 1, 1}   // 9
    };
    
    dW(A, digits[num][0]);
    dW(B, digits[num][1]);
    dW(C, digits[num][2]);
    dW(D, digits[num][3]);
    dW(E, digits[num][4]);
    dW(F, digits[num][5]);
    dW(G, digits[num][6]);
}

////////////////////////////////////////////////////////////////////
// Configuração do Timer 1 para diferentes intervalos de tempo
////////////////////////////////////////////////////////////////////

void initTimer1Int() 
{
    noInterrupts();               // Desativa interrupções globais

    TCCR1A = 0;                   // Modo Normal
    TCCR1B = 0;                   // Limpar o registo
    TCCR1B |= (1 << WGM12);       // CTC Mode (Clear Timer on Compare Match)

    if (timeInterval == 500) {
        // Intervalo de 500ms
        TCCR1B |= (1 << CS12);    // Prescaler de 256
        OCR1A = 31250 - 1;        // 16MHz/256/500ms
    } 
    else if (timeInterval == 200) {
        // Intervalo de 200ms
        TCCR1B |= (1 << CS12);    // Prescaler de 256
        OCR1A = 12500 - 1;        // 16MHz/256/200ms
    } 
    else if (timeInterval == 2000) {
        // Intervalo de 2000ms (2 segundos)
        TCCR1B |= (1 << CS12);    // Prescaler de 256
        OCR1A = 125000 - 1;       // 16MHz/256/2000ms
    }

    TCNT1 = 0;                    // Inicializa o contador
    TIMSK1 |= (1 << OCIE1A);      // Habilita interrupção de comparação do Timer 1
    interrupts();                 // Ativa interrupções globais
}

////////////////////////////////////////////////////////////////////
// Rotina de interrupção do Timer 1
////////////////////////////////////////////////////////////////////

ISR(TIMER1_COMPA_vect) 
{
    TCNT1 = 0;   // Reinicializar o valor do contador para evitar overflow
    cnt++;       // Incrementar o contador
    if (cnt > 9) // Reseta o contador após 9
    {
        cnt = 0;
    }
    write2Display(cnt); // Atualiza o display com o valor do contador
}

////////////////////////////////////////////////////////////////////
// Função Setup - Inicialização
////////////////////////////////////////////////////////////////////

void setup() 
{
    Serial.begin(9600);    // Inicializa a comunicação serial

    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    pinMode(E, OUTPUT);
    pinMode(F, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(DP, OUTPUT);
    
    pinMode(Btn1, INPUT_PULLUP); // Botão 1 para aumentar o tempo
    pinMode(Btn2, INPUT_PULLUP); // Botão 2 para diminuir o tempo

    initTimer1Int();       // Inicializa o Timer 1 com o intervalo de tempo atual
}

////////////////////////////////////////////////////////////////////
// Função Loop - Teste de diferentes tempos e mostrar no Serial
////////////////////////////////////////////////////////////////////

void loop() 
{
    // Verifica o estado dos botões para mudar o intervalo de tempo
    if (digitalRead(Btn1) == LOW) {
        if (timeInterval == 200) {
            timeInterval = 500;  // Aumenta para 500ms
        } else if (timeInterval == 500) {
            timeInterval = 2000; // Aumenta para 2000ms
        }
        delay(200);              // Debounce
        Serial.print("Tempo mudou para: ");
        Serial.print(timeInterval);
        Serial.println("ms");
        initTimer1Int();         // Reconfigura o Timer com o novo intervalo de tempo
    }

    if (digitalRead(Btn2) == LOW) {
        if (timeInterval == 2000) {
            timeInterval = 500;  // Diminui para 500ms
        } else if (timeInterval == 500) {
            timeInterval = 200;  // Diminui para 200ms
        }
        delay(200);              // Debounce
        Serial.print("Tempo mudou para: ");
        Serial.print(timeInterval);
        Serial.println("ms");
        initTimer1Int();         // Reconfigura o Timer com o novo intervalo de tempo
    }
}