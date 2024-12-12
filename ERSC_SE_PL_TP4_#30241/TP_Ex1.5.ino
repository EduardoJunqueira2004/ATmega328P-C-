//////////////////////////////////////////////////////////////////////////////
// Ficha Prática 4 - 1. Compreender de forma genérica a diferença entre máquinas de
//estados de Moore e Mealy.
//2. Representação de sistemas com recurso a diagramas de estados e diagramas de fluxo.
//3. Implementação de máquinas de estados no Atmega328p.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
// Ficha Prática 4:2. FSM - Exercícios - Exercício1.4/1.5
//////////////////////////////////////////////////////////

/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2-Exercícios-Ex1 Máquinas de estados de Mealy Diagrama 5 agora implementação Link Trinkercad: https:https://www.tinkercad.com/things/4xAOWrBTAZu-copy-of-fp4ex1/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard&sharecode=SJQNFpqHAsunp8BklnEwFNib_9EuMqf4KwUJAZ-QZyo
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ex1.4/1.5 A figura 5 corresponde á implementação de máquinas de estados de Mealy  e o código que se segue é a sua execução!
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
volatile bool buttonPressed = false;
volatile bool timeExpired = false;

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
    switch (Estado)
    {
        // Estado OFF
        case ST_OFF:
            if (buttonPressed)
            {
                buttonPressed = false;
                Estado = ST_ON;
                setTimerFor4Seconds();
                digitalWrite(LedG, HIGH);  // Liga o LED verde
                Serial.println("Estado: ON");
            }
            else
            {
                digitalWrite(LedG, LOW);
                digitalWrite(LedR, LOW);
                Serial.println("Estado: OFF");
            }
            break;

        // Estado ON
        case ST_ON:
            if (buttonPressed)
            {
                buttonPressed = false;
                Estado = ST_OFF;
                timeExpired = false;
                digitalWrite(LedG, LOW);  // Desliga o LED verde
                digitalWrite(LedR, LOW);  // Garante que o LED vermelho também está desligado
                Serial.println("Estado: OFF");
            }
            else if (timeExpired)
            {
                timeExpired = false;
                Estado = ST_WARN;
                setTimerFor2Seconds();
                digitalWrite(LedR, HIGH);  // Liga o LED vermelho
                Serial.println("Estado: WARN");
            }
            else
            {
                digitalWrite(LedG, HIGH);  // Garante que o LED verde está ligado
                digitalWrite(LedR, LOW);
                Serial.println("Estado: ON");
            }
            break;

        // Estado WARN
        case ST_WARN:
            if (buttonPressed || timeExpired)
            {
                buttonPressed = false;
                timeExpired = false;
                Estado = ST_OFF;
                digitalWrite(LedG, LOW);  // Desliga o LED verde
                digitalWrite(LedR, LOW);  // Desliga o LED vermelho
                Serial.println("Estado: OFF");
            }
            else
            {
                digitalWrite(LedG, HIGH);  // Garante que o LED verde está ligado
                digitalWrite(LedR, HIGH);  // Garante que o LED vermelho está ligado
                Serial.println("Estado: WARN");
            }
            break;
    }

    delay(100); // Pequeno atraso para evitar saturação na porta série
}

void btnISR() // Interrupt Service Routine para o botão
{
    buttonPressed = true;  // Marca que o botão foi pressionado
}

void setTimerFor4Seconds() {
    TCNT1 = 0; // Reset do contador para overflow em aproximadamente 4 segundos
}

void setTimerFor2Seconds() {
    TCNT1 = 3036; // Valor inicial para overflow após aproximadamente 2 segundos
}

ISR(TIMER1_OVF_vect) // Interrupt Service Routine do Timer1 para controlar a duração dos estados
{
    timeExpired = true; // Marca que o tempo expirou
}