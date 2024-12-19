
//////////////////////////////////////////////////////////
// Ficha Prática 7: Parte2 2. USART (Universal Synchronous and Asynchronous serial Receiver and Transmitter ) - Exercicio
//////////////////////////////////////////////////////////

/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ficha Prática nº7 -Exercícios Link Trinkercad:https://www.tinkercad.com/things/4Th8gLV5IWq/editel?sharecode=fD9h5ysMVILWHmsyfy25H_qaqh2DSpIYHX-2QNJ58-M
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//Exercício 1 Tudo 
//Implementar uma solução que permita enviar através da consola um código de cor entre 0 e 6 para configurar a cor pretendida no LED. Sempre que o utilizador inserir um novo código
//a cor do LED deve ser alterada em conformidade.
//Seguir os passos dados pelo prof nas alineas em baixo!
//////////////////////////////////////////////////////////////////////////////


//Definir pinos PWM(Pulse Width Modulation)
#define PIN_R 6 // ~ PWM PIN IN ARDUINO
#define PIN_G 3 // ~ PWM PIN IN ARDUINO
#define PIN_B 5 // ~ PWM PIN IN ARDUINO

// Variáveis globais:
unsigned char incomingByte;
int color = 0;

// Configurar a porta serial com baud rate de 9600 bps, 8 bits de dados, sem paridade e um stop bit.
void initSerial() {
  // Configurar baud rate para 9600
  UBRR0H = 0;       // Parte alta de 103
  UBRR0L = 103;     // Parte baixa

  // Ativar receção e transmissão
  bitWrite(UCSR0B, RXEN0, 1); // Ativar RX
  bitWrite(UCSR0B, TXEN0, 1); // Ativar TX

  // Configurar frame: 8 bits de dados, 1 stop bit, sem paridade
  bitWrite(UCSR0C, UCSZ01, 1);
  bitWrite(UCSR0C, UCSZ00, 1);
}

// Função que verifica se há dados disponíveis no buffer de receção
unsigned char rxByteAvailable() {
  return bitRead(UCSR0A, RXC0); // Se RXC0 for 1, significa que há dados prontos para leitura
}

// Função para ler dados recebidos; devolve o byte recebido no registo de dados USART
unsigned char rxByte() {
  return UDR0; // Retorna o conteúdo do registo UDR0
}

// Função para configurar a cor do LED RGB com base no código recebido (0-6)
void SET_RGB_COLOR(int color) {
  switch (color) {
    case 0:
      RGB_Led(0, 0, 0); // Apagar LED
      break;
    case 1:
      RGB_Led(255, 0, 0); // Vermelho
      break;
    case 2:
      RGB_Led(0, 255, 0); // Verde
      break;
    case 3:
      RGB_Led(0, 0, 255); // Azul
      break;
    case 4:
      RGB_Led(255, 255, 0); // Amarelo
      break;
    case 5:
      RGB_Led(0, 255, 255); // Ciano
      break;
    case 6:
      RGB_Led(255, 0, 255); // Magenta
      break;
    default:
      RGB_Led(0, 0, 0); // Valor inválido: Apagar LED
  }
}

// Função para aplicar os valores de cor aos pinos do LED RGB
void RGB_Led(unsigned char r, unsigned char g, unsigned char b) {
  analogWrite(PIN_R, r);
  analogWrite(PIN_G, g);
  analogWrite(PIN_B, b);
}

// Configurações iniciais
void setup() {
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  initSerial();
  Serial.println("Insira um numero entre 1 a 6 porfavor!");
}

// Função principal
void loop() {
  if (rxByteAvailable() == 1) {
    incomingByte = rxByte(); // Ler o byte recebido

    // Verificar se é um código válido (entre '0' e '6')
    if (incomingByte >= '0' && incomingByte <= '6') {
      color = incomingByte - '0'; // Converter ASCII para número
      SET_RGB_COLOR(color);      // Configurar a cor do LED
    } else {
      // Mensagem de erro para valores inválidos
      Serial.println("Codigo invalido! Insira um numero entre 0 e 6.");
    }
  }
}
