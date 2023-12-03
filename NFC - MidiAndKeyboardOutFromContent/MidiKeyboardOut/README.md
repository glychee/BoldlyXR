Steps to use this project:
1. Upload MidiKeyboardOut.ino to an arduino with NFC reader attached
2. Write a text block to the NFC card, this block should be the first one written. See steps below:


Using NFC Tools:
1. Download and install https://play.google.com/store/apps/details?id=com.wakdev.wdnfc&hl=en&gl=US&pli=1
2. Go to the Write tab
3. Select "Add a record"
4. Select "Text"
5. To add a Keyboard output, add a character, examples: 'a' or 'Z' or 'c' or 'D'. 
6. To add a Midi output without channel, add a # followed by a character between A and G, examples: '#A' or '#D'.  (This sends to Channel 1)
6. To add a Midi output WITH channel, add a # followed by a number and then by a character between A and G, examples: '#3A' or '#1D'.  

Debugging MIDI:
Madmapper works for checking Midi functionality but requires pressing "Learn" every time you want to check. 
Instead there's an application included that shows live data for quicker debugging.

1. Install MidiView-Installer-1.2.0-Win.exe
2. Run the application 
3. In the red/green Combobox at the top, select "Arduino Micro"
4. Scan a card with midi output 
5. Check in the scan history if the midi output matches what you expect