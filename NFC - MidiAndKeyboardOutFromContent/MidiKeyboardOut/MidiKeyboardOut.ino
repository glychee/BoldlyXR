#if 0
#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"

PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);
#elif 1
#include <PN532_HSU.h>
#include <PN532.h>

PN532_HSU pn532hsu(Serial1);
PN532 nfc(pn532hsu);
#else
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);
#endif

#include <Keyboard.h>
#include "MIDIUSB.h"

void setup(void) {
  Serial.begin(115200);
  Serial.println("Hello!");
  Keyboard.begin();
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A Card ...");
}

void sendMidiNote(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
  delay(20);
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
  delay(20);
}

void sendKeypress(char keyChar){  //Keyboard.releaseAll();
  Keyboard.press(keyChar);
  delay(100);
  Keyboard.release(keyChar);
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t data[32];                         // data array to store read data in
  uint8_t midiNotes[] = { 33, 35, 24, 26, 28, 29, 31, 33, 35};
  bool dataReadSuccessfully = false;
  char readChars[3];


  //Data reading
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {
    if (uidLength == 4)
    {
      uint8_t keya[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7};
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
      if (success)
      {
        success = nfc.mifareclassic_ReadDataBlock(4, data);
        dataReadSuccessfully = true;
        readChars[0] = data[9];
        readChars[1] = data[10];
        readChars[2] = data[11];
      }
    }
    if (uidLength == 7)
    {
      success = nfc.mifareultralight_ReadPage (6, data);
      dataReadSuccessfully = true;
      readChars[0] = data[1];
      readChars[1] = data[2];
      readChars[2] = data[3];
    }
  }

  //Keyboard and midi output blocks
  if (dataReadSuccessfully)
  {
    Serial.print(readChars[0]);
    Serial.print(readChars[1]);
    Serial.print(readChars[2]);
    Serial.println("");
    if (readChars[0] == '#') {
      Serial.println("Midi");
      if (readChars[1] >= 'A' and readChars[1] <= 'G') {
        Serial.println("Midi without channel"); //defaults to channel 1
        char midiValue = (readChars[1] - 'A'); //translation magic :)
        sendMidiNote(0, midiNotes[midiValue], 64);
      }
      else if (readChars[1] >= '0' and readChars[1] <= '9') {
        Serial.println("Midi with channel");
        char midiChannel = (readChars[1] - '0' - 1); //translation magic :)
        char midiValue = (readChars[2] - 'A'); //translation magic :)
        sendMidiNote(midiChannel, midiNotes[midiValue], 64);
      }
    }
    else if ((readChars[0] >= 'A' and readChars[0] <= 'z') or (readChars[0] >= '0' and readChars[0] <= '9') ) {
      Serial.println("Keyboard output");
      sendKeypress(readChars[0]);
    }
    // Wait a bit before reading the card again
    delay(1000);
  }
}
