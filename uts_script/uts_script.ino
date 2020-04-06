#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


#define SS_PIN 2  //D2
#define RST_PIN 0 //D1
#define sensorLDR A0
#define lampu 10

int nilaiSensor;
int nilaiSuhu;
int sensorSuhu = 16; //sesuaikan dengan GPIO pin yang anda gunakan
int status = 0;
dht DHT;


MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

byte termometru[8] = //icon for termometer
{
 B00100,
 B01010,
 B01010,
 B01110,
 B01110,
 B11111,
 B11111,
 B01110
};

void setup()
{
  pinMode(lampu, OUTPUT);
  digitalWrite(lampu, LOW);
  Serial.begin(115200); // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  lcd.init();
  lcd.backlight();
  lcd.home();
  lcd.createChar(1,termometru);

  lcd.clear();
  lcd.print("Waiting card...");
}

void loop()
{
  if(status == 1){
    DHT.read11(sensorSuhu);
    lcd.setCursor (0,0);
    lcd.print("ID Terverivikasi");
    //Kolom ke 0 baris ke 2
    lcd.setCursor (0,1);
    //munculkan icon
    lcd.write(1);
    // Kolom ke 2 baris ke 2
    int temp = DHT.temperature;
    
    lcd.setCursor (2,1);
    lcd.print(temp);
    lcd.print((char)223);
    lcd.setCursor (10,1);
    if(temp > 29){
     lcd.clear();
    lcd.setCursor (0,0);
    lcd.print("ID Terverivikasi");
    //Kolom ke 0 baris ke 2
    lcd.setCursor (0,1);
    //munculkan icon
    lcd.write(1);
     lcd.setCursor (2,1);
     lcd.print(temp);
     lcd.print((char)223);
     lcd.setCursor (10,1);
     lcd.print("AC ON"); 
    } else {
      lcd.print("AC OFF");
    }
    nilaiSensor = analogRead(sensorLDR);
     if(nilaiSensor > 800){
       digitalWrite(lampu, HIGH);
       } else {
         digitalWrite(lampu,LOW);
  }
  }
  lcd.home();
  

  delay(1000);

  delay(1000);
  
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  delay(500);
  
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "09 AD 93 99") //change here the UID of the card/cards that you want to give access
  { 
    if(status == 0){
    status = 1;
//    DHT.read11(sensorSuhu);
//    lcd.setCursor (0,0);
//    lcd.print("ID Terverivikasi");
//    //Kolom ke 0 baris ke 2
//    lcd.setCursor (0,1);
//    //munculkan icon
//    lcd.write(1);
//    // Kolom ke 2 baris ke 2
//    lcd.setCursor (2,1);
//    lcd.print(DHT.temperature);
//    lcd.print((char)223);
//    lcd.setCursor (10,1);
    }
    else{
      status = 0;
      lcd.clear();
      lcd.print("Unplug Card");
      
    }

  }
 
 else {
   Serial.println(" Access Incorrect");
   lcd.print("Access Incorrect");

   delay(3000);
   lcd.clear();
   delay(200);
  }
  
}



// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void dinamis(){
        if(nilaiSensor > 800){
       digitalWrite(lampu, LOW);
       } else {
         digitalWrite(lampu,HIGH);
  }
}

void scrollText(int row, String message, int delayTime, int lcdColumns)
{
  for (int i = 0; i < lcdColumns; i++)
  {
    message = " " + message;
  }
  message = message + " ";
  for (int pos = 0; pos < message.length(); pos++)
  {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}
