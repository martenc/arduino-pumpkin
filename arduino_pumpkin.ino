
#define TRIGPIN 13
#define ECHOPIN 12
#define LEDAMBER 11 //amber
#define LEDRED 10 //red
#define BUZZER 4
#define BUZZER_FREQUENCY 4
#define PROXIMITY_THRESHOLD 85
#define PROXIMITY_CONSECUTIVE_READINGS 3
#define FLICKER_INTERVAL 10

long previousMillis = 0;
long closeReadings = 0;

void setup()
{
  Serial.begin (9600);
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(LEDAMBER, OUTPUT);
  pinMode(LEDRED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  delay(500); // Allow the proximity sensor to initialize
}

void loop()
{
  long duration, distance;
  digitalWrite(TRIGPIN, LOW); 
  digitalWrite(TRIGPIN, HIGH);
  duration = pulseIn(ECHOPIN, HIGH);
  distance = (duration/2) / 29.1;
  
  // This is where the LED On/Off happens
  if (distance < PROXIMITY_THRESHOLD) {  
    closeReadings++;
    // require n consecutive "close" readings before going into EVIL mode. This prevents little blips of the buzzer.
    if (closeReadings >= PROXIMITY_CONSECUTIVE_READINGS) 
    {
      digitalWrite(LEDRED,HIGH); // When the Red condition is met, the Green LED should turn off
      digitalWrite(LEDAMBER,LOW);
      tone(BUZZER, BUZZER_FREQUENCY); //comment this out if it gets too annoying
    }
  }
  else {
    closeReadings = 0;
    digitalWrite(LEDRED,LOW);
    noTone(BUZZER);
    
    // candle flicker - based on Arduino example BlinkWithoutDelay
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > FLICKER_INTERVAL)
    {
      previousMillis = currentMillis;
      analogWrite(LEDAMBER, random(0, 256));
    }
    
  } 
  

  if (distance >= 200 || distance <= 0){
    Serial.println("Out of range");
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
  }
  //delay(50);

}
