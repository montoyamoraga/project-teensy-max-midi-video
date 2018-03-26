int sensorPin = 9;
unsigned long duration;

void setup()
{
  pinMode(sensorPin, INPUT);
}

void loop()
{
  duration = pulseIn(sensorPin, HIGH);
}
