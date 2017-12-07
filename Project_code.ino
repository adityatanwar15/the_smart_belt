#include "TinyGPS++.h"
#include "SoftwareSerial.h"
SoftwareSerial mySerial(9, 10);//Rx=pin9, TX = pin10 for sim900a95

SoftwareSerial serial_connection(12, 11); //RX=pin 12, TX=pin 11 for GPS
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data


const int trigpin1 = 0;
const int echopin1 = 1;
const int trigpin2 = 2;
const int echopin2 = 3;
const int trigpin3 = 4;
const int echopin3 = 5;
const int pinBuz = 6;
const int pin1Swi = 7; // Push-button pin
const int pin2Swi = 8;
int distance1 = 0;
int distance2 = 0;
int distance3 = 0;
long duration1;
long duration2;
long duration3;
void setup() {
  // put your setup code here, to run once:
  pinMode(trigpin1,OUTPUT);
  pinMode(echopin1,INPUT);
  pinMode(trigpin2,OUTPUT);
  pinMode(echopin2,INPUT);
  pinMode(trigpin3,OUTPUT);
  pinMode(echopin3,INPUT);
  pinMode(pinBuz,OUTPUT);
  pinMode(pin1Swi,INPUT);
  Serial.begin(9600);
  Serial.begin(9600);
  serial_connection.begin(9600);//This opens up communications to the GPS
  mySerial.begin(9600);   // Setting the baud rate of GSM Module
  

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigpin1,LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigpin1,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin1,LOW);

  duration1 = pulseIn(echopin1, HIGH);
  distance1 = duration1*0.034/2;
  Serial.print(distance1);
  if(distance1<150){
    digitalWrite(pinBuz,1);
  }
  else{
    digitalWrite(pinBuz,0);
  }
  digitalWrite(trigpin1,LOW);
  delayMicroseconds(2);
  Serial.print(" ");
  

  int botao; // To save the last logic state of the button
  botao = digitalRead(pin1Swi); //Put the reading value of the switch on botao
  Serial.println(botao); //Shows the logic state of the input on Serial Monitor
  if (botao == 1) // Pressed button, logic State HIGH (5V)
  {
    digitalWrite(pinBuz,1); //Switch pressed, buzzer on
    
    while(botao==1){
      botao = digitalRead(pin1Swi);
      Serial.println(botao);
    }
    digitalWrite(pinBuz,0);
  }

  digitalWrite(trigpin2,LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigpin2,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin2,LOW);

  duration2 = pulseIn(echopin2, HIGH);
  distance2 = duration2*0.034/2;
  Serial.print(distance2);
  if(distance2<150){
    digitalWrite(pinBuz,1);
    delay(100);
  }
  else{
    digitalWrite(pinBuz,0);
    delay(100);
  }
  digitalWrite(trigpin2,LOW);
  delayMicroseconds(2);
  Serial.print(" ");
  
  digitalWrite(trigpin3,LOW);
  delayMicroseconds(2);
  
  
  digitalWrite(trigpin3,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin3,LOW);

  duration3 = pulseIn(echopin3, HIGH);
  distance3 = duration3*0.034/2;
  Serial.println(distance3);
  if(distance3<150){
    digitalWrite(pinBuz,1);
    delay(100);
  }
  else{
    digitalWrite(pinBuz,0);
    delay(100);
  }
  digitalWrite(trigpin3,LOW);
  delayMicroseconds(2);

botao = digitalRead(pin2Swi); //Put the reading value of the switch on botao
  if (botao == 1) // Pressed button, logic State HIGH (5V)
  {
    while(serial_connection.available())//While there are characters to come from the GPS
    {
      gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time
    }
    if(gps.location.isUpdated())//This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
    {
      //Get the latest info from the gps object which it derived from the data sent by the GPS unit
      /*
       * has to be updated when sim900 code came
      */
      mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
      delay(1000);  // Delay of 1000 milli seconds or 1 second
      mySerial.println("AT+CMGS=\"+91\"\r"); // Replace x with mobile number
      delay(1000);

      mySerial.println("Satellite Count:");
      mySerial.println(gps.satellites.value());
      mySerial.println("Latitude:");
      mySerial.println(gps.location.lat(), 6);
      mySerial.println("Longitude:");
      mySerial.println(gps.location.lng(), 6);
      mySerial.println("Speed MPH:");
      mySerial.println(gps.speed.mph());
      mySerial.println("Altitude Feet:");
      mySerial.println(gps.altitude.feet());
      mySerial.println("");
      delay(100);
      mySerial.println((char)26);// ASCII code of CTRL+Z
      delay(1000);
    }

   if (mySerial.available()>0)
   Serial.write(mySerial.read());
    }
    }


