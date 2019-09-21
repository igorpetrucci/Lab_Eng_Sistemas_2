#define SENSOR_PIN A0

// Pulse Monitor Test Script
int ledPin = 13; // This pin is connected on the Arduino board to an LED, if you want to blink this LED
                       // to indicate a pulse is being detected, defining it here is a good idea, however
                       // in the code that follows, this is never done, so this line could be deleted.
int sensorPin = 0; // This is the analog sensor pin the backwards S pin is connected to
                           // you can use any of the analog pins, but would need to change this to match
double alpha = 0.50; // This code uses a rather cool way of averaging the values, using 75% of the
                               // average of the previous values and 25% of the current value.
int period = 20; // This is how long the code delays in milliseconds between readings (20 mSec)
double change = 0.0; // My guess is that this was going to be used to detect the peaks, but the
                                // code never does... you can delete this line as well.

// This code runs on startup, it sets ledPin to output so it can blink the LED that is built into the
// Arduino (but then never does), and sets the serial port up for fast 115,200 baud communication
// of the values (make sure you change your serial monitor to match).
void setup ()
{
  pinMode (ledPin, OUTPUT); // not used, can remove
  Serial.begin (115200); // the baud rate of the serial data
}
void loop ()
{
    static double oldValue = 0; // used for averaging.
    static double oldChange = 0; // not currently used
    int rawValue = analogRead (sensorPin); // This reads in the value from the analog pin.
                                                              // this is a 10 bit number, and will be between 0 and 1023
                                                              // If this value doesn't change, you've connected up
                                                              // something wrong


    double value = alpha * oldValue + (1 - alpha) * rawValue; // Calculate an average using 75% of the
                                                                                         // previous value and 25% of the new
   
    Serial.print (rawValue); // Send out serially the value read in
    Serial.print (",");          // Send a comma
    Serial.println (value);   // Send out the average value and a new line
    //Serial.println (analogRead(A0));
    oldValue = value;        // Save the average for next iteration
    delay (period);            // Wait 20 mSec
}
