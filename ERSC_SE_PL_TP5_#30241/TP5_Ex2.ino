//////////////////////////////////////////////////////////////////////////////
//
// Ficha Prática 5 - PWM (LED RGB)
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
// Ficha Prática 5: Parte2 2. PWM - Exercícios
//////////////////////////////////////////////////////////

/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ficha Prática nº5 -Exercícios Link Trinkercad:https://www.tinkercad.com/things/iwDUrn2JRFv/editel?sharecode=ACT9VqjbS6bAhJ78drLp5Y9w8NIByYU_fZTKLwcGcHA
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ex2 Todo-->2.3+2.4 Tabela 2 Representação cromática do número de eventos. :Enunciado-->
//Exercício 2 Pretende-se implementar um sistema utilizando o paradigma de
//programação orientado a eventos. Do ponto de vista funcional o sistema apresenta os
//seguintes requisitos:
//• O sistema arranca com o LED RGB a vermelho com intensidade máxima e vai
//reduzindo a sua intensidade durante 4 segundos até se apagar;
//• Após os 4 segundos iniciais, o LED RGB fica verde durante 1 segundo;
//• Depois de 1 segundo a verde, o LED RGB fica a piscar em modo intermitente com
//cor azul e um período de 500 ms (250 ms ON, 250 ms OFF) até que o botão seja
//pressionado.
//• Após o botão ser pressionado:
//– O número de vezes que o LED acedeu com a cor azul aparece na consola;
//– O LED RGB fica a piscar em modo intermitente com período de 500 ms na cor
//que representa o número de vezes que o LED acendeu (ver tabela 2).
//• Depois do LED piscar 3 vezes com indicação do número de vezes que o LED acendeu
//a azul, todo o processo é repetido indefinidamente.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define os pinos correspondentes às cores do LED RGB
#define PIN_R 6 // Pino PWM para a cor Vermelha (Red)
#define PIN_G 3 // Pino PWM para a cor Verde (Green)
#define PIN_B 5 // Pino PWM para a cor Azul
#define BTN 2   // Botão

// Definição dos estados da máquina 5 estados 
enum States { INIT, GREEN, BLUE, SHOWDISPLAY, RESTART };
States currentState = INIT; // Estado inicial

// Contador de eventos e estado do botão
int blinkCount = 0;
bool buttonPressed = false;

// Função para controlar o LED RGB
void RGB_Led(unsigned char red, unsigned char green, unsigned char blue) {
    analogWrite(PIN_R, red);   // Controla o duty-cycle para o pino Vermelho
    analogWrite(PIN_G, green); // Controla o duty-cycle para o pino Verde
    analogWrite(PIN_B, blue);  // Controla o duty-cycle para o pino Azul
}

// Função para reduzir a intensidade do LED RGB
void fadeOutRed() {
    for (int intensity = 255; intensity >= 0; intensity--) {
        RGB_Led(intensity, 0, 0);
        delay(16); // 16 ms para completar 4 segundos
    }
}

// Função para determinar o código de cor com base no número de eventos Tabela 2 do enunciado
int getColorCode(int events) {
    if (events >= 0 && events <= 4) return 0;       // RED
    if (events >= 5 && events <= 9) return 1;       // ORANGE
    if (events >= 10 && events <= 14) return 2;     // YELLOW
    if (events >= 15 && events <= 19) return 3;     // GREEN
    if (events >= 20 && events <= 24) return 4;     // BLUE
    if (events >= 25 && events <= 29) return 5;     // INDIGO
    if (events >= 30) return 6;                     // VIOLET
    return 0; // Default (se algo der errado)
}

// Função que implementa a máquina de estados implementação consoante a tabela  1 do exercício 1 copy
void SET_RGB_COLOR() {
    // Mapa de cores RGB
    unsigned char color_map[7][3] = {
        {255, 0, 0},    // [0] - RED
        {255, 127, 0},  // [1] – ORANGE
        {255, 255, 0},  // [2] - YELLOW
        {0, 255, 0},    // [3] - GREEN 
        {0, 0, 255},    // [4] - BLUE
        {75, 0, 130},   // [5] - INDIGO
        {143, 0, 255}   // [6] - VIOLET
    };

    // Máquina de estados
    switch (currentState) {
      //estado1
        case INIT: {
            fadeOutRed();
            currentState = GREEN; // Transição para o próximo estado
            break;
        }
  //estado2
        case GREEN: {
            RGB_Led(0, 255, 0); // LED verde
            delay(1000);        // 1 segundo
            currentState = BLUE; // Transição
            break;
        }
  //estado3
        case BLUE: {
            if (!buttonPressed) {
                RGB_Led(0, 0, 255); // LED azul
                delay(250);         // ON
                RGB_Led(0, 0, 0);   // OFF
                delay(250);
                blinkCount++;       // Incrementa o contador
            } else {
                currentState = SHOWDISPLAY; // Transição
            }
            break;
        }
  //estado4
        case SHOWDISPLAY: {
            int colorCode = getColorCode(blinkCount); // Determina a cor

            // Exibe o número de eventos
            Serial.print("Numero de eventos: ");
            Serial.println(blinkCount);

            // Pisca 3 vezes com a cor correspondente
            for (int i = 0; i < 3; i++) {
                RGB_Led(color_map[colorCode][0], color_map[colorCode][1], color_map[colorCode][2]);
                delay(500); // ON
                RGB_Led(0, 0, 0);
                delay(500); // OFF
            }

            blinkCount = 0;         // Reinicia o contador
            buttonPressed = false;  // Reinicia o estado do botão
            currentState = RESTART; // Transição
            break;
        }
  //estado5
        case RESTART: {
            currentState = INIT; // Reinicia
            break;
        }
    }
}

// Função de inicialização
void setup() {
    Serial.begin(9600);

    // Configura os pinos como saída
    pinMode(PIN_R, OUTPUT);
    pinMode(PIN_G, OUTPUT);
    pinMode(PIN_B, OUTPUT);

    // Configura o botão como entrada
    pinMode(BTN, INPUT);

    Serial.println("Sistema inicializado.");
}

// Loop principal
void loop() {
    // Verifica o estado do botão
    if (digitalRead(BTN) == HIGH) {
        buttonPressed = true;
    }

    // Chama a função da máquina de estados
    SET_RGB_COLOR();
}
