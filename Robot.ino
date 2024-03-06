#include <Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial BTserial(10, 11);
// Pinii motor 1
#define mpin00 5
#define mpin01 6
// Pinii motor 2
#define mpin10 3
#define mpin11 2
int trigPin = 9;    // Trigger
int echoPin = 7;    // Echo

char Incoming_value;

Servo srv;
void setup() {
  // configurarea pinilor motor ca iesire, initial valoare 0
  digitalWrite(mpin00, 0);
  digitalWrite(mpin01, 0);
  digitalWrite(mpin10, 0);
  digitalWrite(mpin11, 0);
  pinMode (mpin00, OUTPUT);
  pinMode (mpin01, OUTPUT);
  pinMode (mpin10, OUTPUT);
  pinMode (mpin11, OUTPUT);
  // pin LED
  pinMode(13, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600); 
  BTserial.begin(9600);
  delay(500);
  srv.attach(8);

}
// Funcție pentru controlul unui motor
// Intrare: pinii m1 și m2, direcția și viteza
void StartMotor (int m1, int m2, int forward, int speed)
{

  if (speed == 0) // oprire
  {
    digitalWrite(m1, 0);
    digitalWrite(m2, 0);
  }
  else
  {
    if (forward)
    {
      digitalWrite(m2, 0);
      analogWrite(m1, speed); // folosire PWM
    }
    else
    {
      digitalWrite(m1, 0);
      analogWrite(m2, speed);
    }
  }
}
// Funcție de siguranță
// Execută oprire motoare, urmată de delay
void delayStopped(int ms)
{
  StartMotor (mpin00, mpin01, 0, 0);
  StartMotor (mpin10, mpin11, 0, 0);
  delay(ms);
}
void loop() {
  
  if (Serial.available()>0 || BTserial.available() > 0 ) {

    Incoming_value = BTserial.read(); 
    //Incoming_value = Serial.read();      //Read the incoming data and store it into variable Incoming_value
    Serial.write(Incoming_value);
    //Print Value of Incoming_value in Serial monitor
    //    Serial.print("\n");

    if (Incoming_value == 'F') {          //Checks whether value of Incoming_value is equal to 1
      digitalWrite(13, HIGH);  //If value is 1 then LED turns ON
        Serial.println(distanta());
      if (distanta() > 20)
        fata(128, 325);
      digitalWrite(13, LOW);
    }
    else if (Incoming_value == 'B') {     //Checks whether value of Incoming_value is equal to 0
      digitalWrite(13, LOW);   //If value is 0 then LED turns OFF
      spate(128, 250);
    }
    else if (Incoming_value == 'L') {
      digitalWrite(13, LOW);

      srv.write(180);
      delay(700);
      if (distanta() > 20 ) {
        stanga(128, 400);
        delay(100);
        fata(128, 250);
      }
      srv.write(90);
    }
    else if (Incoming_value == 'R') {
      srv.write(0);
      delay(700);
      if (distanta() > 20 ) {
        dreapta(128, 400);
        delay(100);
        fata(128, 250);
      }
      srv.write(90);
    }
    else if (Incoming_value == 'O') {
      srv.write(45);
      delay(700);
      if (distanta() > 20 ) {
        dreapta(128, 250);
        delay(100);
        fata(128, 250);
      }
      srv.write(90);
    }
    else if (Incoming_value == 'N') {
      digitalWrite(13, LOW);

      srv.write(135);
      delay(700);
      if (distanta() > 20 ) {
        stanga(128, 250);
        delay(100);
        fata(128, 250);
      }
      srv.write(90);
    }
  }
}

int distanta() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  int duration = pulseIn(echoPin, HIGH);

  // Convert the time into a distance
  int cm = (duration / 2) / 29.1;
  return cm;
}

void fata(int x, int del) {
  StartMotor (mpin01, mpin00, 1, 255);//right
  StartMotor (mpin11, mpin10, 1, 220);

  delay (del);
  delayStopped(20);
}

void spate(int x, int del) {
  StartMotor (mpin01, mpin00, 0, 255);
  StartMotor (mpin11, mpin10, 0, 255);

  delay (del);
  delayStopped(20);
}

void stanga(int x, int del) {
  StartMotor (mpin00, mpin01, 0, x);
  StartMotor (mpin10, mpin11, 1, x);

  delay (del);
  delayStopped(20);
}

void dreapta(int x, int del) {
  StartMotor (mpin00, mpin01, 1, x);
  StartMotor (mpin10, mpin11, 0, x);

  delay (del);
  delayStopped(20);
}
