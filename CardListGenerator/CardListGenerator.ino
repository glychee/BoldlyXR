
#include <PN532_HSU.h>
#include <PN532.h>
    
PN532_HSU pn532hsu(Serial1);
PN532 nfc(pn532hsu);

/* Instructions for usage: 
 * 1. Upload to cardscanner, board type is Arduino Leonardo
 * 2. Open the serial monitor
 * 3. Scan each card one by one 
 * 4. Send the character 'x' to the serial monitor
 * 5. Copy the list idList in it's entirity
 */

uint8_t idList[100][8] = {};
int currentStorage = 0;
#include <Keyboard.h>

void setup(void) {
    Keyboard.begin();

  Serial.begin(115200);
  nfc.begin();
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  Serial.println("Waiting for an ISO14443A Card ...");
}

//private function to compare arrays, in this script it's used to compare a new card to an entry(an array)from the 2dimensional array 
boolean array_cmp(uint8_t *a, uint8_t *b, int len_a, int len_b){
     int n;
     //if lengths don't match, return false, otherwise continue
     if (len_a != len_b) return false;
     // test each element to be the same. if not, return false, otherwise the function will continue and return true
     for (n=0;n<len_a;n++) if (a[n]!=b[n]) return false;
     return true;
}

void loop(void) {

  //Serial input 
  if (Serial.available()) {
    // get the new byte:
    switch((uint8_t)Serial.read()){
      case 'x':
          Serial.println("Printing all scanned UID's, Copy the following array into your sketch:\n");
          Serial.print("uint8_t idList[40][8] = "); 
          Serial.print("{\n{");
          for(int x = 0; x<currentStorage; x++){
              for(int y = 0; y<7; y++){
              Serial.print("0x");
              Serial.print(idList[x][y],HEX);
              Serial.print(", ");
              }
              Serial.print("0x00}");
              x==currentStorage-1?Serial.print("\n};"):Serial.print(",\n{");
          }
              Serial.print("\nint currentStorage = ");Serial.print(currentStorage);Serial.print(";");
              switch(currentStorage){
                case 2:
                            Keyboard.print('z');  
                case 4:
                          Keyboard.print('x');  
              }
          break;
    }
  }
  
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    //uncomment these two lines to always see which card is scanned
    //Serial.print("  UID Value: "); 
    //nfc.PrintHex(uid, 7);
    
    //check if we already scanned this uid before
    bool scannedBefore=false;

    for(uint8_t counter = 0;counter <= currentStorage; counter++){
      //check a matching card is known
      if(array_cmp(uid,idList[counter],7,7)){
          scannedBefore=true;
      }
      else{
      }
    }

    // if not scanned before
    if(!scannedBefore){      Serial.print("Adding new carddd: ");
      nfc.PrintHex(uid, 7);
      for(int currentByte=0;currentByte<7;currentByte++){
        idList[currentStorage][currentByte]=uid[currentByte];
        
      }
      currentStorage++;
      Serial.print("Stored: "); Serial.print(currentStorage); Serial.println(" cards now."); 
      delay(500); //delay to have time between new reads
    }
  }
}
