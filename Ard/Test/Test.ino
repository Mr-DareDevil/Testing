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


MFRC522 rfid(SS_PIN, RST_PIN);  
MFRC522::MIFARE_Key key;          
int block = 2;   
byte readbackblock[18];
const int rs = 12, en = 10, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
byte nuidPICC[4];
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
   rfid.PCD_Init();       
   
   for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;  
    }

  //LCD
  lcd.begin(16, 2);
  lcd.clear();
}

void hard_left(){

       digitalWrite(motorLpin1, LOW);
      digitalWrite(motorLpin2, HIGH);
      digitalWrite(motorRpin1, HIGH);
      digitalWrite(motorRpin2, LOW);
      
}

void soft_left(){
  digitalWrite(motorLpin1, LOW);
      digitalWrite(motorLpin2, LOW);
      digitalWrite(motorRpin1, HIGH);
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
      
      digitalWrite(motorLpin1, HIGH);
      digitalWrite(motorLpin2, LOW);
      digitalWrite(motorRpin1, LOW);
      digitalWrite(motorRpin2, HIGH);
}

void soft_right(){
     digitalWrite(motorLpin1, HIGH);
      digitalWrite(motorLpin2, LOW);
      digitalWrite(motorRpin1, LOW);
      digitalWrite(motorRpin2, LOW);
}

void stopMotion(){
      digitalWrite(motorLpin1, LOW);
      digitalWrite(motorLpin2, LOW);
      digitalWrite(motorRpin1, LOW);
      digitalWrite(motorRpin2, LOW);
}



void full_left(){

      hard_left();
      delay(200);
      stopMotion();
      delay(200);
}

void full_right(){
      
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
      soft_right();
      delay(50);
      stopMotion();
      delay(100);
}

void correct_right(){
      backward();
      delay(70);
      soft_left();
      delay(50);
      stopMotion();
      delay(100);
}


int countNode=1;
int rfidTable[6][6];
int rfidTemp[7];
int row=4, col=4;
int tempInd=0;

void readRfid()
{
if ( ! rfid.PICC_IsNewCardPresent())
    return;
  
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    } 
    int blockNumber =2;
    byte buffersize=18;
    int largestModulo4Number=blockNumber/4*4;
    int trailerBlock=largestModulo4Number+3;//determine trailer block for the sector
    byte status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(rfid.uid));
    rfid.MIFARE_Read(blockNumber, readbackblock, &buffersize);
    getCoord();
    rfidTable[row][col]=readbackblock[0]-48;
    rfidTemp[tempInd]=readbackblock[0]-48;

    
    lcd.setCursor(0, 1);
    lcd.print(readbackblock[0]-48);
    tempInd++;
    countNode++;
    if(tempInd==6){
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("READINGS:");
        for(int i=0; i<6; i++){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(rfidTemp[i]);
          delay(2000);
        }
        tempInd=0;
      }
    }

    
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();  
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



void getCoord(){
  int val=countNode-1;
  row=4-(val/5);
  if((val/5)%2==0){
    col=val%5;
  }else{
    col=4-val%5;
  }
}

void loop() { 
//FIRST RUN
while(1){
    scanIrSensorBW();
     readRfid();
    if(irValues=="11111111") stopMotion();
    else if(irValues[7]=='0' && irValues[6]=='0' && irValues[5]=='0'){
      lcd.setCursor(0, 0);
      lcd.print(countNode);
      forward();
      delay(100);
      if(countNode==4 || countNode == 5 || countNode == 14 || countNode ==15){
        full_left();
      }else if(countNode==9 || countNode == 10 || countNode == 19 || countNode ==20){
        full_right();
      }else{
        full_forward();
      }
      if(countNode==24){
        stopMotion();
        delay(20000);
        break;
      }    
    }else if(irValues[0]=='0' && irValues[1]=='0' && irValues[2]=='0'){
      lcd.setCursor(0, 0);
      lcd.print(countNode);
      forward();
      delay(100);
      if(countNode==4 || countNode == 5 || countNode == 14 || countNode ==15){
        full_left();
      }else if(countNode==9 || countNode == 10 || countNode == 19 || countNode ==20){
        full_right();
      }else{
        full_forward();
      }
      if(countNode==24){
        stopMotion();
        delay(20000);
        break;
      }       
    }else if(irValues[7]=='0' || irValues[6]=='0'){
      correct_left();
    }else if(irValues[0]=='0' || irValues[1]=='0'){
      correct_right();
    }else{
      full_forward();
    } 
}



}
