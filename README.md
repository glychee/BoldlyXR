Steps to use this project:
1. Upload CardListGenerator to the arduino and open the serial monitor
2. Scan all of the cards you want one by one
3. enter the character 'x' into the serial monitor and press Send
4. The serial monitor will output some text like the following:
-------------
uint8_t idList[40][8] = {
{0x23, 0x1D, 0xAA, 0xC, 0x0, 0x0, 0x0, 0x00},
{0xC3, 0xFC, 0xEC, 0x1A, 0x0, 0x0, 0x0, 0x00},
{0x93, 0xA8, 0xE8, 0x1A, 0x0, 0x0, 0x0, 0x00},
{0x3, 0x98, 0x57, 0x2, 0x0, 0x0, 0x0, 0x00}
};
int currentStorage = 4;
-------------
5. Copy the idlist and the currentStorage variables, these are used to import your cards into the other program
6. Open CardScanner_Knob_Keyboard
7. Replace lines 2 through 10 with the idlist you copied in step 5
8. Change the last hex for each card id to the character you want as an output, example:
  "{0x23, 0x1D, 0xAA, 0xC, 0x0, 0x0, 0x0, 0x00}," 
  becomes
  "{0x23, 0x1D, 0xAA, 0xC, 0x0, 0x0, 0x0, 'x'},"
9. Upload the program to the arduino
