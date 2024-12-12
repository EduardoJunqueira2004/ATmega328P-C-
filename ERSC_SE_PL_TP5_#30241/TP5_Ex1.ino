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
// Ficha Prática nº5 -Exercícios Link Trinkercad: https://www.tinkercad.com/things/7JYj9uaVGKt/editel?sharecode=bMzFAsSWbd6nPvBD9yHhPeptnuRHEcSqcxsmPQ_fQbQ
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ex1 Todo-->1.1+1.2+1.3+1.4 Tabela 1 LED RGB - códigos de cores 
//:Enunciado-->Exercício 1 o seguinte protótipo:
//Pretende-se implementar um device driver para controlar um LED RGB com
//void SET_RGB_COLOR(unsigned int color);
//Os LEDs RGB possuem três LEDs encapsulados no mesmo dispositivo e permitem
//apresentar uma grande quantidade de cores que são originadas a partir de três cores
//primárias: vermelho (Red), verde (Green) e azul (Blue). Cada um destes LEDs pode ser
//controlado de forma individual ou combinados entre si para gerar diversas cores.
//1.1 Importe para a sua área de trabalho o circuito disponibilizado neste link e
//implemente uma função que recebe como parâmetros os valor do duty-cycle entre 0 e
//255 para cada uma das cores do LED RGB e gere o respetivo PWM em cada um dos pinos
//ligados ao LED RGB.
//1.2Implemente uma função que recebe como parâmetro de entrada um valor entre 0 e 6 
//e que acenda o LED de acordo com a codificação de cores apresentada na tabela
//1.3 Implemente uma solução que permita enviar através da consola um código de cor entre 
//0 e 6 para configurar a cor pretendida no LED. 
//Sempre que o utilizador inserir um novo código a cor do LED deve ser alterada em conformidade.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Define os pinos correspondentes às cores do LED RGB

#define PIN_R 6 //  Pino PWM para a cor Vermelha (Red)
#define PIN_G 3 // Pino PWM para a cor Verde (Green)
#define PIN_B 5 // Pino PWM para a cor Azul (Blue)

//Definir o botão

#define BTN 2 


//função RGB LED para controlar o LED RGB
void LED_RGB(unsigned char red,unsigned char green ,unsigned char blue)
{
 
  
   //gerar os sinais PWM nos pinos especificados de cada cor
    analogWrite(PIN_R, red);   // Controla o duty-cycle para o pino Vermelho
    analogWrite(PIN_G, green); // Controla o duty-cycle para o pino Verde
    analogWrite(PIN_B, blue);  // Controla o duty-cycle para o pino Azul

}

//função SET RGB COLOR para configurar a cor com base no código recebido
//Recebe um código de cor (0-6) e utiliza um mapa de cores para definir a saída do LED RGB

void SET_RGB_COLOR(unsigned int ColorCode)
{
  //implementação SET_RGB_COLOR apartir da Tabela 1 LED RGB-código de cores Matriz 7 colunas por 3 linhas
  unsigned char color_map[7][3] = 
    {
        {255, 0, 0}, // [0] - RED
        {255, 127, 0}, // [1] – ORANGE
        {255, 255, 0}, // [2] - YELLOW
        {0, 255, 0}, // [3] - GREN 
        {0, 0, 255}, // [4] - BLUE
        {75, 0, 130},  // [5] - INDIGO
        {143, 0, 255}    // [6] - VIOLET
  };
  
  //Verifica se o código de cor está dentro do intevalo pretendido 
  if(ColorCode<0 || ColorCode >6)
  {
    Serial.println("Codigo de cor invalido! Insira um valor entre 0 e 6.");
    return;//sai da função se o código for inválido
  }
        
  
        // Passa os valores RGB correspondentes para a função LED_RGB
   LED_RGB(color_map[ColorCode][0], color_map[ColorCode][1], color_map[ColorCode][2]);//inclui os três valores RGB
        
        //Exibe uma mensagem confirmando a cor configurada
        Serial.print("Cor configurada para o código: ");
        Serial.println(ColorCode);
    }

//função setup de inicialização
void setup()
{
  
  Serial.begin(9600);
  
  //Meter BTN A INPUT
  pinMode(BTN , INPUT);
  
  //Meter Pinos a OUTPUT
  // Inicializar os pinos como saída
    pinMode(PIN_R, OUTPUT); // Vermelho
    pinMode(PIN_G, OUTPUT); // Verde
    pinMode(PIN_B, OUTPUT); // Azul
  
  //Pedir os valores na consola parâmetro de entrada
  Serial.println("Insira um numero inteiro de cor (0-6) para configurar o LED RGB:");
}
  

// Função loop de controlo
void loop() {
    // Verifica se há dados disponíveis na consola serial
    if (Serial.available() > 0) {
        // Lê o código de cor enviado pela consola
        int ColorCode = Serial.parseInt(); // Lê um número inteiro da entrada serial

        // Configura a cor do LED com base no código recebido
        SET_RGB_COLOR(ColorCode);

        // Limpa o buffer serial para evitar conflitos
        while (Serial.available() > 0) {
            Serial.read();
        }
    }

    // Exemplo de teste de todas as cores do mapa (opcional, pode ser removido)
    /*for (int i = 0; i <= 6; i++) {
        SET_RGB_COLOR(i); // Configura a cor correspondente
        delay(1000);      // Espera 1 segundo antes de mudar a cor
    }*/
}