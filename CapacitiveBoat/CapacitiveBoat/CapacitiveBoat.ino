#include <CapacitiveSensor.h>
#include <Keyboard.h>

//change these variables to change keypress characters and durations after pressing each
const char keyPress1 = '1';
const long key1duration = 1000;
const char keyPress2 = '2';
const long key2duration = 1000;
const char keyPress3 = '3';
const long key3duration = 1000;
const char keyPress4 = '4';
const long key4duration = 1000;
const char keyPress5 = '5';
const long key5duration = 1000;
const char keyPress6 = '6';
const long key6duration = 1000;
const char keyPress7 = '7';
const long key7duration = 1000;
const char keyPress8 = '8';
const long key8duration = 1000;

CapacitiveSensor   cs_10_2 = CapacitiveSensor(10,2); 
CapacitiveSensor   cs_10_3 = CapacitiveSensor(10,3);
CapacitiveSensor   cs_10_4 = CapacitiveSensor(10,4);  
CapacitiveSensor   cs_10_5 = CapacitiveSensor(10,5); 
CapacitiveSensor   cs_10_6 = CapacitiveSensor(10,6);
CapacitiveSensor   cs_10_7 = CapacitiveSensor(10,7);  
CapacitiveSensor   cs_10_8 = CapacitiveSensor(10,8); 
CapacitiveSensor   cs_10_9 = CapacitiveSensor(10,9); 

char lastKeyPress = ' ';
char releaseCharacter = 'z';

long lastKeyPressTime = 0;
long currentKeyDuration = 0;

bool interruptable = false;

void setup()                    
{
//   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(9600);
   Keyboard.begin();
}
void triggerKeyPress(long inputValue, long treshold, char keyPressCharacter, long duration){
  if(inputValue>=treshold && lastKeyPress!=keyPressCharacter && (lastKeyPress==' '||interruptable)){
    lastKeyPress=keyPressCharacter;
    Keyboard.press(keyPressCharacter);
    delay(50);
    Keyboard.release(keyPressCharacter);
    lastKeyPressTime = millis();
    currentKeyDuration = duration;
  }
}
void loop()                    
{
    long start = millis();
    long total1 =  cs_10_2.capacitiveSensor(30);
    long total2 =  cs_10_3.capacitiveSensor(30);
    long total3 =  cs_10_4.capacitiveSensor(30);
    long total4 =  cs_10_5.capacitiveSensor(30);
    long total5 =  cs_10_6.capacitiveSensor(30);
    long total6 =  cs_10_7.capacitiveSensor(30);
    long total7 =  cs_10_8.capacitiveSensor(30);
    long total8 =  cs_10_9.capacitiveSensor(30);
    
    long treshold = 15;
 //change durations here, each triggerKeyPress has a variable it uses, a treshold(defined above), the character it sends and the duration for that action
    triggerKeyPress(total1,treshold,keyPress1,key1duration);
    triggerKeyPress(total2,treshold,keyPress2,key2duration);
    triggerKeyPress(total3,treshold,keyPress3,key3duration);
    triggerKeyPress(total4,treshold,keyPress4,key4duration);
    triggerKeyPress(total5,treshold,keyPress5,key5duration);
    triggerKeyPress(total6,treshold,keyPress6,key6duration);
    triggerKeyPress(total7,treshold,keyPress7,key7duration);
    triggerKeyPress(total8,treshold,keyPress8,key8duration);
    
    
    Serial.print(millis() - start);        // check on performance in milliseconds
    Serial.print("\t");                    // tab character for debug windown spacing

    Serial.print(total1);  
    Serial.print("\t");
    Serial.print(total2); 
    Serial.print("\t");
    Serial.print(total3);  
    Serial.print("\t");
    Serial.print(total4); 
    Serial.print("\t");
    Serial.print(total5);  
    Serial.print("\t");
    Serial.print(total6); 
    Serial.print("\t");
    Serial.print(total7);  
    Serial.print("\t");
    Serial.println(total8); 
    Serial.print("\t");
    
    if(lastKeyPressTime && (millis()-lastKeyPressTime>currentKeyDuration) && lastKeyPress!=' '){
      Serial.print("release triggered");
      lastKeyPress = ' ';
      lastKeyPressTime = 0;
        Keyboard.press(releaseCharacter);
        delay(50);
        Keyboard.release(releaseCharacter);
    }
    
    delay(10);                             // arbitrary delay to limit data to serial port 
}
