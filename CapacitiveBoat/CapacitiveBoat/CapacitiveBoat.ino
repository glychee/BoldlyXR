#include <CapacitiveSensor.h>
#include <Keyboard.h>

CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapacitiveSensor   cs_4_6 = CapacitiveSensor(4,6);        // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
CapacitiveSensor   cs_4_8 = CapacitiveSensor(4,8);        // 10M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil

char lastKeyPress = ' ';
char releaseCharacter = 'z';

long lastKeyPressTime = 0;
long currentKeyDuration = 0;

bool interruptable = false;

void setup()                    
{
   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
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
    long total1 =  cs_4_2.capacitiveSensor(30);
    long total2 =  cs_4_6.capacitiveSensor(30);
    long total3 =  cs_4_8.capacitiveSensor(30);
    long treshold = 15;
    triggerKeyPress(total1,treshold,'1',4000);
    triggerKeyPress(total2,treshold,'2',4000);
    triggerKeyPress(total3,treshold,'3',1000);
    
    //debug stuff
    
    Serial.print(millis() - start);        // check on performance in milliseconds
    Serial.print("\t");                    // tab character for debug windown spacing

    Serial.print(total1);                  // print sensor output 1
    Serial.print("\t");
    Serial.print(total2);                  // print sensor output 2
    Serial.print("\t");
    Serial.println(total3);                // print sensor output 3

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
