/////////////////////////////////////////////////
// Ficha Prática 3 Timers - Exercícios:
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 2-Exercícios-Tudo
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Ex2:https://www.tinkercad.com/things/0vl9TjLQDS6/editel?sharecode=JcYqctOhepBMNDyzBpu8fwbU9itWlLZWC4nlYnTJxFk
//////////////////////////////////////////////////////////////////////////////
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
#define Btn1 2  // Botão para aumentar a frequência
#define Btn2 3  // Botão para diminuir a frequência

int cnt = 0;
int frequency = 1; // Frequência inicial de 1 Hz

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
// Configuração do Timer 1 para diferentes frequências
////////////////////////////////////////////////////////////////////

void initTimer1Int() 
{
    noInterrupts();               // Desativa interrupções globais

    TCCR1A = 0;                   // Modo Normal
    TCCR1B = 0;                   // Limpar o registo
    TCCR1B |= (1 << WGM12);       // CTC Mode (Clear Timer on Compare Match)

    if (frequency == 1) {
        // Frequência de 1 Hz
        TCCR1B |= (1 << CS12);    // Prescaler de 256
        OCR1A = 62500 - 1;        // 16MHz/256/1Hz
    } 
    else if (frequency == 2) {
        // Frequência de 2 Hz
        TCCR1B |= (1 << CS12);    // Prescaler de 256
        OCR1A = 31250 - 1;        // 16MHz/256/2Hz
    } 
    else if (frequency == 0) {
        // Frequência de 0.5 Hz
        TCCR1B |= (1 << CS12);    // Prescaler de 256
        OCR1A = 125000 - 1;       // 16MHz/256/0.5Hz
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
    
    pinMode(Btn1, INPUT_PULLUP); // Botão 1 para aumentar a frequência
    pinMode(Btn2, INPUT_PULLUP); // Botão 2 para diminuir a frequência

    initTimer1Int();       // Inicializa o Timer 1 com a frequência atual
}

////////////////////////////////////////////////////////////////////
// Função Loop - Teste de diferentes frequências e mostrar no Serial
////////////////////////////////////////////////////////////////////

void loop() 
{
    // Verifica o estado dos botões para mudar a frequência
    if (digitalRead(Btn1) == LOW) {
        if (frequency == 0) {
            frequency = 1;  // Aumenta para 1Hz
        } else if (frequency == 1) {
            frequency = 2;  // Aumenta para 2Hz
        }
        delay(200);              // Debounce
        Serial.print("Frequência mudou para: ");
        Serial.print(frequency == 1 ? "1 Hz" : (frequency == 2 ? "2 Hz" : "0.5 Hz"));
        Serial.println();
        initTimer1Int();         // Reconfigura o Timer com a nova frequência
    }

    if (digitalRead(Btn2) == LOW) {
        if (frequency == 2) {
            frequency = 1;  // Diminui para 1Hz
        } else if (frequency == 1) {
            frequency = 0;  // Diminui para 0.5Hz
        }
        delay(200);              // Debounce
        Serial.print("Frequência mudou para: ");
        Serial.print(frequency == 1 ? "1 Hz" : (frequency == 2 ? "2 Hz" : "0.5 Hz"));
        Serial.println();
        initTimer1Int();         // Reconfigura o Timer com a nova frequência
    }
}
