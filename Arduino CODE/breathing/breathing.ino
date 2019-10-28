/* Breathing Sensor
 *  this sketch allows to detect if an infant is breathing or not
 *  this is a ugly hack for the "AngelCare breathing sensor" like this:
 *  http://www.angelcarebaby.com/wp-content/uploads/2013/01/AC401-monitor-sensor-padmoniteur-AC401-detecteur-de-mouvement-600x600.jpg
 *  It has a piezoelectric sensor in the center
 *  in order to make this sketch to work, you will need to connect the positive pole from the sensor to the A0 PIN with a 1mohm 
 *  the other to ground
 *  This is free software, you can modify it and use it the way you like.
 *  Miguel Barajas (@gnuowned) <migbarajas@gmail.com>
 */


// these constants won't change:
const int ledPin = 13;      // led connected to digital pin 13
const int piezoSensor = A0; // the piezo is connected to analog pin 0
const int threshold = 100;  // threshold value to decide when the detected sound is a knock or not
const int incidences = 5;
const int holes = 2;


// these variables will change:
int sensorReading = 0;      // variable to store the value read from the sensor pin
int lastReading = 0;        // varuable to store de last value from the sensor pin
int ledState = LOW;         // variable used to store the last LED status, to toggle the light
int incidencesCount = 0;    // variable to count how many incidences the value from the sensor has been more or less de same (+-10)
int holesCount = 0;         // variable to count false positives and false negatives

void setup() {
  pinMode(ledPin, OUTPUT); // declare the ledPin as as OUTPUT
  Serial.begin(115200);       // use the serial port
}

void loop() {
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
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
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
}
