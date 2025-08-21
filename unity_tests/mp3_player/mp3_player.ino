/***************************************************
Based on library example code
DFPlayer - A Mini MP3 Player For Arduino
 <https://www.dfrobot.com/product-1121.html>
 
 ***************************************************
 This example shows the basic function of library for DFPlayer.
 
 Created 2016-12-07
 By [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here
 <https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299#Connection_Diagram>
 2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

// Parameters
#define POWER_UP_AUDIO 1
#define SHUT_DOWN_AUDIO 2
#define HUMMING_AUDIO 3
#define SWING_AUDIO_START 4
#define SWING_AUDIO_END 5
#define VOLUME 15

// MP3 player configuration
#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/D7, /*tx =*/D8);
#define FPSerial softSerial

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup(){
  FPSerial.begin(9600);

  // initialize serial monitor
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  // Check configuration of MP3 player
  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  
  // Set volume
  myDFPlayer.volume(VOLUME);  //Set volume value. From 0 to 30
}

void loop()
{
  // Play each audio separately with 3s interval
  Serial.println("POWER_UP_AUDIO");
  myDFPlayer.playMp3Folder(POWER_UP_AUDIO);
  delay(3000);

  Serial.println("HUMMING_AUDIO");
  myDFPlayer.playMp3Folder(HUMMING_AUDIO);
  delay(3000);

  Serial.println("SWING_AUDIO_START");
  myDFPlayer.playMp3Folder(SWING_AUDIO_START);
  delay(3000);

  Serial.println("SWING_AUDIO_END");
  myDFPlayer.playMp3Folder(SWING_AUDIO_END);
  delay(3000);

  Serial.println("SHUT_DOWN_AUDIO");
  myDFPlayer.playMp3Folder(SHUT_DOWN_AUDIO);
  delay(3000);
  
  // Check MP3 module health
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}

