#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

int RST_PIN = 9;
int SS_PIN = 10;
int servoPin = 8;

Servo motor;
MFRC522 rfid(SS_PIN, RST_PIN);
byte ID_1[4] = {58, 24, 214, 129};
byte ID_2[4] = {153, 24, 132, 156};

void setup() {
  motor.attach(servoPin);
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  if(!rfid.PICC_IsNewCardPresent())
    return;
  if(!rfid.PICC_ReadCardSerial())
    return;
  if((rfid.uid.uidByte[0] == ID_1[0] && rfid.uid.uidByte[1] == ID_1[1] && 
      rfid.uid.uidByte[2] == ID_1[2] && rfid.uid.uidByte[3] == ID_1[3]) ||
     (rfid.uid.uidByte[0] == ID_2[0] && rfid.uid.uidByte[1] == ID_2[1] && 
      rfid.uid.uidByte[2] == ID_2[2] && rfid.uid.uidByte[3] == ID_2[3])){
    Serial.println("Servo calisti");
    ekranaYaz();
    motor.write(180);
    delay(3000);
    motor.write(0);
    delay(1000);
  }else{
    Serial.println("Yetkisiz Kart");
    ekranaYaz(); 
  }
  rfid.PICC_HaltA();
}

void ekranaYaz(){
  Serial.print("ID Numarasi: ");
  for(int i=0; i<4; i++){
    Serial.print(rfid.uid.uidByte[i]);
    Serial.print(" ");
  }
  Serial.println("");
}
