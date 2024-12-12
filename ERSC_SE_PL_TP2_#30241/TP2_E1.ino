
/////////////////////////////////////////////////
// Ficha Prática 2 I/O Ports - Exercícios:
/////////////////////////////////////////////////
/////////////////////////////////////////////////
// Eduardo Miguel Moreira Junqueira nº30241 ERSC 
/////////////////////////////////////////////////
/////////////////////////////////////////////////
// 1-Exercícios-Ex1:..
/////////////////////////////////////////////////

Perg1:Consulte a secção 2.1 (diagrama de blocos). Quantos portos de I/O digital estão disponíveis no MCU ?
R:3 Portos 
PD[0..7] Porto D com 8 pinos;
PB[0..7] Porto B com 8 pinos;
PC[0..6] Porto C com 7 pinos;

//_____________________________
Perg2:O modo de operação dos portos I/O encontra-se descrito na secção 14, consulte a secção 14.2.1 (Configuring the Pin). Quais os registos que permitem controlar/ler/escrever em cada um dos portos? Para que serve cada um dos registos?

R:Existe 3 tipos de Registos 
Registos que permitem controlar:
DDRx: bit in the DDRx Register selects the direction of this pin
Registos que permitem ler:
PINx:O registo PINx permite ler o estado lógico de um pino configurado como entrada
Registo que permite escrever:
PORTx:Quando o pino está configurado como entrada:
	•	Se o bit PORTxn for 1, o resistor de pull-up é ativado.
	•	Se o bit PORTxn for 0, o resistor de pull-up é desativado.
	Quando o pino está configurado como saída:
	•	Se o bit PORTxn for 1, o pino envia um nível lógico alto (1).
	•	Se o bit PORTxn for 0, o pino envia um nível lógico baixo (0).
	//__________________________________________________
Perg 3:Aceda a https://www.arduino.cc/reference/en/ e consulte a documentação das funções digitalRead(), digitalWrite(), pinMode(). Encontra alguma relação entre estas funções e os registos referidos na secção 14.2.1 do datasheet? 
	
R:O pinMode() interage diretamente com o registo DDRx para configurar o pino como entrada ou saída e, opcionalmente, com PORTx para ativar/desativar o resistor de pull-up.
	O digitalWrite() controla o registo PORTx, seja para alterar o valor de saída de um pino ou para ativar o pull-up quando o pino está como entrada.
	O digitalRead() lê o valor do pino do registo PINx, permitindo saber o estado atual do pino, independentemente de ser entrada ou saída.
	
		//__________________________________________________
Perg 4:Consulte o documento com o mapeamento dos pinos do Arduino, disponivel no moodle, e determine quais os registos afetados pela execução do código infra. Quais os valores configurados em cada registo?

R:
int ledPin = 13; // LED connected to digital pin 13 
int inPin = 7; // pushbutton connected to digital pin 7
void setup() 
{ pinMode(ledPin, OUTPUT); // sets the digital pin 13 as output 
pinMode(inPin, INPUT); // sets the digital pin 7 as input 
}
void loop() {}