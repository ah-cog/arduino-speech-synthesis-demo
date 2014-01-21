/*

Collider Voice
(based on Emic 2 Text-to-Speech Module: Basic Demonstration)
                                                 
Authors:
  Michael Gubbels [www.michaelgubbels.com]
  mrgubbels [at] gmail [dot] com

Program Description:

This program provides a basic voice server for Collider.

*/

// include the SoftwareSerial library so we can use it to talk to the Emic 2 module
#include <SoftwareSerial.h>

#define rxPin 2    // Serial input (connects to Emic 2 SOUT)
#define txPin 3    // Serial output (connects to Emic 2 SIN)
#define ledPin 13  // Most Arduino boards have an on-board LED on this pin

// set up a new serial port
SoftwareSerial emicSerial =  SoftwareSerial(rxPin, txPin);

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

boolean active = false;

void setup() { // Set up code called once on start-up

  Serial.begin(9600);
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  // define pin modes
  pinMode(ledPin, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  
  // set the data rate for the SoftwareSerial port
  emicSerial.begin(9600);

  digitalWrite(ledPin, LOW);  // turn LED off
  
  /*
    When the Emic 2 powers on, it takes about 3 seconds for it to successfully
    intialize. It then sends a ":" character to indicate it's ready to accept
    commands. If the Emic 2 is already initialized, a CR will also cause it
    to send a ":"
  */
  emicSerial.print('\n');             // Send a CR in case the system is already up
  while (emicSerial.read() != ':');   // When the Emic 2 has initialized and is ready, it will send a single ':' character, so wait here until we receive it
  delay(10);                          // Short delay
  emicSerial.flush();                 // Flush the receive buffer
}

void loop() { // Main code, to run repeatedly

  // Check if Collider is online
  if (!active) {
    speakOnline(); // Speak some text
    
    // Set active
    active = true;
  }

  delay(500);    // 1/2 second delay

//  singSong(); // Sing a song

  // Print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    
    // Speak the text
    speakText(inputString);
    
    // Clear the string
    inputString = "";
    stringComplete = false;
  }

}

/**
 * SerialEvent occurs whenever a new data comes in the
 * hardware serial RX.  This routine is run between each
 * time loop() runs, so using delay inside loop can delay
 * response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char) Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}

void speakOnline() {
  emicSerial.print('S');
  emicSerial.print("Collider ready.");  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
}

void speakText(String phrase) {
  emicSerial.print('S');
  emicSerial.print(phrase);  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
}

void singSong() {
  emicSerial.print("D1\n");
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
}



