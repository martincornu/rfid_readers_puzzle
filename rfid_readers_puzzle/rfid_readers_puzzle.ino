/*
  Rfid reader puzzle.
  created  5 Dec 2019
  modified 5 Dec 2019
  by Martin CORNU

  - MOSI pin = 11
  - MISO pin = 12
  - SCK pin = 13
  
*/

#include <SPI.h>
#include <MFRC522.h>

#define DEBUG

/***************************** VARIABLES ***************************************/
const uint8_t numReaders = 1;           /* Nombre de lecteurs RFID */
const uint8_t ssPins[] = {2};           /* Pin "SDA" de chaque reader */
const uint8_t resetPin = 8;             /* Pin reset partagee par l'ensemble des readers */
const uint8_t ledGreenPin = 9;          /* Pin de la led verte */
const uint8_t ledRedPin = 10;           /* Pin de la led rouge */
const uint8_t lockPin = A0;             /* Pin de sortie a activer/desactiver si le puzzle est resolu*/
uint8_t       lockPinState = HIGH;      /* HIGH pour desactiver la sortie. LOW pour activer la sortie. */

const String  correctIDs[] = {"3a849315"}; /* Suite d'identifiant de readers à realiser pour resoudre le puzzle */

MFRC522 mfrc522[numReaders];
String currentIDs[numReaders];

/***************************** FUNCTIONS ***************************************/
/*
 * Helper routine to dump a byte array as hex values to Serial.
 */
String dump_byte_array(byte *buffer, byte bufferSize) {
    String buff = "";
    for (byte i = 0; i < bufferSize; i++) {
        buff += String(buffer[i], HEX);
    }
    return buff;
}

/***************************** PROGRAM ***************************************/
void setup() {

  #ifdef DEBUG
  Serial.begin(9600);
  Serial.println(F("------Start SETUP-----"));
  #endif

  pinMode(lockPin, OUTPUT);
  digitalWrite(lockPin, lockPinState);
  pinMode(ledGreenPin, OUTPUT);
  digitalWrite(ledGreenPin, LOW);
  pinMode(ledRedPin, OUTPUT);
  digitalWrite(ledRedPin, LOW);

  SPI.begin();

  for(uint8_t i=0; i<numReaders; i++)
  {
    mfrc522[i].PCD_Init(ssPins[i], resetPin);

    #ifdef DEBUG
    Serial.print(F("Reader #"));
    Serial.print(i);
    Serial.print(F(" initialized on pin "));
    Serial.print(String(ssPins[i]));
    Serial.print(F(". Version : "));
    mfrc522[i].PCD_DumpVersionToSerial();
    #endif
  
    delay(100);
  }

  #ifdef DEBUG
  Serial.println(F("------Start SETUP-----"));
  #endif
}


void loop() { 
  uint8_t puzzleSolved = 1;
  uint8_t changedValue = 0;

  for (uint8_t i=0; i<numReaders; i++)
  {
    mfrc522[i].PCD_Init();

    String readRFID = "";

    /* if sensor detect card and able to read it */
    if (mfrc522[i].PICC_IsNewCardPresent() && mfrc522[i].PICC_ReadCardSerial())
    {
      readRFID = dump_byte_array(mfrc522[i].uid.uidByte, mfrc522[i].uid.size);
    }

    if (readRFID != currentIDs[i])
    {
      changedValue = 1;
      currentIDs[i] = readRFID;
    }

    if (currentIDs[i] != correctIDs[i])
    {
      puzzleSolved = 0;
    }

    mfrc522[i].PICC_HaltA();
    mfrc522[i].PCD_StopCrypto1();
  }

  #ifdef DEBUG
  if (changedValue == 1)
  {
    for (uint8_t i=0; i<numReaders; i++)
    {
      Serial.print(F("Reader #"));
      Serial.print(String(i));
      Serial.print(F(" on Pin #"));
      Serial.print(String((ssPins[i])));
      Serial.print(F(" detected tag: "));
      Serial.println(currentIDs[i]);
    }
    Serial.println(F("---"));
  }
  
  #endif

  if (puzzleSolved == 1)
  {
    #ifdef DEBUG
    Serial.println(F("Puzzle solved!"));
    #endif
    digitalWrite(lockPin, !lockPinState);
    digitalWrite(ledGreenPin, HIGH);
    digitalWrite(ledRedPin, LOW);

    /* Exit program */
    while(1){}
  }
  else
  {
    digitalWrite(ledRedPin, HIGH);
    digitalWrite(ledGreenPin, LOW);
  }
  
}
