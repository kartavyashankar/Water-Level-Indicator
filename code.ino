#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Blynk.h>
char auth[] = "HDtQEReNU4j1BPYyB0ObFWYvJpdvU0ss";


char ssid[] = "kartavya";
char pass[] = "password";

bool isFirstConnect = true;

BLYNK_CONNECTED() {
  if (isFirstConnect) {
    // Requesting Blynk server to re-send latest values for all pins
    Blynk.syncAll();
    isFirstConnect = false;  // Made it false so that it does not sync after every reconnection
  }
}

WidgetLCD lcd(V1);

const int echoPin = 5; //D1
const int trigPin = 4; //D2
int iPin = 0; // High water alert
int lpin = 2; // Low water alert

long duration; 
int distance; 
int buttonPressed = 0; // Corresponds to V4
float vol;

void setup()
{ 
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(iPin, OUTPUT);
  pinMode(lpin, OUTPUT);
  digitalWrite(iPin, LOW);
  digitalWrite(lpin, LOW);
  Blynk.begin(auth, ssid, pass);
  lcd.clear(); 
  lcd.print(0, 0, "Empty space : "); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  
}

void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  duration = pulseIn(echoPin, HIGH);
  distance = duration/58;
  // vol = 46.791*46.791*distance*3.14/1000;
  vol = 390*distance/1000; 
  if(vol<=5)
  {
    digitalWrite(iPin, HIGH);
    if(buttonPressed == 1)
    {
      Blynk.notify("Tank about to be full");
    }
    digitalWrite(lpin, LOW);
  }
  else if(vol>=30)
  {
    if(buttonPressed == 1)
    {
      Blynk.notify("You're running LOW on water supply!");
    }
    digitalWrite(lpin, HIGH);
    digitalWrite(iPin, LOW);
  }
  else
  {
    digitalWrite(iPin, LOW);
    digitalWrite(lpin, LOW);
    Blynk.virtualWrite(V4, HIGH);
  }
  
  lcd.print(0, 1, distance);
  vol = 35-vol;
  Blynk.virtualWrite(V2, vol);
  Blynk.run();
  delay(500);
}
BLYNK_WRITE(V4){
 int pinValue = param.asInt(); // Assigning incoming value from pin V4 to a variable
 if (pinValue == 1) {
    buttonPressed = 1;
  } else {
    buttonPressed = 0;
  }
}
