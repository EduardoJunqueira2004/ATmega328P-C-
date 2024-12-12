/////////////////////////////////////////////////
// Ficha Prática 3 Timers - Exercícios:
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 2-Exercícios-Ex3 https://www.tinkercad.com/things/eBVgjaalExi/editel?sharecode=BIcpQI7pW0gX0q8JKVbl-_G_sKz8ko5_JknXSqpfSew
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Ex3:Tudo
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
// Define I/0 Ports
//////////////////////////////////////////////////////////////////

// Semáforo 1 - Esquerda
#define Led1 13  // Semáforo 1 - Vermelho
#define Led2 12  // Semáforo 1 - Amarelo
#define Led3 11  // Semáforo 1 - Verde

// Semáforo 2 - Direita
#define Led4 10  // Semáforo 2 - Vermelho
#define Led5 9   // Semáforo 2 - Amarelo
#define Led6 8   // Semáforo 2 - Verde

int state = 0;  // Estado atual da FSM (Máquina de Estados Finitos)
int timerCounter = 0;  // Contador de tempo para mudar os estados

//////////////////////////////////////////////////////////////////
//Função para configurar os LEDs (Semáforos)
//////////////////////////////////////////////////////////////////

void setupLights() {
    pinMode(Led1, OUTPUT);  // Semáforo 1 - Vermelho
    pinMode(Led2, OUTPUT);  // Semáforo 1 - Amarelo
    pinMode(Led3, OUTPUT);  // Semáforo 1 - Verde

    pinMode(Led4, OUTPUT);  // Semáforo 2 - Vermelho
    pinMode(Led5, OUTPUT);  // Semáforo 2 - Amarelo
    pinMode(Led6, OUTPUT);  // Semáforo 2 - Verde

    // Inicializar os semáforos para o estado inicial
    digitalWrite(Led1, LOW);  // Semáforo 1 - Vermelho
    digitalWrite(Led2, LOW);  // Semáforo 1 - Amarelo
    digitalWrite(Led3, LOW);  // Semáforo 1 - Verde

    digitalWrite(Led4, LOW);  // Semáforo 2 - Vermelho
    digitalWrite(Led5, LOW);  // Semáforo 2 - Amarelo
    digitalWrite(Led6, LOW);  // Semáforo 2 - Verde
}

//////////////////////////////////////////////////////////////////
// Configuração do Timer 1
//////////////////////////////////////////////////////////////////

void initTimer1Int() 
{
    noInterrupts();               // Desativa interrupções globais

    TCCR1A = 0;                   // Modo Normal
    TCCR1B = 0;                   // Limpar o registo
    TCCR1B |= (1 << WGM12);       // CTC Mode (Clear Timer on Compare Match)
    TCCR1B |= (1 << CS12) | (1 << CS10);  // Prescaler de 1024

    OCR1A = 15625 - 1;            // Interrupção a cada 1 segundo (16MHz/1024)

    TIMSK1 |= (1 << OCIE1A);      // Habilita interrupção de comparação do Timer 1
    interrupts();                 // Ativa interrupções globais
}

//////////////////////////////////////////////////////////////////
// Rotina de Interrupção (ISR) para o Timer 1
//////////////////////////////////////////////////////////////////

ISR(TIMER1_COMPA_vect) 
{
    timerCounter++;  // Incrementa o contador de tempo a cada 1 segundo

    if (timerCounter >= 5) {
        state++;  // Muda de estado a cada 5 segundos
        if (state > 3) {
            state = 0;  // Volta ao estado inicial após o último estado
        }
        timerCounter = 0;  // Reinicia o contador de tempo
    }

    // Atualiza os semáforos conforme o estado
    updateLights();
}

//////////////////////////////////////////////////////////////////
// Função para atualizar os semáforos conforme o estado
//////////////////////////////////////////////////////////////////

void updateLights() 
{
    switch (state) 
    {
        case 0:  // Semáforo 1 Verde (4s) e Semáforo 2 Vermelho
            digitalWrite(Led1, LOW);   // Semáforo 1 - Vermelho
            digitalWrite(Led2, LOW);   // Semáforo 1 - Amarelo
            digitalWrite(Led3, HIGH);  // Semáforo 1 - Verde

            digitalWrite(Led4, HIGH);  // Semáforo 2 - Vermelho
            digitalWrite(Led5, LOW);   // Semáforo 2 - Amarelo
            digitalWrite(Led6, LOW);   // Semáforo 2 - Verde
            break;

        case 1:  // Semáforo 1 Amarelo (1s) e Semáforo 2 Vermelho
            digitalWrite(Led1, LOW);   // Semáforo 1 - Vermelho
            digitalWrite(Led2, HIGH);  // Semáforo 1 - Amarelo
            digitalWrite(Led3, LOW);   // Semáforo 1 - Verde

            digitalWrite(Led4, HIGH);  // Semáforo 2 - Vermelho
            digitalWrite(Led5, LOW);   // Semáforo 2 - Amarelo
            digitalWrite(Led6, LOW);   // Semáforo 2 - Verde
            break;

        case 2:  // Semáforo 1 Vermelho (4s) e Semáforo 2 Verde
            digitalWrite(Led1, HIGH);  // Semáforo 1 - Vermelho
            digitalWrite(Led2, LOW);   // Semáforo 1 - Amarelo
            digitalWrite(Led3, LOW);   // Semáforo 1 - Verde

            digitalWrite(Led4, LOW);   // Semáforo 2 - Vermelho
            digitalWrite(Led5, LOW);   // Semáforo 2 - Amarelo
            digitalWrite(Led6, HIGH);  // Semáforo 2 - Verde
            break;

        case 3:  // Semáforo 1 Vermelho (1s) e Semáforo 2 Amarelo
            digitalWrite(Led1, HIGH);  // Semáforo 1 - Vermelho
            digitalWrite(Led2, LOW);   // Semáforo 1 - Amarelo
            digitalWrite(Led3, LOW);   // Semáforo 1 - Verde

            digitalWrite(Led4, LOW);   // Semáforo 2 - Vermelho
            digitalWrite(Led5, HIGH);  // Semáforo 2 - Amarelo
            digitalWrite(Led6, LOW);   // Semáforo 2 - Verde
            break;
    }
}

//////////////////////////////////////////////////////////////////
// Função Setup - Inicialização
//////////////////////////////////////////////////////////////////

void setup() 
{
    setupLights();  // Configura os pinos dos LEDs
    initTimer1Int();  // Inicializa o Timer 1
}

//////////////////////////////////////////////////////////////////
// Função Loop - Não precisa fazer nada aqui pois a lógica está na interrupção
//////////////////////////////////////////////////////////////////

void loop() 
{
    // A lógica de controle dos semáforos ocorre na interrupção.
}
