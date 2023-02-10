#include <SPI.h>                              // biblioteca SPI 
#include <MFRC522.h>                          // biblioteca do modulo RC522

#define SDA_PIN 10                            // pino SDA RC522 = D10 Arduino
#define RST_PIN 9                             // pino RST RC522 = D9  Arduino

const int pinLEDG = 6;
const int pinLEDB = 5;
const int pinLEDR = 3;

MFRC522 mfrc522(SDA_PIN, RST_PIN);    // cria uma instância MFRC522
String UID = "";                      // string da identificação UID
String TAGLiberada = " 12 37 A1 7B";  // UID da etiqueta NFC liberada 

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();  // mostra as informções do modulo RC522

  pinMode(pinLEDG,OUTPUT);
  pinMode(pinLEDB,OUTPUT);
  pinMode(pinLEDR,OUTPUT);

  Serial.println("iniciado!!!");  
}

void loop()
{
  analogWrite(pinLEDB, 2);

  if ( ! mfrc522.PICC_IsNewCardPresent()){return;}   // verifica a presença de etiqueta

  if ( ! mfrc522.PICC_ReadCardSerial()){return;}    // leitura da etiqueta NFC

  UID = "";
  getUID();
  Serial.println(UID);

  if (TAGLiberada == UID)
  {
    analogWrite(pinLEDB, 0);

    Serial.println("Liberado!");

    analogWrite(pinLEDG, 3);
    delay(500);
    analogWrite(pinLEDG, 0);
  }
  else
  {
    analogWrite(pinLEDB, 0);

    Serial.println("Negado!");

    analogWrite(pinLEDR, 3);
    delay(500);
    analogWrite(pinLEDR, 0);
  }
}

void getUID()
{
  for (byte i = 0; i < mfrc522.uid.size; i++)                   // leitura da identificação UID da NFC
  {
    if ( mfrc522.uid.uidByte[i] < 0x10)                         // se byte menor do que 16
      UID.concat(" 0");                                         // insere um zero
    else                                                        // senão
      UID.concat(" ");                                          // insere um espaço
    
    UID.concat(String(mfrc522.uid.uidByte[i], HEX));            // concatena todos os bytes da UID
  }

  UID.toUpperCase();
}