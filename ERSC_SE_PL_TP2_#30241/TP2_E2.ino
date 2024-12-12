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
// 2-Exercícios-Ex2:https://www.tinkercad.com/things/8601lkBPuMD/editel?sharecode=QmYbfn2zjdC1GApMoC3aAY4eeVcblcSzknnltyaUR2I
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//Exercício 2 Pretende-se implementar um sistema que conte o número 
//de vezes que o botão de pressão é pressionado. 
//Sempre que o contador sofrer alterações o valor deve ser enviado
//via porta série para consola. O LED deve indicar se o valor do 
//contador é par ou ímpar (desligado quando o valor do 
//contador é par e ligado quando o valor do contador é ímpar).
////////////////////////////////////////////////////////////////// 


// Pinos ARDUINO
#define Led1 13	// LED1 pin
#define Btn1 2	// BTN1 pin

// Variáveis globais
int Cnt = 0;
int LastBtnState = LOW;// Contador de pressões de botão

////////////////////////////////////////////////////////////////
//  LedsInit - Set led pins as output
////////////////////////////////////////////////////////////////

void MudaLed(){
  // Atualiza o estado do LED com base no valor do contador
      // Atualiza o estado do LED com base no valor do contador
      if (Cnt % 2 == 0) 
      {
        digitalWrite(Led1, LOW);  // Se o contador é par, desliga o LED
        Serial.println("LED apagado (contador par).");
      } else 
      {
        digitalWrite(Led1, HIGH);  // Se o contador é ímpar, acende o LED
        Serial.println("LED aceso (contador impar).");
      }
}

//////////////////////////////////////////////////////////////////
//Função Setup - Inicialização  é invocada quando o micro-controlador inicia
// A função setup() é executada apenas uma vez, após a placa ser alimentada ou ser feito um reset.
//////////////////////////////////////////////////////////////////
void setup(){
Serial.begin(9600);
  pinMode(Led1, OUTPUT);
  pinMode(Btn1, INPUT);
Serial.println("In setup fuction");
}

/////////////////////////////////////////////////
// Função Loop - Loop de controlo coma utilização de Polling
/////////////////////////////////////////////////
void loop() {	 
  // Leitura do estado atual do botão (Polling)
  int currentBtnState = digitalRead(Btn1);

  // Verifica se o botão foi pressionado (transição de LOW para HIGH)
  if (currentBtnState == HIGH && LastBtnState == LOW) {
    Cnt++;  // Incrementa o contador quando o botão é pressionado
    Serial.print("Contador: ");
    Serial.println(Cnt);

    // Atualiza o estado do LED com base no valor do contador
    MudaLed();
  }

  // Atualiza o estado anterior do botão
  LastBtnState = currentBtnState;
}

  
  