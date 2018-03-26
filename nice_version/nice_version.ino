//interactive lights
//by aaron parsekian and aaron montoya-moraga
//march 2018
//runs on teensy 3.2

//based on code found here
//http://www.instructables.com/id/Max-Sonar-EZ0/

//configure on the tools menu:
//board: teensy 3.2
//usb type: midi
//cpu speed: 72 mhz

//pins used
//pin  9 is the max sonar ez0 sensor
//pin 10 is output to LEDs
const int sensorPin = 9;
const int analogOutPin = 10;

//variables for midi cc communications
// sendControl needs to CHANGE for each teensy, 1  or 2 or 3
int sendControl = 1;
int sendChannel = 1;
int sendValue = 0;
int yesPeople = 127;
int noPeople = 0;

//variables for sensors
int sensorValue;

//variables for lights
int onLight = 20;
int offLight = 1;
int currentLight = onLight;
int deltaLight = 3;
int thresholdValue = 400;
float percentageLight = 0.6;

// value to be output
int outputValue = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(9, INPUT);
}

void loop() {

  // read the analog in value:
  sensorValue = pulseIn(sensorPin, HIGH);

  //delay(20);

  // map it to the range of the analog out
  outputValue = map(sensorValue, 0, 1023, 0, 255);

  //  // change the analog out value:
  //  analogWrite(analogOutPin, outputValue);
  analogWrite(analogOutPin, currentLight);

  // print the results to the serial monitor
  Serial.print("sensorValue = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(1);

  if (sensorValue < thresholdValue ) {
    //    currentLight = (int)(currentLight * (1 - percentageLight));
    currentLight = currentLight - deltaLight;
    sendValue = yesPeople;
  } else {
    //    currentLight = (int)(currentLight * (1 + percentageLight));
    sendValue = noPeople;
    currentLight = currentLight + deltaLight;
  }

  //constrain to minimum and maximum values
  currentLight = constrain(currentLight, offLight, onLight);

  // change the analog out value:
  //  analogWrite(analogOutPin, currentLight);

  //send midi value to max patch
  usbMIDI.sendControlChange(sendControl, sendValue, sendChannel);

}
