#include <Servo.h>

Servo lidServo;

// Pin settings
int trigOut = 2;
int echoOut = 3;

int trigIn = 4;
int echoIn = 5;

int ledPin = 7;    // LED bulb positive terminal
int servoPin = 6;  // Servo signal pin

// Distance threshold
int personDistance = 20;   // cm (outside sensor)
int fullDistance   = 5;    // cm (inside sensor)

long getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH);
  long distance = duration * 0.034 / 2;   // convert to cm

  return distance;
}

void setup() {
  Serial.begin(9600);

  pinMode(trigOut, OUTPUT);
  pinMode(echoOut, INPUT);

  pinMode(trigIn, OUTPUT);
  pinMode(echoIn, INPUT);

  pinMode(ledPin, OUTPUT);

  lidServo.attach(servoPin);
  lidServo.write(0);   // lid closed
}

void loop() {

  // Read ultrasonic distances
  long outsideDistance = getDistance(trigOut, echoOut);
  long insideDistance  = getDistance(trigIn, echoIn);

  
  Serial.print("Outside Distance: ");
  Serial.print(outsideDistance);
  Serial.print(" cm | Inside Distance: ");
  Serial.print(insideDistance);
  Serial.println(" cm");

  // --- Outside sensor: detects person ---
  if (outsideDistance < personDistance && outsideDistance > 0) {
    lidServo.write(90);          // open lid
  } else {
    lidServo.write(0);           // close lid
  }

  // --- Inside sensor: garbage level ---
  if (insideDistance < fullDistance && insideDistance > 0) {
    digitalWrite(ledPin, HIGH);  // bin full → LED on
  } else {
    digitalWrite(ledPin, LOW);   // not full → LED off
  }

  delay(300);
}
