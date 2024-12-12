//////////////////////////////////////////////////////////////////////////////
// Ficha Prática 1 - Introdução ATMEGA (IO Digitais)
//////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
// Ficha Prática 2 I/O Ports - Exercícios:
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////
/////////////////////////////////////////////////
// 2-Exercícios-Ex1:https://www.tinkercad.com/things/jyGnAK5dmEy/editel?sharecode=_Ox1ZvjMJl_uPxZmBEKowj7KomxjWBalJFgPR0w_L68
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//Ex1:Exercício 1 Pretende-se implementar um sistema com 3 botões de pressão a funcionar
//como transdutores de entrada (sensor) e LEDs a funcionar como transdutores de saída 
//(atuador) que cumpra com os seguintes requisitos:
//• Enquanto o botão 1 estiver pressionado o LED 1 deve permanecer aceso; 
//• Enquanto o botão 2 estiver pressionado os LEDs 1 e 2 devem permanecer acesos; 
//• Enquanto o botão 3 estiver pressionado os LEDs 1, 2 e 3 devem permanecer acesos.
//E todas as outras alineas aqui feitas:
//////////////////////////////////////////////////////////////////


// Pinos ARDUINO
#define Led1 13	// LED1 pin
#define Led2 12	// LED2 pin
#define Led3 11	// LED3 pin
#define Btn1 8	// BTN1 pin
#define Btn2 9	// BTN2 pin
#define Btn3 10	// BTN3 pin

// Pressed buton (1, 2 ou 3)
// Nr of active LEDs
int BtnNumber;

// Serial print counter value
int Cnt;

//Para a utilizacao da funcao millis
unsigned long previousMillis = 0; // Armazena o último tempo que a ação foi realizada
const long interval = 1000;       // Intervalo de 1 segundo (1000 ms)



////////////////////////////////////////////////////////////////
//  LedsInit - Set led pins as output
////////////////////////////////////////////////////////////////
void LedsInit() 
{
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
}

////////////////////////////////////////////////////////////////
//  BtnsInit - Set buton pins as input
////////////////////////////////////////////////////////////////
void BtnsInit() 
{
  pinMode(Btn1, INPUT);
  pinMode(Btn2, INPUT);
  pinMode(Btn3, INPUT);
}

////////////////////////////////////////////////////////////////
//  ReadButons - Read butons state
////////////////////////////////////////////////////////////////
int ReadButons() 
{ 
  int NumButtonsPressed = 0;
  
  // Lê o estado dos botões
  if (digitalRead(Btn1) == LOW) { // Botão pressionado (assumindo botão pull-up)
    NumButtonsPressed++;
  }
  if (digitalRead(Btn2) == LOW) { // Botão pressionado
    NumButtonsPressed++;
  }
  if (digitalRead(Btn3) == LOW) { // Botão pressionado
    NumButtonsPressed++;
  }
  
  return NumButtonsPressed;
}


////////////////////////////////////////////////////////////////
//  ReadButons - Read butons state
////////////////////////////////////////////////////////////////
void UpdateLeds(int NumLEDsOn)
{ 
  // Desliga todos os LEDs primeiro
  digitalWrite(Led1, LOW);
  digitalWrite(Led2, LOW);
  digitalWrite(Led3, LOW);
  
// Acende os LEDs de acordo com o número de botões pressionados
  if (digitalRead(Btn1) == HIGH) 
  {
    digitalWrite(Led1, HIGH);
  }
  if (digitalRead(Btn2) == HIGH) 
  {
     digitalWrite(Led1, HIGH);
     digitalWrite(Led2, HIGH);
  }
  if (digitalRead(Btn3) == HIGH) 
  {
    digitalWrite(Led1, HIGH);
    digitalWrite(Led2, HIGH);
    digitalWrite(Led3, HIGH);
  }
  
}

void setup()  	// Função de Inicialização
{
  
  // Porta serie para debug
  Serial.begin(9600);
  
  // LEDs initialization
  LedsInit();
  
  // Butons initialization
  BtnsInit();
	Serial.print("In setup fuction!");
}

void loop() 	// Loop de Controlo
{
  
  BtnNumber = ReadButons();
  UpdateLeds(BtnNumber);
  
   //delay(1000); // pauses the program for 1 seconds
  
  /*
  *A função millis() retorna o tempo em milissegundos desde que o Arduino foi iniciado. 
  *Isso permite criar um temporizador que não bloqueia o código, o que significa que o Arduino pode continuar a executar outras tarefas enquanto espera que o tempo passe.
  */
  // Cálculo do tempo atual usando millis()
  unsigned long currentMillis = millis();

  // Se o intervalo de tempo passou (1 segundo neste caso), envia a informação pela porta série
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis; // Atualiza o tempo de referência

    // Incrementa o contador e imprime no Serial Monitor
    Cnt++;
    Serial.println(Cnt);
  }
   
}

