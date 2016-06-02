//! Código para o dispositivo SHIU
 /*! Projeto que visa diminuir o nível de ruído no ambiente.
 * @author Ruamberg Vasconcelos e Kewin Lima
 * @since 25/05/2016
 * @version 2.0
 */

//Testar os níveis
#define DEBUG        1 // Ativar(1) ou desativar(0) a comunicação com o serial.
#define NUM_SENSORES 2
#define NUM_INTERACOES 500
#define OVERFLOW     4000000000
#define LUZ          13 // Sinalizador luminoso ligado à porta digital 13 do arduino 
#define SOM1         8	// Sirene ligada à porta digital 8 do arduino
#define NIVEL_AVISO  500 // Determina nível de ruído/pulsos para ativar o sinalizador luminoso.
#define NIVEL_LIMITE 500 // Determina nível de ruído/pulsos para ativar a sirene.
#define TEMPO_LUZ    3000 // Define o tempo de duração em que o sinalizador permanecerá ativo.
#define TEMPO_SOM    3000 // Define o tempo de duração em que a sirene permanecerá ativo.
#define REP_SOM      2 //Quantidade de vezes que a sirene irá disparar 
#define REP_LUZ      2 //Quantidade de vezes que o sinalizador irá disparar
#define ON           1
#define OFF          0
#define TOLERANCIA   2000

int sensores[NUM_SENSORES] = {A0, A1}; // Sensores ligados às portas analógicas A0 e A1
int nivel = 0;
unsigned long t0 = 0;
bool deveAlertar = false;
void(* reset) (void) = 0;

//Função que define os componentes sinalizador,sirene e sensores como entrada ou saída

void setup() 
{
  Serial.begin(9600);
  pinMode(LUZ,  OUTPUT); //@param[OUT]
  pinMode(SOM1, OUTPUT); //@param[OUT]

  for(int i = 0; i < NUM_SENSORES; i++) 
  {
    pinMode(sensores[i], INPUT); //@param[IN]
  }
}

void loop() 
{
  nivel = ouvirNivel();
	
  if(nivel < NIVEL_AVISO) 
  {
    if(t0 < OVERFLOW)
    {
      t0 = (millis()/1000); // Transforma para segundo para armazenar mais valores na variável
      Serial.println(t0);
    }
    else 
    reset();
  }

  deveAlertar = ((millis()/1000) - t0) > TOLERANCIA; // Se a condicao for aceita deveAlertar = 1(true) se a condicao for falsa deveAlertar = 0(false)

  if(deveAlertar && nivel < NIVEL_LIMITE) //se deveAlertar for verdadeiro e nivel < NIVEL_LIMITE aviso luminoso
  {
    luz();
  }
  else if(deveAlertar && nivel >= NIVEL_LIMITE) //se deveAlertar for verdadeiro e nivel >= NIVEL_LIMITE aviso luminoso e sonoro
  {
    luz();
    som();
  }
  if(DEBUG)delay(1000);
}

int ouvirNivel() 
{
  int value = 0;
  int leitura = 0;
  for(int i = 0; i < NUM_SENSORES; i++) 
  {
    leitura = read_sensor(sensores[i]);
    if(DEBUG)
    	imprime_valor(i,leitura,valor);
    if(leitura > value) 
    	value = leitura;
    return value;
  }
}

int read_sensor(int port)
{
  unsigned long value = 0;
  for(int x = 0; x < NUM_INTERACOES; x++)  //Antes tinha times, uma funcao que passava como argumento. Desnecessario muito mais viavel usar um define N_INTERACOES
  {
    value += analogRead(port);
  }
  value /= NUM_INTERACOES;
  return value;
}

void imprime_valor(int i, int leitura, int value) 
{
  Serial.print("Sensor ");
  Serial.print(i+1);
  Serial.print(": ");
  Serial.println(leitura);
}
void luz() 
{
    for(int n=0 ; n< REP_LUZ; n++)
    {
      digitalWrite(LUZ, HIGH);
      if(DEBUG) 
      	Serial.println("Disparado Luz");
      	delay(TEMPO_LUZ);
      	digitalWrite(LUZ, LOW);
      if(DEBUG) 
      	Serial.println("Desligando Luz");
    }
}

void som() 
{
    for(int n=0; n < REP_SOM ; n++ )
    {
      digitalWrite(SOM1, HIGH);
      if(DEBUG)
      	Serial.println("Disparado Som");
	delay(TEMPO_SOM);
      	digitalWrite(SOM1, LOW);
      if(DEBUG) 
      	Serial.println("Desligando Som");
    }
}
