  //rfid vars
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.


  //lift vars
#include <AccelStepper.h>

#define dirPin 7
#define stepPin 6
#define motorInterfaceType 1

AccelStepper myStepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

int liftSpeed = 500;
int fl1 = 0;
int fl2 = 420;
int fl3 = 800;
int currentflr =1;

  //but vars
int but1 = 2;
boolean State1 = LOW; // button
boolean pState1 = State1; //button

int but2 = 3;
boolean State2 = LOW; // button
boolean pState2 = State2; //button

int but3 = 4;
boolean State3 = LOW; // button
boolean pState3 = State3; //button


void setup() {

  //rfid setup
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  //stepper setup
  myStepper.setMaxSpeed(1000);

  //buttons setup
     pinMode(but1, INPUT);
     pinMode(but2, INPUT);
     pinMode(but3, INPUT);
}
//end setup

void loop() {

butPress();
rfidScan();

}
//end loop


void butPress (){
  
    //Floor 1
  State1 = (debounce(but1, pState1));
  if (State1 == HIGH && pState1 == LOW){
            while (myStepper.currentPosition() >= fl1){
            myStepper.setSpeed(-liftSpeed);
            myStepper.runSpeed();
            }
      }
   pState1 = State1;

    //Floor 2
  State2 = (debounce(but2, pState2));
  if (State2 == HIGH && pState2 == LOW){
      if (myStepper.currentPosition() <= fl1){
            while (myStepper.currentPosition() <= fl2){
            myStepper.setSpeed(liftSpeed);
            myStepper.runSpeed();
            }
      }   
      if (myStepper.currentPosition() >= fl3){
            while (myStepper.currentPosition() >= fl2){
            myStepper.setSpeed(-liftSpeed);
            myStepper.runSpeed();
            }
      }  
  }
   pState2 = State2;

    //Floor 3
  State3 = (debounce(but3, pState3));
  if (State3 == HIGH && pState3 == LOW){
      if (myStepper.currentPosition() <= fl3){
            while (myStepper.currentPosition() <= fl3){
            myStepper.setSpeed(liftSpeed);
            myStepper.runSpeed();
            }
      }   
      else if (myStepper.currentPosition() >= fl3){
            while (myStepper.currentPosition() >= fl1){
            myStepper.setSpeed(-liftSpeed);
            myStepper.runSpeed();
            }
      }     
  }
   pState3 = State3;
}
//end buttons function


void rfidScan(){

   if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  //Show UID on serial monitor
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  content.toUpperCase();
  if (content.substring(1) == "99 C2 ED 97") {
      if (myStepper.currentPosition() <= fl1){
        currentflr = 1;
      }
      if (myStepper.currentPosition() >= fl3){
        currentflr = 3;
      }
      if (myStepper.currentPosition() < fl3 && myStepper.currentPosition() > fl1) {
        currentflr = 2;
      }     

      if (currentflr == 1){
            while (myStepper.currentPosition() <= fl3){
            myStepper.setSpeed(liftSpeed);
            myStepper.runSpeed();
            }
      }
            if (currentflr == 2){
            while (myStepper.currentPosition() >= fl1){
            myStepper.setSpeed(-liftSpeed);
            myStepper.runSpeed();
            }
      }
            if (currentflr == 3){
            while (myStepper.currentPosition() >= fl2){
            myStepper.setSpeed(-liftSpeed);
            myStepper.runSpeed();
            }
      }


    delay(2000);
  }
  else   {
    delay(1000);
  }
  
}
//end rfid function


boolean debounce (int butPin, int prevState){
  
  boolean butState = digitalRead(butPin);
  
  if (butState == HIGH && prevState == LOW){
    delay(75);
   // State = !State;
  }
 
  return butState;
}
//end debounce
