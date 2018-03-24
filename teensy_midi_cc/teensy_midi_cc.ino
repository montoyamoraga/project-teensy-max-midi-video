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
int sendControl = 1;
int sendChannel = 1;
int sendValue = 0;

//variables for sensor and LED
int sensorPin = 9;
int ledPin = 10;
bool isPeople = false;
int yesPeople = 127;
int noPeople = 0;
int onLight = 700;
int offLight = 50;
int currentLight = onLight;
int deltaLight = 1;
int thresholdValue = 400;
long measure;
long sensorValue;
int numberLoop = 10;
int currentLoop = 0;

void setup() {
  //  Serial.begin(115200);
  analogWriteFrequency(10, 5000);
  analogWriteResolution(12);
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {

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

  currentLoop++;
  currentLoop = currentLoop % numberLoop;

}

void detectPeople() {

  //  sensorValue = pulseIn(sensorPin, HIGH);
  sensorValue = pulseIn(sensorPin, HIGH);

  if (sensorValue < (long)thresholdValue) {
    isPeople = true;
  } else {
    isPeople = false;
  }

}

//function for turning on led lights
void ledControl() {

  //send the value to the led lights
  analogWrite(ledPin, currentLight);

  //turn up if there is nobody, turn down if there is somebody
  if (isPeople) {
    //    currentLight = currentLight - deltaLight;
    currentLight = currentLight * 0.90;
  }
  else {
    //    currentLight = currentLight + deltaLight;
    currentLight = currentLight * 1.10;
  }

  //constrain to the maximum and minimum values
  currentLight = constrain(currentLight, offLight, onLight);

}

