//////////////////////////////////////////////////////////////////////////////
// 
// Ficha Prática 6 - ADC e PWM (LED RGB)
// SIL - Novembro de 2018
//
// A - Protótipos das funções Arduino C a utilizar para efectuar Digital I/O:
//   void pinMode(int pin_ID, bool INPUT/OUTPUT) - define a direcção do fluxo INPUT/OUTPUT
//   void digitalWrite(int pin_ID, bool LOW/HIGH)- escreve na porta digital pin_ID o valor LOW/HIGH
//   int digitalRead(int pin_ID) - lê o valor lógico (LOW/HIGH) da porta digital pin_ID
//   int digitalWrite(int pin_ID, unsigned char dc) - gera um pulso PWM com dutyCycle dc no pino pin_ID
//
// B - Datasheet do ATMEGA328
//  http://ww1.microchip.com/downloads/en/devicedoc/atmel-42735-8-bit-avr-microcontroller-atmega328-328p_datasheet.pdf
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
// Ficha Prática 6: Parte2 2. 2. ADCs - Exercícios
//////////////////////////////////////////////////////////

/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ficha Prática nº6 -Exercícios Link Trinkercad:https:https://www.tinkercad.com/things/14W0pigVr29-fp6ex2/editel?returnTo=https://www.tinkercad.com/dashboard/designs/circuits&sharecode=56hM318h59g6ghvyIwPCSoymxClgvAE8x5Bn-7h_TGY
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//Exercício 2 A partir da implementação do exercício anterior altere o código de forma 
//a efetuar a amostragem do sinal analógico recorrendo agora a um evento periódico gerado a partir 
//do Timer 1 que deverá ser configurado para gerar uma interrupção a cada 100 ms. 
//Considere que a atuação no LED RGB é feita em background e que, do ponto de vista funcional, 
//o LED RGB deverá piscar com a cor e período (Ton/Toff ) definidos na tabela 2:
//Tabela 2 Exercício 2 - Comportamento do LED em função da tensão de entrada
//////////////////////////////////////////////////////////////////////////////


// Definir valores para LED RGB e Potenciômetro
#define RED	5  // RGB - RED
#define GRE 3  // RGB - GREEN
#define BLU 4  // RGB - BLUE
#define POT A0 // Potenciometro

volatile bool toggleLED = false;  // Flag para alternar estado do LED
volatile unsigned int currentColor = 0; // Cor atual
volatile float currentVin = 0;    // Valor de tensão atual

// Inicialização do Timer 1 para diferentes periodos
void initTimer1WithPeriod(float periodSeconds) 
{
    noInterrupts();               // Desativa interrupções globais
    TCCR1A = 0;                   // Modo Normal
    TCCR1B = 0;                   // Limpa o registo
    TCCR1B |= (1 << WGM12);       // Modo CTC (Clear Timer on Compare Match)
    TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler de 1024

    // Calcula OCR1A com base no período fornecido
    OCR1A = (unsigned long)((16e6 / 1024) * periodSeconds) - 1;

    TCNT1 = 0;                    // Inicializa o contador
    TIMSK1 |= (1 << OCIE1A);      // Habilita interrupção de comparação do Timer 1
    interrupts();                 // Ativa interrupções globais
}


// Rotina de interrupção para Timer 1
ISR(TIMER1_COMPA_vect) 
{
   toggleLED = !toggleLED; // Alterna o estado do LED
}


// Inicialização do LED RGB
void RGB_Led_init(void)
{
    pinMode(RED, OUTPUT);
    pinMode(GRE, OUTPUT);
    pinMode(BLU, OUTPUT);
}


// Configuração da cor do LED RGB
void SET_RGB_COLOR(unsigned int color)
{
  if (toggleLED) { // Se a flag estiver ativa, liga a cor
        analogWrite(RED, 0);
        analogWrite(GRE, 0);
        analogWrite(BLU, 0);

    switch (color) {
        case 0: analogWrite(RED, 255); break;                 // Vermelho
        case 1: analogWrite(RED, 255); analogWrite(GRE, 127); break; // Laranja
        case 2: analogWrite(RED, 255); analogWrite(GRE, 255); break; // Amarelo
        case 3: analogWrite(GRE, 255); break;                 // Verde
        case 4: analogWrite(BLU, 255); break;                 // Azul
    }
  }else{
    // Caso contrário, desliga o LED
        analogWrite(RED, 0);
        analogWrite(GRE, 0);
        analogWrite(BLU, 0);
  
  }
}

// Inicialização do ADC
void ADC_init(void)
{
    pinMode(POT, INPUT);
    bitWrite(ADCSRA, ADEN, 1); // Ativa o ADC
    bitWrite(ADCSRA, ADIF, 0); // Limpa o flag de interrupção do ADC
    bitWrite(ADMUX, REFS0, 1); // AVCC como referência de tensão
    bitWrite(ADMUX, REFS1, 0);
}

// Ler o valor do ADC e converter para tensão
float ADC_read_Volt(void)
{
    bitWrite(ADMUX, MUX3, 0);
    bitWrite(ADMUX, MUX2, 0);
    bitWrite(ADMUX, MUX1, 0);
    bitWrite(ADMUX, MUX0, 0);

    bitWrite(ADCSRA, ADSC, 1);
    while (bitRead(ADCSRA, ADSC) == 1);
    int digital_code = (ADCL | (ADCH << 8));
    return digital_code * 5.0f / 1023.0f;
}

// Determinar a cor com base no valor de Vin
unsigned int Vin_to_Color(float vin,float* period)
{
    if (vin >= 0 && vin < 1.0){ *period = 1.0; return 0; }   // Vermelho
    if (vin >= 1 && vin < 2.0){ *period = 0.8; return 1; }    // Laranja
    if (vin >=2.0 && vin < 3.0){ *period = 0.6; return 2; } // Amarelo
    if (vin >=3.0 && vin < 4.0){ *period = 0.4; return 3; } // Verde
    if (vin >=4.0 && vin < 5.0){ *period = 0.2; return 4; }// Azul
}

void setup()
{
    Serial.begin(9600);
    RGB_Led_init();
    ADC_init();
    float initialPeriod = 1.0;
    initTimer1WithPeriod(initialPeriod); // Inicializa o Timer 1
}

void loop()
{
  float vin = ADC_read_Volt();         // Lê a tensão
    float period;
    unsigned int color = Vin_to_Color(vin, &period); // Determina a cor e o período

    if (color != currentColor || vin != currentVin) {
        currentColor = color;
        currentVin = vin;
        initTimer1WithPeriod(period); // Atualiza o Timer com o novo período
    }

    SET_RGB_COLOR(currentColor); // Atualiza o LED RGB com base na cor e no estado

    Serial.print("Vin: ");
    Serial.print(vin, 1);               // Imprime Vin com uma casa decimal
    Serial.print(" V -> Cor: ");
    Serial.print(color);
    Serial.print(" -> Periodo: ");
    Serial.print(period, 1);
    Serial.println(" s");
    delay(100); // Reduz a frequência de atualização no Serial Monitor
}
