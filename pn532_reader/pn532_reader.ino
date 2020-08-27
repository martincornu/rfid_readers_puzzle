/**************************************************************************/
/*! 
    @file     pn532_reader
    @author   Martin CORNU

This program read in loop RFID reader.
If a tag is present, it checks its ID and if the ID is correct it trigger an output.
*/
/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define DEBUG

#define OUTPUT_PIN      (uint8_t)6
#define CARD_ID         (uint32_t)380037037
#define OUTPUT_STATE_ON HIGH

// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

// Use this line for a breakout with a SPI connection:
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

void setup(void) {
  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("S-CAPE Game Saint-Etienne");
  #endif

  pinMode(OUTPUT_PIN,OUTPUT);
  digitalWrite(OUTPUT_PIN, !OUTPUT_STATE_ON);

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    #ifdef DEBUG
    Serial.print("Didn't find PN53x board");
    #endif
    while (1); // halt
  }
  // Got ok data, print it out!
  #ifdef DEBUG
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  #endif
  
  // configure board to read RFID tags
  nfc.SAMConfig();

  #ifdef DEBUG
  Serial.println("Waiting for an ISO14443A Card ...");
  #endif
}


void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 1000);
  
  if (success) {
    if (uidLength == 4)
    {
      // We probably have a Mifare Classic card ... 
      uint32_t cardid = uid[0];
      cardid <<= 8;
      cardid |= uid[1];
      cardid <<= 8;
      cardid |= uid[2];  
      cardid <<= 8;
      cardid |= uid[3]; 
      
      #ifdef DEBUG
      Serial.print("ID #");
      Serial.println(cardid);

      if(cardid == CARD_ID)
      {
        digitalWrite(OUTPUT_PIN, OUTPUT_STATE_ON);
        #ifdef DEBUG
        Serial.println("RFID OK! ACTIVATION OF OUTPUT");
        #endif
      }
      else
      {
        digitalWrite(OUTPUT_PIN, !OUTPUT_STATE_ON);
      }
      #endif
    }
    #ifdef DEBUG
    Serial.println("");
    #endif
  }
  else
  {
    digitalWrite(OUTPUT_PIN, !OUTPUT_STATE_ON);
  }
}
