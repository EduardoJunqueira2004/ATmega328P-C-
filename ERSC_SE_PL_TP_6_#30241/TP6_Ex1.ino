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
// Ficha Prática nº6 -Exercícios Link Trinkercad:https://www.tinkercad.com/things/98nNJ7nM6I4/editel?sharecode=z0EdmMD-2EXuZCUySCrf9AKMWlo9GGJwL3ttiVksjqA
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//Exercício 1 Pretende-se implementar um sistema para amostrar um sinal analógico
//convertendo uma tensão de entrada V in (entre 0 V e 5 V ) para um código digital. 
//A variação da tensão pode ser obtida com recurso a um divisor de tensão implementado
//através de um potenciómetro. Adicionalmente pretende-se utilizar um LED RGB a 
//funcionar como dispositivo de saída. Do ponto de vista funcional o programa apresenta os seguintes requisitos de aplicação:
//• Na etapa de inicialização deverá configurar o módulo A/D para amostrar a entrada 
//analógica AN0 do módulo A/D do ATmega328 com tensões de referência V ref+ = 5 V e V ref− = 0 V .
//• A amostragem de V in deverá ser efectuada na função loop() via pooling com um período de 100 ms;
//• O led RGB deverá alterar a sua cor mediante o valor de V in medido, tendo em conta a informação na tabela 1.
//• Sempre que amostrar a tensão de entrada V in, deverá imprimir na consola o respetivo valor em Volt, 
//numa escala de 0.0 V a 5.0 V , com resolução à décima de Volt;
//Tabela 1 Exercício 1 - representação cromática da tensão de entrada.
//
//
//////////////////////////////////////////////////////////////////////////////

//Definir valores para LED RGB e Potenciômetro
#define RED	5  // RGB - RED
#define GRE 3  // RGB - GREEN
#define BLU 4  // RGB - BLUE
#define POT A0 // Potenciometro
#define DIVISOR A1 // Pino onde o divisor está ligado

unsigned long prevTimestamp = 0;
unsigned long currentTimestamp = 0;

// Definir  Drivers LED RGB
void RGB_Led_init(void)// Inicialização do Led RGB 
{
  pinMode(RED, OUTPUT);
  pinMode(GRE, OUTPUT);
  pinMode(BLU, OUTPUT);
}

void SET_RGB_COLOR(unsigned int color) // Controlo do Led RGB 
{ 
  	  analogWrite(RED, 0);
      analogWrite(GRE, 0);
      analogWrite(BLU, 0);
  
  switch (color) {
    case 0: // rgb Vermelho
      analogWrite(RED, 255);
      break;
    case 1: //rgb  Laranja
      analogWrite(RED, 255);
      analogWrite(GRE, 127);
      break;
    case 2: // rgb Amarelo
      analogWrite(RED, 255);
      analogWrite(GRE, 255);
      break;
    case 3: // rgb Verde
      analogWrite(GRE, 255);
      break;
    case 4: // rgb Azul
      analogWrite(BLU, 255);
      break;
  }
  
}


// ADC Device Drivers importante 
void ADC_init(void){ 			 // Init ADC function
 pinMode(POT, INPUT); // Configura o pino analógico como entrada

  // Configuração dos registos do ADC neste caso para um sinal analógico 
  bitWrite(ADCSRA, ADEN, 1); // Ativa o ADC
  bitWrite(ADCSRA, ADIF, 0); // Limpa o flag de interrupção do ADC
  bitWrite(ADCSRA, ADIE, 0); // Desabilita a interrupção do ADC
  bitWrite(ADMUX, REFS0, 1); // AVCC como referência de tensão
  bitWrite(ADMUX, REFS1, 0); // Continua usando AVCC
  bitWrite(ADMUX, ADLAR, 0); // Justificação à direita do resultado
}

//Ler o valor do ADC no canal ADC0
float ADC_read_Volt(void)// read sample from ADC function
{ 		 
 // Seleciona o canal ADC0
  bitWrite(ADMUX, MUX3, 0);
  bitWrite(ADMUX, MUX2, 0);
  bitWrite(ADMUX, MUX1, 0);
  bitWrite(ADMUX, MUX0, 0);

  // Inicia a conversão
  bitWrite(ADCSRA, ADSC, 1);

  // Aguarda o término da conversão
  while (bitRead(ADCSRA, ADSC) == 1);

  // Calcula o valor digital a partir dos registos ADCL e ADCH
  int digital_code = (ADCH << 8) + ADCL;

  // Converte o valor digital para tensão
  float vin = digital_code * 5.0f / 1023.0f;
  return vin;
}


// Função para determinar a cor com base no valor de Vin ver tabela valores 
unsigned int Vin_to_Color(float vin) {
  if (vin >=0 && vin < 1.0 ) return 0;   // Vermelho
  if (vin >=1 && vin < 2.0) return 1;   // Laranja
  if (vin >= 2.0 && vin <3.0) return 2;   // Amarelo
  if (vin >=3.0 && vin < 4.0) return 3;   // Verde
  if (vin >=4.0 && vin < 5.0) return 4;     // Azul
}

//Função setup de inicio
void setup(){
  	Serial.begin(9600);
  RGB_Led_init();     // Inicializa o LED RGB
  ADC_init();         // Inicializa o ADC
	
}
             
//Função de Loop de controlo
void loop()
{

  currentTimestamp = millis(); // Obtém o timestamp atual

  if (currentTimestamp - prevTimestamp > 100) { // Executa a cada 100 ms
    prevTimestamp = currentTimestamp;

    // Realiza a conversão ADC Analógico to Digital chama a função
    float vin = ADC_read_Volt();

    // Determina a cor do LED com base em Vin chama a função
    unsigned int color = Vin_to_Color(vin);

    // Atualiza o LED RGB 
    SET_RGB_COLOR(color);

    // Imprime o valor de Vin e a cor na consola
    Serial.print("Vin valor : ");
    Serial.print(vin, 1); // Imprime Vin com uma casa decimal
    Serial.print(" V -> Cor: ");
    Serial.println(color);
    
    float divisorVoltage = analogRead(DIVISOR) * 5.0 / 1023.0;
Serial.print("Divisor Voltagem: ");
Serial.print(divisorVoltage, 2); // Mostra a tensão medida com duas casas decimais
Serial.println(" V");

  }

}