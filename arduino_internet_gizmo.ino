// Arduino Internet Gizmo: instructable by talk2bruce
// This program reads RFID card tag id from a Parallax RFID card reader 
// and send that information to a PC connected using a serial connection (USB).
// A program on the PC looks up the tag id in a table to find the name of the
// web site associated with this tag.  The PC program then opens that web site
// in a new tab on the browser.

#include <SoftwareSerial.h>  // library used to read from the RFID reader

#define NO_SUCH_PIN 255      // non-existent pin
#define RFID_OUT_PIN 3       // pin used to read data from the RFID reader
#define RFID_ENABLE_PIN 4    // pin used to tell RFID reader to read tags
#define RGB_INDICATOR_PIN 5  // pin used to turn on the color changing LEDs in the gizmo

#define LED6 13              // pin for the first LED in the circle of LEDs on top of the gizmo
#define LED13 6              // pin for the last LED in the circle of LEDs on top of the gizmo

SoftwareSerial rfid_reader(RFID_OUT_PIN, NO_SUCH_PIN); // Setup the serial port for the RFID reader
int val = 0;                 // used for holding the byte just read from the reader
char code[10];               // used for holding the entire tag id that was just read 
int bytes_read = 0;          // used for counting the number of tag bytes read          
int tags_read = 0;           // used to count the number of tags read
unsigned long time;          // used to remember the time that the tag was read
                             // the time is milliseconds since the Arduino was last reset
//
// Intialize the Arduino Internet Gizmo
//
void setup() { 
  pinMode(RGB_INDICATOR_PIN, OUTPUT);     // set the RGB color changing LED indicator pin to output.
  digitalWrite(RGB_INDICATOR_PIN, HIGH);  // turn on the RGB color changing LED circuit  
  Serial.begin(9600);                     // initialize the serial port used to write to the PC
  rfid_reader.begin(2400);                // initialize the serial port for reading RFID tags              
  pinMode(RFID_ENABLE_PIN, OUTPUT);       // set the pin to enable the RFID reader to output    
  digitalWrite(RFID_ENABLE_PIN, LOW);     // tell the RFID reader to start reading tags  
  initialize_leds();                      // initialize the circle of LEDs on top of the gizmo  
}  

//
// Loop forever reading tags and sending data to the PC
//
void loop() { 
  if(rfid_reader.available() > 0) {         // check to see if data is available from the reader    
    if((val = rfid_reader.read()) == 10) {  // check for a "start of data" byte
      bytes_read = 0; 
      while(bytes_read < 10) {              // if less than 10 bytes have been read, keep reading 
        if( rfid_reader.available() > 0) {  // if a new byte has been read
          val = rfid_reader.read();         // read the byte
          if((val == 10)||(val == 13)) {    // if header or stop bytes before the 10 digit reading 
            break;                          // stop reading 
          } 
          code[bytes_read] = val;           // add the data read to the tag variable          
          bytes_read++;                     // increment the number of bytes read and keep reading  
        } 
      } 
      if(bytes_read == 10) {                // if all of the bytes in the tag have been read 
        tags_read++;                        // increment the number of tags read
        time = millis();                    // record the time the tag was read
        Serial.print(tags_read);            // send the number of tags read to the PC
        Serial.print(",");                  // send a comma
        Serial.print(time);                 // send the time the tag was read
        Serial.print(",");                  // send a comma
        Serial.print(code);                 // send the RFID card tag       
      } 
      bytes_read = 0;                       // reset the number of bytes read
      digitalWrite(RFID_ENABLE_PIN, HIGH);  // deactivate the RFID reader for a moment 
      run_leds(5, 75);                      // make the LEDs on top of the gizmo race around five times
      delay(1500);                          // wait for 1,500 milliseconds
      digitalWrite(RFID_ENABLE_PIN, LOW);   // re-enable the RFID reader to read tags
    } 
  } 
} 

//
// subroutine to make the LEDs on top of the gizmo race around in circles
// input is the number of times to race around and the time delay in milliseconds
// to keep each LED illuninated
//
void run_leds(byte rotations, byte time_delay_ms) {
  byte led;                                 // pin number for the LED currently lit
  byte r;                                   // current rotation number
  for (r = 0; r < rotations; r++) {         // loop for the number of rotations
    for (led = LED13; led <= LED6; led++) { // loop for the number of LEDs
      digitalWrite(led, HIGH);              // turn the LED on
      delay(time_delay_ms);                 // wait for the specified time
      digitalWrite(led,LOW);                // turn the LED off
    }                                       // continue to the next LED
  }                                         // continue to the next rotation
}

//
// subroutine to initialize the LEDs on the top of gizmo
//
void initialize_leds() {
  byte led;                                 // pin number of the LED
  for (led = LED13; led <= LED6; led++) {   // loop through all the LEDs    
    pinMode(led, OUTPUT);                   // set the pin for output
  }                                         // continue to the next LED
}



