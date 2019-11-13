#include <Wire.h>
#include "MAX30105.h"
#include <LiquidCrystal.h>

#include "heartRate.h"
#define pino_sinal_analogico A0 //Sensor umidade
const int piezoSensor = A1; // the piezo is connected to analog pin 1 RESPIRAÇÃO

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


  //Logica para enviar um int para o processing
  float Sensores_Pesos = 0.0;
  Sensores_Pesos = (beatAvg*0.7367) + (valor_analogico*0.1967) + (sensorReading*0.0833);
  Serial.print(Sensores_Pesos);
  Serial.println();
  lcd.setCursor(7,1);
  lcd.print("P:");
  lcd.print(Sensores_Pesos);
  
}
