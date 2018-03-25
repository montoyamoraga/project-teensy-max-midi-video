//teensy send midi
//control change message
//by aaron montoya-moraga
//march 2018
//running on teensy 3.2

//using examples found here
//http://www.instructables.com/id/Max-Sonar-EZ0/

//pin  9 is the max sonar ez0 sensor
//pin 10 is output to LEDs

//we will be using the pulse width mode of the sensor
//the scale factor is 147 us per inch

//variables for midi cc communications
// sendControl needs to CHANGE for each teensy, 1  or 2 or 3
int sendControl = 2;
int sendChannel = 1;
int sendValue = 0;

//variables for sensor and LED
//int sensorPin = 9;
int sensorPin = 3;
int ledPin = 10;
bool isPeople = false;
int yesPeople = 127;
int noPeople = 0;
//int onLight = 700;
//int onLight = 2000;
//int offLight = 50;
int onLight = 100;
int offLight = 5;
int currentLight = onLight;
int deltaLight = 10;
unsigned long thresholdValue = 400; //= 100; //400
long measure;
unsigned long sensorValue;
int numberLoop = 10;
int currentLoop = 0;
int scaledValue;

void setup() {
    Serial.begin(9600);
//  analogWriteFrequency(10, 5000);
  //  analogWriteFrequency(10, 4394.531);
  //  analogWriteResolution(13);
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);

  //Serial.begin(9600);
}

void loop() {

  //    Serial.print("currentLoop: ");
  //    Serial.println(currentLoop);

  if (currentLoop % numberLoop == 0) {
    //detect people
    detectPeople();

    //decide the value to be sent
    if (isPeople) {
      sendValue = yesPeople;
    }
    else {
      sendValue = noPeople;
    }
  }

  //send the value over midi cc
  usbMIDI.sendControlChange(sendControl, sendValue, sendChannel);

  //control the led lights
  ledControl();

  currentLoop = (currentLoop + 1) % numberLoop;
  //  Serial.println("scaledValue: " + (String)scaledValue);

}

void detectPeople() {

  //  sensorValue = pulseIn(sensorPin, HIGH);
  sensorValue = pulseIn(sensorPin, HIGH);

  //  Serial.print("sensorValue: ");
  //  Serial.print(sensorValue);
  //  Serial.println(" right?");

  //scale it and turn it to
  scaledValue = sensorValue / 58;

  //filter out when the value is too low
  if (scaledValue > 4) {
    //  if ((sensorValue < (long)thresholdValue) && (sensorValue > (long)2) ) {
    if ((sensorValue < thresholdValue)) {
      isPeople = true;
    } else {
      isPeople = false;
    }
  }


}

//function for turning on led lights
void ledControl() {

  //send the value to the led lights
  analogWrite(ledPin, currentLight);
  //  Serial.print("currentLight: ");
  //  Serial.println(currentLight);

  //turn up if there is nobody, turn down if there is somebody
  if (isPeople) {
    currentLight = currentLight - deltaLight;
    //    currentLight = (int)(currentLight * 0.90);
  }
  else {
    currentLight = currentLight + deltaLight;
    //currentLight = (int)(currentLight * 1.10);
  }

  //constrain to the maximum and minimum values
  currentLight = constrain(currentLight, offLight, onLight);

}

