// Only replace these values! ************************************************

uint8_t idList[40][8] = {
{0x23, 0x1D, 0xAA, 0xC, 0x0, 0x0, 0x0, 'x'},
{0xC3, 0xFC, 0xEC, 0x1A, 0x0, 0x0, 0x0, 'y'},
{0x93, 0xA8, 0xE8, 0x1A, 0x0, 0x0, 0x0, 'z'},
{0x3, 0x98, 0x57, 0x2, 0x0, 0x0, 0x0, 'w'}
};
int currentStorage = 4;

//************************************************

/* After replacing the values above, the last 0x00 decided which keypress will be sent, update some entries as follows:
Example:
The first entry:
{0x3, 0x98, 0x57, 0x2, 0x0, 0x0, 0x0, 0x00},
should output "z", so it is changed into 
{0x3, 0x98, 0x57, 0x2, 0x0, 0x0, 0x0, 'z'},
*/

#include <PN532_HSU.h>
#include <PN532.h>
    
PN532_HSU pn532hsu(Serial1);
PN532 nfc(pn532hsu);

#include <Keyboard.h>

void sendKeypress(char keyChar){  //Keyboard.releaseAll();
  Keyboard.press(keyChar);
  delay(100);
  Keyboard.release(keyChar);
}

//variables for knob state tracking and key output
volatile int state = 0;
int previousstate = 0;
volatile bool moving = false;
volatile unsigned long resetTime = 0;
const int timerDelay = 10;
const char keyPresses[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
char currentCardChar;
char previousKeyChar;
char currentEncoderChar;
char previousEncoderChar;

//variables for tracking screensaver
bool screenNeedsSaving = true;
volatile unsigned long lastActivityTime = 0;
const long screenSaveTimeDuration = 5000;
const char screenSaveChar = 's';

void extendScreenSaverTimer(){
    //reset screensaver timer
    lastActivityTime = millis();
    screenNeedsSaving = true;
}

void pin2changefunc() {
  volatile int state2 = digitalRead(2);
  volatile int state3 = digitalRead(3);
  //if pin 2 changed into LOW and pin 3 is currently HIGH, we moving increasing in state
  //if pin 2 changed into HIGH and pin 3 is currently LOW, we moving right, but pin 2 already arrived, do nothing and let pin 3 handle
  //if pin 2 changed into HIGH and pin 3 is currently HIGH, we were moving left, 
    if((state2!=state3)&&!moving&&(millis()-resetTime>timerDelay)){
    if(state==0){
      state=15;
    }
    else{
      state--;
    }
    currentEncoderChar = keyPresses[state];
    extendScreenSaverTimer();
    moving = true;
  }
  if((state2==state3)&&moving){
    moving = false;
    resetTime = millis();
  }
}

void pin3changefunc() {
  volatile int state2 = digitalRead(2);
  volatile int state3 = digitalRead(3);
  if((state2!=state3)&&!moving&&(millis()-resetTime>timerDelay)){
    if(state==15){
      state=0;
    }
    else{
      state++;
    }
    currentEncoderChar = keyPresses[state];
    extendScreenSaverTimer();
    moving = true;
  }
  if((state2==state3)&&moving){
    moving = false;
    resetTime = millis();
  }
}



void setup(void) {
  Serial.begin(115200);
  
  // knob setup code
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), pin2changefunc, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), pin3changefunc, CHANGE);
  
  Keyboard.begin();
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
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  //success will be empty if no valid UID is read, it is used as both the success variable and the storage of the UID 
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {
    //check if we already scanned this uid before
    bool scannedBefore=false;
    for(uint8_t counter = 0;counter <= currentStorage; counter++){
      //check a matching card is known

      Serial.println(array_cmp(uid,idList[counter],7,7));
      if(array_cmp(uid,idList[counter],7,7)){
          scannedBefore=true;
          currentCardChar = (char)idList[counter][7];
          extendScreenSaverTimer();  
          //delay(10);
      }
    }
  }
  if (currentCardChar != previousKeyChar){
    sendKeypress(currentCardChar);
    previousKeyChar = currentCardChar;
  }
  if (currentEncoderChar != previousEncoderChar){
    sendKeypress(currentEncoderChar);
    previousEncoderChar = currentEncoderChar;
  }
  //screensaver code
  if(screenNeedsSaving&&(millis()-lastActivityTime>screenSaveTimeDuration)){
    sendKeypress(screenSaveChar); 
    screenNeedsSaving = false;
  }
}
