#include <PN532_HSU.h>
#include <PN532.h>
#include <Keyboard.h>

PN532_HSU pn532hsu(Serial1);
PN532 nfc(pn532hsu);

uint8_t idList[180][8] = {};
int currentStorage = 0;

//randomised output variables
int chanceNumber = 100; //chanceNumber 3 gives a 1 in 3 chance to win 
char winChar = 'x';
char loseChar = 'y';

//repeat cards
char alreadyTriedChar = 'z';
bool repeatAllowed = true;
const long alreadyTriedCardDelay = 4000; //only after 4 seconds can unknown cards repeat their action, normal cards are instant
unsigned long lastTriggerTime = 0;

void setup(void) {
  //setup the randomness:
  randomSeed(analogRead(0));

  //setup serial and nfc
  //Serial.begin(115200);
  nfc.begin();
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  //Serial.println("Waiting for an ISO14443A Card ...");
}

bool getRandomWinOrLoss(){
  return (bool) random(0,chanceNumber-1);
}

void sendKeypress(char keyChar){
  Keyboard.press(keyChar);
  delay(50);
  Keyboard.release(keyChar);
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
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success ) {
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
    if(!scannedBefore){      
      //Serial.print("Adding card to known list: ");
      //nfc.PrintHex(uid, 7);
      for(int currentByte=0;currentByte<7;currentByte++){
        idList[currentStorage][currentByte]=uid[currentByte];
        
      }
      //random function
      bool result = getRandomWinOrLoss();
      //Serial.print("Result is: ");Serial.println(result);
      idList[currentStorage][7] = result;
      //result debugging
      //result ? Serial.print("Card lost") : Serial.print("Card won");
      //result keypresses
      result ? sendKeypress(loseChar) : sendKeypress(winChar);
      currentStorage++;
      lastTriggerTime=millis();
      //Serial.print("Stored: "); Serial.print(currentStorage); Serial.println(" cards now."); 
      delay(5); //delay to have time between new reads
    }
    else{
      if(repeatAllowed&&(millis()-lastTriggerTime>alreadyTriedCardDelay)){
        sendKeypress(alreadyTriedChar);
        lastTriggerTime=millis();
      }
    }
  }
}
