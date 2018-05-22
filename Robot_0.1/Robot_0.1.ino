//Radar voor
const int echoVoor = 24;
const int trigVoor = 25;

//Radar achter
const int echoAchter = 38;
const int trigAchter = 39;

// motor A:
const int enA = 10;
const int in1 = 9;
const int in2 = 8;

// motor B:
const int enB = 5;
const int in3 = 7;
const int in4 = 6;

//kleursensor voor
const int vs0 = 26; 
const int vs1 = 27; 
const int vs2 = 28; 
const int vs3 = 29; 
const int vout = 30; 

//kleursensor onder
const int s0 = 33; 
const int s1 = 34; 
const int s2 = 35; 
const int s3 = 36; 
const int out = 37; 

//lijnsensor1
const int pinIRd = 31; 

//lijnsensor2
const int pinIRd2 = 32;

//servo
#include <Servo.h>
Servo myservo;

long duur;
float afstand;
int frequency = 0;
int pos = 0;

int blikje = 0;

int red = 0; 
int green = 0; 
int blue = 0; 
int black = 0;

//functies
void rotate(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in4, HIGH);
  digitalWrite(in3, LOW);
  analogWrite(enA, 110);
  analogWrite(enB, 110);
  }

void turnServo() {
  if(pos == 180) {
    for(pos = 180; pos > 0; pos -= 2) {
      myservo.write(pos); 
      delay(15);
    }
  }
  else {
    for(pos = 0; pos < 180; pos += 2) {
      myservo.write(pos); 
      delay(15);
    }
  }
}

void achteruit(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 110);
  analogWrite(enB, 110);
}

void vooruit(){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enA, 110);
    analogWrite(enB, 110);
  }

void stoppen(){
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

void links(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 110);
  analogWrite(enB, 110);
}

void rechts(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 110);
  analogWrite(enB, 110);
}

float getRadarVoor() {
  digitalWrite(trigVoor, LOW);
  delayMicroseconds(2);
  digitalWrite(trigVoor, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigVoor, LOW);
  duur = pulseIn(echoVoor, HIGH);
  return duur / 58.2;  
}

float getRadarAchter() {
  digitalWrite(trigAchter, LOW);
  delayMicroseconds(2);
  digitalWrite(trigAchter, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigAchter, LOW);
  duur = pulseIn(echoAchter, HIGH);
  return duur / 58.2;  
}
void bijsturen() {
  stoppen();
  //delay(1000);
  achteruit();
  //delay(500);
  rotate();
  //delay(200);  
}

int getRed(){
  // Setting red filtered photodiodes to be read
  digitalWrite(vs2,LOW);
  digitalWrite(vs3,LOW);
  // Reading the output frequency
  frequency = pulseIn(vout, LOW);
  // Printing the value on the serial monitor
  frequency = map(frequency, 62, 340, 255, 0);
  //Serial.print("R= ");//printing name
  //Serial.print(frequency);//printing RED color frequency
  //Serial.println("  ");
  return frequency;
}
void color() 
{   
  digitalWrite(s2, LOW); 
  digitalWrite(s3, LOW); 
  //count OUT, pRed, RED 
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH); 
  digitalWrite(s3, HIGH); 
  //count OUT, pBLUE, BLUE 
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH); 
  digitalWrite(s2, HIGH); 
  //count OUT, pGreen, GREEN 
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH); 
}

void setup() {
  Serial.begin(9600);
  //Radar
  pinMode(echoVoor, INPUT);
  pinMode(trigVoor, OUTPUT);

  pinMode(echoAchter, INPUT);
  pinMode(trigAchter, OUTPUT);
  
  //Motorboard
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  //Kleursensor
  pinMode(s0, OUTPUT); 
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 
  pinMode(s3, OUTPUT); 
  pinMode(out, INPUT);

  pinMode(vs0, OUTPUT); 
  pinMode(vs1, OUTPUT); 
  pinMode(vs2, OUTPUT); 
  pinMode(vs3, OUTPUT); 
  pinMode(vout, INPUT);
  digitalWrite(s0, HIGH); 
  digitalWrite(s1, LOW); 

  //lijnsensor1
  pinMode(pinIRd, INPUT);

  //lijnsensor2
  pinMode(pinIRd2, INPUT);

  //servo
  myservo.attach(13);
}

void loop() { 
  
  //color();
//  Serial.print("R Intensity:"); 
//  Serial.print(red, DEC); 
//  Serial.print(" G Intensity: "); 
//  Serial.print(green, DEC); 
//  Serial.print(" B Intensity : "); 
//  Serial.print(blue, DEC); 
  
  //Radar
  afstand = getRadarVoor();
  //afstand = getRadarAchter();
  Serial.println(afstand);
  
  if (afstand < 60 && afstand != 0) {
    blikje = 1;
    Serial.println("blikje");
    stoppen();
  }
  
  if(blikje == 0) {
    rotate();
    Serial.println("rotate");
    return;
  }

  if(blikje && !digitalRead(pinIRd) && !digitalRead(pinIRd2)){ 
    if (afstand > 8) {
      vooruit();
      Serial.println("vooruit");
    }
    else {
      stoppen();
      Serial.println("STOP!");

      red = getRed();
      Serial.print("RED: ");
      Serial.println(red);
      if(red > 250) {
        Serial.println("Draaien");
        turnServo();
      }
      
      blikje = 0;
      
      /*if (red < blue && red < green && red < 20){ 
        //Serial.println(" - (Red Color)");
        achteruit(); 
      }*/
    }
  }  
  else{
    //Serial.println("motor stop");
    bijsturen();
    Serial.println("bijsturen");
  }
} 

