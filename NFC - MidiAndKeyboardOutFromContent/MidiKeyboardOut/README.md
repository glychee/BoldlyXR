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
