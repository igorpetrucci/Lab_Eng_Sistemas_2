#include <Wire.h>
#include "MAX30105.h"
#include <LiquidCrystal.h>
//Carrega a biblioteca Keypad
#include <Keypad.h>

#include "heartRate.h"
#define pino_sinal_analogico A0 //Sensor umidade
const int piezoSensor = A1; // the piezo is connected to analog pin 1 RESPIRAÇÃO

//Define o tamnho do teclado
const byte ROWS = 4; //quatro linhas
const byte COLS = 4; //Quatro 

//Define a configuraço dos simbolos para o teclado
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
  
};
byte rowPins[ROWS] = {24, 25, 26, 27}; //Define os pinos usados para as linhas
byte colPins[COLS] = {32, 33, 34, 35}; //Define os pinos usados para as colunas

//Cria o teclado
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

MAX30105 particleSensor;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
int valor_analogico;
// these variables will change:
int sensorReading = 0;      // variable to store the value read from the sensor pin
float Comparacao_superior = 0.0;
float Comparacao_inferior = 0.0;
float Sensores_Leitura = 0.0;

void setup()
{
  Serial.begin(9600);
  pinMode(pino_sinal_analogico, INPUT); // Sensor Umidade
  lcd.begin(16, 2);
  
  Serial.println("Initializing...");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}

//CONTINUAÇÃO TECLADO
  //Declaraçod e variaveis
  int k = 0, c, limiar = 0;  //Numeros digitado no formato inteiro
  int res_fis_inc[3]={0,0,0}, res_fis_atu[3]={0,0,0};
  String num1, mens2;       //Numeros que digitados no tipo string
  boolean calibrar = false, calibrado = false, run = false, confirma = false, fc = false, fr = false, um = false, limpa = false, verdade = false, mentira = false;
  
  //Variaveis usadas somente para testar o codigo
  boolean mens1 = false;

void loop()
{
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);

  if (irValue < 50000)
    Serial.print(" No finger?");

  Serial.println();

  lcd.setCursor(0,0);
  lcd.print("B:");
  lcd.print(beatAvg);

//  lcd.setCursor(0,1);
//  lcd.print(" IR: ");
//  lcd.print(irValue);

  //Umidade
  //Le o valor do pino A0 do sensor
  valor_analogico = analogRead(pino_sinal_analogico);
  //converte a variação do sensor de 0 a 1023 para 0 a 100
  valor_analogico = map(valor_analogico,255,1023,100,0);
  //Mostra o valor da porta analogica no serial monitor
  Serial.print("Porta analogica: ");
  Serial.print(valor_analogico);
  Serial.println();
  lcd.setCursor(0,1);
  lcd.print("M:");
  lcd.print(valor_analogico);

  //Respiração
    // read the sensor and store it in the variable sensorReading:
  sensorReading = analogRead(piezoSensor);
    // print the information in the Serial port (for debug)
  Serial.println(sensorReading);
  Serial.println();
  lcd.setCursor(7,0);
  lcd.print("R:");
  lcd.print(sensorReading);
  
  //TECLADO ROTINA
        char key = customKeypad.getKey();
      //Inserir rotina para LCD
      if(mens1 == false){
         mens2 = "Pressione C para calibrar.";
         Serial.println(mens2);
         mens1 = true;
      }
      
      if(calibrado == true && run == false){
         mens2 = "Pressione D modo de deteccao.";
         Serial.println(mens2);
         run = true;
      }
      switch(key){
            case 'C':
                //Rotina para inserir dados de calibraçao
                while(k < 3){  
                    switch(k){
                      case 0:
                        if(fc == false){
                          //Nesse trecho vamos inserir rotinas para imprimir no LCD
                          mens2 = "Insira a frequencia cardiaca:";
                          Serial.println(mens2);
                          fc = true;
                        }
                      break;
                      case 1:
                        if(fr == false){
                           //Nesse trecho vamos inserir rotinas para imprimir no LCD
                           mens2 = "Insira a frequencia respiratoria:";
                           Serial.println(mens2);
                          fr = true;
                        }
                       
                      break;
                      case 2:
                         if(um == false){
                           //Nesse trecho vamos inserir rotinas para imprimir no LCD
                           mens2 = "Insira o valor de umidade:";
                           Serial.println(mens2);
                           um = true;
                        }
                         
                      break;
                    };
                    char key = customKeypad.getKey(); //Guarda o dado lido teclado na varivel key
                    if (key != NO_KEY && (key=='1' || key=='2' || key=='3' || key=='4' || key=='5' || key=='6' || key=='7' || key=='8'|| key=='9' || key=='0')){
                          num1 = num1 + key; //Concatena string
                      }
                  
                      if (key !=NO_KEY && key == '*'){
                         res_fis_inc[k] = num1.toInt(); //Converte para interiro
                         Serial.println(res_fis_inc[k]);
                         k++;
                         num1 = "";
                      }
              
                }
               //Define o limiar para pessoa em questao 
               for(int i = 0; i<k; i++){
                 limiar += res_fis_inc[i];  
               }
               k = 0;
               calibrado = true;
               if(calibrado == true){
                 mens2 ="Equipamento calibrado.\nLimiar:";
                 Serial.println(mens2);
                 Serial.println(limiar);
               }
                //Fim da rotina para inserir dados de calibraçao
            break;

            //IMPLEMENTAÇÃO DO RESET
            case 'A':
              calibrado = false;
              limiar = 0;
              Sensores_Leitura = 0.0;
              Comparacao_superior = 0.0;
              Comparacao_inferior = 0.0;
              beatAvg = 0;
              valor_analogico = 0;
              sensorReading = 0;
            
            break;        
      }; //Fim do switch case menu     


    //Logica 
    Sensores_Leitura = (beatAvg) + (valor_analogico) + (sensorReading);
  //Sensores_Leitura = (beatAvg*0.7367) + (valor_analogico*0.1967) + (sensorReading*0.0833);
  //Sensores_Leitura = ((Sensores_Leitura)/(0.7367+0.1967+0.0833));
  Serial.print(Sensores_Leitura);
  Serial.println();

  //LÓGICA DE PROGRAMAÇÃO

  Comparacao_superior = (Sensores_Leitura*1.4); //adicionando 40% do valor da leitura
  Comparacao_inferior = (Sensores_Leitura*0.6); //subtraindo 40% do valor da leitura

  if( limiar < Comparacao_inferior  || limiar > Comparacao_superior){
      lcd.setCursor(7,1);
      lcd.print("MENTIRA");
      delay(500);
  }
    if(Comparacao_inferior < limiar && limiar < Comparacao_superior ){
      lcd.setCursor(7,1);
      lcd.print("VERDADE");
  }
  
}
