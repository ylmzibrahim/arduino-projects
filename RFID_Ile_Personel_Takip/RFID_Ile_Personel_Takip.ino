//#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

String ag_adi = "deneme";
String ag_sifresi = "10203040";
String data;
String server = "yourServer"; // www.example.com
String uri = "yourURI"; // our example is /esppost.php
int rxPin = 3;
int txPin = 2;

const short personel_sayisi = 5;
byte ID[personel_sayisi][4] = {{58, 24, 214, 129}, {153, 24, 132, 156}, {0,0,0,0}, {1,1,1,1}};
String personel[personel_sayisi] = {"Ibrahim Yilmaz", "Mustafa", "Ali", "Veli"};

int RST_PIN = 9;
int SS_PIN = 10;
int buzzerPin = 4;

LiquidCrystal lcd(8, 7, 6, 5, A0, A1);
MFRC522 rfid(SS_PIN, RST_PIN);
SoftwareSerial esp(rxPin, txPin);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  esp.begin(115200);
  connectWifi();
  //  reset();
  //  connectWifi_1();

  pinMode(buzzerPin, OUTPUT);
  rfid.PCD_Init();
  lcd.begin(16, 2);
  lcd.print("Hosgeldiniz...");
  
}

void loop() {
  
  kartOnay();
}

void idYazdir(){
  Serial.print("ID Numarasi: ");
  for(int i=0; i<4; i++){
    Serial.print(rfid.uid.uidByte[i]);
    Serial.print(" ");
  }
  Serial.println("");
}

void kimlikYazdir(String personel_isim){
  Serial.println("Giris Basarili! (" + personel_isim + ")");
  lcd.setCursor(0, 0);
  lcd.print("Giris Basarili");
  lcd.setCursor(0, 1);
  lcd.print(personel_isim);
  {
    tone(buzzerPin, 1000);
    delay(200);
    noTone(buzzerPin);
    delay(20);
    tone(buzzerPin, 1000);
    delay(200);
    noTone(buzzerPin);
  }
  delay(2500);
  lcd.clear();
}

void kartOnay(){
  if(!rfid.PICC_IsNewCardPresent())
    return;
  if(!rfid.PICC_ReadCardSerial())
    return;
  bool personel_OK = false;
  for(int i=0; i<personel_sayisi; i++){
    if(rfid.uid.uidByte[0] == ID[i][0] && rfid.uid.uidByte[1] == ID[i][1] && rfid.uid.uidByte[2] == ID[i][2] && rfid.uid.uidByte[3] == ID[i][3])
    {
      kimlikYazdir(personel[i]);
      idYazdir();
      personel_OK = true;
      break;
    }
  }
  if( !personel_OK ){
    Serial.println("Bilinmeyen Kart");
    lcd.clear();
    lcd.print("Bilinmeyen Kart");
    {
      tone(buzzerPin, 2000);
      delay(400);
      noTone(buzzerPin);
    }
    delay(2500);
    lcd.clear();
    idYazdir(); 
  }
  rfid.PICC_HaltA();
}

void connectWifi(){
  esp.println("AT");
  Serial.println("AT Yollandi");
  while(!esp.find("OK")){
    Serial.println("ESP8266 Bulunamadı");
    esp.println("AT");
  }
  Serial.println("OK Komutu alındı");
  esp.println("AT+CWMODE=1");
  while(!esp.find("OK")){
    Serial.println("Islem Yapiliyor...");
    esp.println("AT+CWMODE=1");
  }
  Serial.println("Client moda gecildi");
  esp.println("AT+CWJAP=\""+ag_adi+"\",\""+ag_sifresi+"\"");
  while(!esp.find("OK"));
  Serial.println("Aga baglandi");
  delay(1000);   
}

//reset the esp8266 module
void reset() {
  esp.println("AT+RST");
  delay(1000);
  if(esp.find("OK") ) Serial.println("Module Reset");
}

//connect to your wifi network
void connectWifi_1() {
  String cmd = "AT+CWJAP=\"" + ag_adi +"\",\"" + ag_sifresi + "\"";
  esp.println(cmd);
  delay(4000);
  if(esp.find("OK")) {
    Serial.println("Connected!");
  }
  else {
    Serial.println("Cannot connect to wifi"); 
    connectWifi();
  }
}
