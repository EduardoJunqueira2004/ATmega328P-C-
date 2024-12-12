//////////////////////////////////////////////////////////////////////////////
// Ficha Prática 4 - 1. Compreender de forma genérica a diferença entre máquinas de
//estados de Moore e Mealy.
//2. Representação de sistemas com recurso a diagramas de estados e diagramas de fluxo.
//3. Implementação de máquinas de estados no Atmega328p.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
// Ficha Prática 4:2. FSM - Exercícios - Exercício1.11.2/1.3
//////////////////////////////////////////////////////////

/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 2-Exercícios-Ex1 Máquinas de estados de Moore implementação Link Trinkercad: https://www.tinkercad.com/things/bUjS41nFVa4/editel?sharecode=u6SfBsBOuWxP5RAdU2CX7yWe5HJ4HNBrnjdKTTUAKOg
/////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ex1.1 A figura 4 corresponde á implementação de máquinas de estados de Moore  e o código que se segue é a sua execução!
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//Ex1:Exercício 1 Pretende-se implementar uma aplicação composta por um led verde (LedG),
//um led vermelho (LedR) e um botão de pressão (Btn) que cumpra com os seguintes requisitos:
//• O botão é usado para acender e apagar o led verde (acende se estiver apagado e apaga se estiver aceso).
//• O led verde não pode estar aceso durante períodos superiores a 6 segundos e é automaticamente desligado ao fim deste tempo.
//• O led vermelho deve acender quando o led verde estiver ligado à mais de 4 segundos para indicar que se o utilizador não pressionar o botão para desligar o led verde este será desligado automaticamente quando passarem 6 segundos desde o momento em que foi ligado.
//Na figura 3 encontram-se representados 2 possíveis diagramas de estados da aplicação, ambos consideram a existência de 3 estados distintos. Analise as soluções apresentadas e verifique se estão de acordo com os requisitos da aplicação.
//De seguida são apresentados os fluxogramas de cada um dos diagramas de estados.
//Ambas as soluções tiram partido do TIMER 1 para controlar os intervalos de tempo entre as transições de estado e da utilização de interrupções externas para detetar eventos com origem no botão de pressão.
//////////////////////////////////////////////////////////////////

// Pinos ARDUINO
int LedR = 13;  
int LedG = 11;  
int Btn = 2;  

enum state {ST_OFF, ST_ON, ST_WARN};

state Estado;

void setup() // Função de Inicialização 
{   
    // Porta série para debug
    Serial.begin(9600);

    // Configuração dos pinos
    pinMode(LedR, OUTPUT);
    pinMode(LedG, OUTPUT);
    pinMode(Btn, INPUT);
    
    // Inicialização do estado
    Estado = ST_OFF;
    
    // Configuração da interrupção externa no pino do botão
    attachInterrupt(digitalPinToInterrupt(Btn), btnISR, RISING);
    
    // Configuração inicial do Timer1 com prescaler 1024
    TCCR1A = 0; // Modo normal
    TCCR1B = (1 << CS12) | (1 << CS10); // Prescaler de 1024
    TIMSK1 = (1 << TOIE1); // Ativar interrupção de overflow do Timer1

    setTimerFor4Seconds(); // Inicia o temporizador para 4 segundos
    interrupts();    // Ativar interrupções globais  
}

void loop() // Loop de Controlo
{
    // Estado OFF
    if (Estado == ST_OFF)
    {
        digitalWrite(LedG, LOW);
        digitalWrite(LedR, LOW);
        Serial.println("Estado: OFF");
    }
    // Estado ON
    else if (Estado == ST_ON)
    {
        digitalWrite(LedG, HIGH);
        digitalWrite(LedR, LOW);
        Serial.println("Estado: ON");
    }
    // Estado WARN
    else if (Estado == ST_WARN)
    {
        digitalWrite(LedG, HIGH);
        digitalWrite(LedR, HIGH);
        Serial.println("Estado: WARN");
    }
    delay(100); // Pequeno atraso para evitar saturação na porta série
}

void btnISR() // Interrupt Service Routine para o botão
{
    // Muda o estado para o próximo na sequência
    if (Estado == ST_OFF)
    {
        Estado = ST_ON;
        setTimerFor4Seconds(); // Configura para 4 segundos no estado ON
    }
    else if (Estado == ST_ON)
    {
        Estado = ST_WARN;
        setTimerFor2Seconds(); // Configura para 2 segundos no estado WARN
    }
    else if (Estado == ST_WARN)
    {
        Estado = ST_OFF;
        setTimerFor4Seconds(); // Reset para o próximo ciclo no estado OFF
    }
}

void setTimerFor4Seconds() {
    // Calcula valor inicial de TCNT1 para 4 segundos com prescaler 1024
    TCNT1 = 0; // Valor inicial para overflow após aproximadamente 4 segundos
}

void setTimerFor2Seconds() {
    // Calcula valor inicial de TCNT1 para 2 segundos com prescaler 1024
    TCNT1 = 3036; // Valor inicial para overflow após aproximadamente 2 segundos
}

ISR(TIMER1_OVF_vect) // Interrupt Service Routine do Timer1 para controlar a duração dos estados
{
    // Controle de transições de estado baseado no overflow
    if (Estado == ST_ON)
    {
        Estado = ST_WARN;
        setTimerFor2Seconds(); // Configura o próximo estado para 2 segundos
    }
    else if (Estado == ST_WARN)
    {
        Estado = ST_OFF;
        setTimerFor4Seconds(); // Retorna ao estado OFF com 4 segundos
    }
    // Reinicia o contador para o próximo intervalo
}