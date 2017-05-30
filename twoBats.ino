

/*
 * 2_Lepakkoa version 8
 * 
 * ORIGINAL Code developed by Will Myers and Justin Miller
 * 
 * This version refactored and modified for two SRF01 sensors 
 * and two corresponding vibration motors by Tatu Polvinen.
 * 
 * This code is published under GNU General Public License v3. See more at http://www.gnu.org/licenses/gpl.html
 * 
 */

#include <SoftwareSerial.h>                                       // to use SoftwareSerial library #include it

const int txrxPin = 2;                                            // Defines Pin 2 to be used as both rx and tx for the SRF01
const byte srf1Address = 0x01;                                    // Address of the SFR01
const byte srf2Address = 0x02;                                    // Address of the SFR01
const byte getSoft = 0x5D;                                        // Byte to tell SRF01 we wish to read software version
const byte getRange = 0x54;                                       // Byte used to get range from SRF01 in centimeters "Real Ranging Mode - Result in centimeters, automatically Tx range back to controller as soon as ranging is complete."
const byte getStatus = 0x5F;                                      // Byte used to get the status of the transducer

SoftwareSerial UltrasonicBus(txrxPin, txrxPin);                   // Sets up the SoftwareSerial with Digital Pin 2 (as defined in line "const int txrxPin = 2;") and sets the name to "UltrasonicBus"


void setup(){
  
//  Serial.begin(19200);                                    // Begin serial connection w/ specified baud rate so code can be debugged and monitored with Serial Monitor

  pinMode(0, OUTPUT);                                       // Set pin modes as OUTPUT to drive motors or LEDs
  pinMode(1, OUTPUT);
  
  UltrasonicBus.begin(9600);                                // Begin softwareSerial connection w/ specified baud rate to communicate w/ sensors
  delay(200);                                               // Waits some time to make sure that SRF01 is powered up
//  Serial.println("SRF01 Test");                           
//  Serial.println("Initialization Complete");              // After inititalization is complete you should see the correct version of your device printed to the serial monitor

}


void loop(){

  int range1 = doRange1();                                  // Measure distance w/ sensor in doRange1 function

//  int alarmLevel = sq(range1);

//  Serial.print("Range1 = ");                                
//  Serial.print(range1);                                   // Print range result to the screen
//  Serial.println("  ");                                   // Print some spaces to the screen to make sure space direcly after the result is clear

  int volume1 = map(range1, 0, 701, 255, 0);                // Calculate intensity to drive motors or LEDs w/
  
//  volume1 = sq(volume1);
  
//  Serial.println(alarmLevel);
  
  analogWrite(0, volume1);                                  // Use analogWrite to drive motor or LED connected to specified pin w/ specified intensity
  
//  Serial.print("Volume1 = ");                             // Print to serial connection for monitoring
//  Serial.print(volume1);
//  Serial.println("  ");
  
  delay(70);                                                // delay between measurements to make sure previous ping will not echo in to next
  
  int range2 = doRange2();                                  // Measure distance w/ sensor in doRange1 function

//  Serial.print("Range2 = ");                                
//  Serial.print(range2);                                   // Print range result to the screen
//  Serial.println("  ");                                   // Print some spaces to the screen to make sure space direcly after the result is clear
  
  int volume2 = map(range2, 0, 701, 255, 0);                // Calculate intensity to drive motors or LEDs w/
  
//  volume2 = sq(volume2);
  
  analogWrite(1, volume2);                                  // Use analogWrite to drive motor or LED connected to specified pin w/ specified intensity
  
//  Serial.print("Volume2 = ");                             // Print to serial connection for monitoring
//  Serial.print(volume2);
//  Serial.println("  ");

  delay(70);                                                // delay between measurements to make sure previous ping will not echo in to next
  
 }


void SRF01_Cmd(byte Address, byte cmd){                      // Function to send commands to the SRF01
  pinMode(txrxPin, OUTPUT);                                  // Set pin to output and send break by sending pin low, waiting 2ms and sending it high again for 1ms
  digitalWrite(txrxPin, LOW);                              
  delay(2);                                               
  digitalWrite(txrxPin, HIGH);                            
  delay(1);                                                
  UltrasonicBus.write(Address);                              // Send the address of the SRF01
  UltrasonicBus.write(cmd);                                  // Send commnd byte to SRF01
  pinMode(txrxPin, INPUT);                                   // Make input ready for Rx
  int availableJunk = UltrasonicBus.available();             // Filter out the junk data
  for(int x = 0; x < availableJunk; x++){
    byte junk = UltrasonicBus.read();
  }
}


int doRange1() {
  SRF01_Cmd(srf1Address, getRange);                          // Calls a function to get range from SRF01
  while (UltrasonicBus.available() < 2);                     // Waits to get good data
  byte highByte = UltrasonicBus.read();                      // Get high byte
  byte lowByte = UltrasonicBus.read();                       // Get low byte
  int dist = ((highByte<<8)+lowByte);                        // Put them together
  return dist;
}

int doRange2() {
  SRF01_Cmd(srf2Address, getRange);                          // Calls a function to get range from SRF01
  while (UltrasonicBus.available() < 2);                     // Waits to get good data
  byte highByte = UltrasonicBus.read();                      // Get high byte
  byte lowByte = UltrasonicBus.read();                       // Get low byte
  int dist = ((highByte<<8)+lowByte);                        // Put them together
  return dist;
}

