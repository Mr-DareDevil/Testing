#include <LiquidCrystal.h>
#include <SPI.h>      
#include <MFRC522.h> 

const int SS_PIN = 53;
const int RST_PIN = 8;
const int motorRpin1 = 36;
const int motorRpin2 = 38;
const int motorLpin2 = 40;
const int motorLpin1 = 42;
const int irPins[8]={31, 33, 35, 37, 39, 41, 43, 45};
const int irSensorDigital[]={0, 0, 0, 0, 0, 0, 0, 0};
int irSensors = B00000000;
String irValues = "00000000";


MFRC522 mfrc522(SS_PIN, RST_PIN);  
MFRC522::MIFARE_Key key;          
int block = 2;   
byte readbackblock[18];
const int rs = 12, en = 10, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



void setup() {
    Serial.begin(9600);

    //Motors Pin Mode
    pinMode(motorRpin1, OUTPUT);
    pinMode(motorRpin2, OUTPUT);
    pinMode(motorLpin1, OUTPUT);
    pinMode(motorLpin2, OUTPUT);

    //IR sensors Pin Mode
    for(int i=0; i<8; i++){
      pinMode(irPins[i], INPUT);
    }
    
   //RFID 
   SPI.begin();               
   mfrc522.PCD_Init();       
   
   for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;  
    }

  //LCD
  lcd.begin(16, 2);
  lcd.clear();
}

void hard_left(){
      digitalWrite(motorLpin1, HIGH);
      digitalWrite(motorLpin2, LOW);
      digitalWrite(motorRpin1, LOW);
      digitalWrite(motorRpin2, HIGH);
}

void soft_left(){
      digitalWrite(motorLpin1, HIGH);
      digitalWrite(motorLpin2, LOW);
      digitalWrite(motorRpin1, LOW);
      digitalWrite(motorRpin2, LOW);
}

void forward(){
      digitalWrite(motorLpin1, HIGH);
      digitalWrite(motorLpin2, LOW);
      digitalWrite(motorRpin1, HIGH);
      digitalWrite(motorRpin2, LOW);
}

void backward(){
      digitalWrite(motorLpin1, LOW);
      digitalWrite(motorLpin2, HIGH);
      digitalWrite(motorRpin1, LOW);
      digitalWrite(motorRpin2, HIGH);
}

void hard_right(){
      digitalWrite(motorLpin1, LOW);
      digitalWrite(motorLpin2, HIGH);
      digitalWrite(motorRpin1, HIGH);
      digitalWrite(motorRpin2, LOW);
}

void soft_right(){
      digitalWrite(motorLpin1, LOW);
      digitalWrite(motorLpin2, LOW);
      digitalWrite(motorRpin1, HIGH);
      digitalWrite(motorRpin2, LOW);
}

void stopMotion(){
      digitalWrite(motorLpin1, LOW);
      digitalWrite(motorLpin2, LOW);
      digitalWrite(motorRpin1, LOW);
      digitalWrite(motorRpin2, LOW);
}



void full_left(){
      forward();
      delay(100);
      hard_left();
      delay(200);
      stopMotion();
      delay(200);
}

void full_right(){
      forward();
      delay(100);
      hard_right();
      delay(200);
      stopMotion();
      delay(200);
}

void full_forward(){
      forward();
      delay(50);
      stopMotion();
      delay(150);
}

void correct_left(){
      backward();
      delay(70);;
      soft_left();
      delay(50);
      stopMotion();
      delay(100);
}

void correct_right(){
      backward();
      delay(70);
      soft_right();
      delay(50);
      stopMotion();
      delay(100);
}

void scanIrSensor(){
  for(byte count =0; count<8; count++){
    bitWrite(irSensors, count, !digitalRead(irPins[count]));
    irValues[count]=digitalRead(irPins[count])+'0';
  }
}

void scanIrSensorBW(){
  for(byte count =0; count<8; count++){
    bitWrite(irSensors, count, !digitalRead(irPins[count]));
    irValues[count]=!digitalRead(irPins[count])+'0';
  }
}



void loop() {

  scanIrSensorBW();
    Serial.println(irValues);
   
  if((irValues[0]=='0' || irValues[7]=='0')&& (1==0)){
    Serial.println("Cross-Section");   
  }else{
    int sumLeft=0, sumRight=0;
    for(int count=0; count<8; count++){
      if(count<=3) sumLeft+=irValues[count];
      else sumRight+=irValues[count];
    }
    if(irValues=="11111111") stopMotion();
    else if(irValues[7]=='0' && irValues[6]=='0' && irValues[5]=='0'){
      full_left();
    }else if(irValues[0]=='0' && irValues[1]=='0' && irValues[2]=='0'){
      full_right();
    }
    else if(irValues[7]=='0' || irValues[6]=='0'){
      correct_left();
    }else if(irValues[0]=='0' || irValues[1]=='0'){
      correct_right();
    }
    
    else{
      full_forward();
    }
  }
    

}
