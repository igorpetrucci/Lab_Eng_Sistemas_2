//====================BATIMENTOS===============================
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
MAX30105 particleSensor;
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

//  Hardware Connections (Breakoutboard to Arduino):
//  -5V = 5V (3.3V is allowed)
//  -GND = GND
//  -SDA = A4 (or port SDA Arduino MEGA) with Resistor 10k
//  -SCL = A5 (or port SCL Arduino MEGA) with Resistor 10k
//  -INT = Not connected

//====================RESPIRAÇÃO===============================
//Sensor de RESPIRAÇÃO (Variaveis que não mudam)
const int piezoSensor = A0; // PIEZO CONECTA NO A0(Fio vermelho) e (Fio Preto) no GND
const int threshold = 100;  // threshold value to decide when the detected sound is a knock or not
const int incidences = 5;
const int holes = 2;
//====================UMIDADE==================================
int valor_analogico;
const int pino_sinal_analogico = A1; //PINO A1 PARA UMIDADE (Sequir Esquematico)

// Variaveis que mudam ao decorrer do programa: (RESPIRACAO)
int sensorReading = 0;      // variable to store the value read from the sensor pin
int lastReading = 0;        // varuable to store de last value from the sensor pin
int ledState = LOW;         // variable used to store the last LED status, to toggle the light
int incidencesCount = 0;    // variable to count how many incidences the value from the sensor has been more or less de same (+-10)
int holesCount = 0;         // variable to count false positives and false negatives


void setup(){
   Serial.begin(115200);  //Serial port 
   pinMode(pino_sinal_analogico, INPUT); //Sensor Umidade

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

void loop(){

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

  //Fim Batimento

  //Le o valor do pino A1 do sensor
  valor_analogico = analogRead(pino_sinal_analogico);
 
  //Mostra o valor da porta analogica no serial monitor
  Serial.print("Porta analogica: ");
  Serial.print(valor_analogico);
 
  //Solo umido, acende o led verde
  if (valor_analogico > 0 && valor_analogico < 400)
  {
    Serial.println(" Status: Solo umido");
  }
 
  //Solo com umidade moderada, acende led amarelo
  if (valor_analogico > 400 && valor_analogico < 800)
  {
    Serial.println(" Status: Umidade moderada");
  }
 
  //Solo seco, acende led vermelho
  if (valor_analogico > 800 && valor_analogico < 1024)
  {
    Serial.println(" Status: Solo seco");
  }
  delay(100);
  //Fim Umidade

  // read the sensor and store it in the variable sensorReading:
  sensorReading = analogRead(piezoSensor);
  // print the information in the Serial port (for debug)
  Serial.println(sensorReading);
  //Serial.println(lastReading);
  //Serial.println(incidencesCount);
  //Serial.println(holesCount);
  // if we has been pass the incidences threashold then we have a problem...
  if (incidencesCount >= incidences)
  {
    Serial.println("NO BREATHING DETECTED!!!");
  }
  else {
    Serial.println("Breathing detected...");
  }
  //since this is an analog sensor, everytime that we read the sensor the value may be little diferent even if there is no activity
  if (lastReading >= sensorReading - 10  && lastReading <= sensorReading + 10)
  {
    // if the value is more or less the same (+-10, could be more, depending on yours) then we asume we have an incidence
    incidencesCount++;
  }
  else 
  {
    holesCount++;
    if (holesCount >= holes)
    {
      // if there is 3 or more holes (false positive/negatives)
      holesCount=0;
      incidencesCount=0;
    }
  }
  lastReading = sensorReading;
  delay(1000);  // delay to avoid overloading the serial port buffer
  //FIm Respiração
  
}
