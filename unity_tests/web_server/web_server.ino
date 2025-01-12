#include <ESP8266WiFi.h>       
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "interface.h"

// WEB SERVER PARAMETERS
const char *ssid = "My Lightsaber"; // Wi-Fi network name
const char *password = "anelegantweapon"; // Wi-Fi network password
IPAddress apIP(192, 168, 0, 1); // Server IP address
ESP8266WebServer server(80); // Server port
// ---------------------

// GENERAL MOCKED VALUES
int battery = 73;
// ---------------------

// GENERAL VARAIBLES
int lightsaberId;
String bladeColor;
int activationSpeed;
// ---------------------

void setup() {
  Serial.begin(115200);

  // Web server definitions
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);             
  Serial.printf("\nAccess Point %s started\n", ssid);
  Serial.print("IP address:\t\t");
  Serial.println(WiFi.softAPIP());

  // Define routes
  server.on("/", handleRoot);      
  server.on("/save_configurations", handleForm);
  
  server.begin();                 
  Serial.println("HTTP server started");

  // Retrieve information from EEPROM
  EEPROM.begin(98);

  int initialPosition = 0;
  lightsaberId = eepromReadInt(initialPosition);
  initialPosition+=2;
  bladeColor = eepromReadString(initialPosition, 7);
  initialPosition+=7;
  activationSpeed = eepromReadInt(initialPosition);

  EEPROM.end();
 
  Serial.printf("Lightsaber ID:\t\t%d\n", lightsaberId);
  Serial.printf("Blade color:\t\t%s\n", bladeColor);
  Serial.printf("Activation speed:\t%d\n", activationSpeed);
}
 
void loop() {
  server.handleClient();
}
 
void handleRoot() {
 // Create and return web page
 String index = createInterface(bladeColor, activationSpeed);
 server.send(200, "text/html", index);
}
 
void handleForm() {
 // Retrieve parameters from HTML form
 String bladeColor = server.arg("blade_color");
 int activationSpeed = server.arg("speed").toInt();
 
 Serial.printf("Blade color: %s\n", bladeColor);
 Serial.printf("Activation speed: %d\n", activationSpeed);

 // Save parameters to EEPROM
 int initialPosition = 2;
 initialPosition = eepromSaveString(initialPosition, bladeColor);
 initialPosition = eepromSaveInt(initialPosition, activationSpeed);

 // Create and return web page
 String index = createInterface(bladeColor, activationSpeed);
 server.send(200, "text/html", index);
}
 
int eepromSaveString(int initialPosition, String value) {
  Serial.printf("Saving %s\n", value);

  int finalPosition = initialPosition+value.length();
  Serial.println(finalPosition);
  
  for(int i = 0; i < value.length(); i++) {
    Serial.printf("Saving at position %d : %c\n", initialPosition+i, value.charAt(i));
    EEPROM.write(initialPosition+i, value.charAt(i));
  }
  
  EEPROM.commit();

  return finalPosition;
}
 
int eepromSaveInt(int initialPosition, int value) {
  Serial.printf("Saving %d\n", value);

  EEPROM.write(initialPosition, highByte(value)); // Write the first half
  Serial.printf("Saving at position %d : %d\n", initialPosition, highByte(value));
  EEPROM.write(initialPosition+1, lowByte(value)); // Write the second half
  Serial.printf("Saving at position %d : %d\n", initialPosition+1, lowByte(value));
  
  EEPROM.commit();

  return initialPosition+2;
}

String eepromReadString(int initialPosition, int valueSize) {
  String value = "";
  
  for(int i = initialPosition; i < initialPosition+valueSize; i++) {
    value.concat(char(EEPROM.read(i)));
  }
  
  return value;
}

int eepromReadInt(int initialPosition) {
  int value = (EEPROM.read(initialPosition)<<8)+EEPROM.read(initialPosition+1);
  
  return value;
}

String createInterface(String bladeColor, int activationSpeed) {
  String index = (const __FlashStringHelper*) MAIN_page;
  String batteryBars;
  if (battery < 25){
    batteryBars = "[    ]";
  }else if (battery < 50){
    batteryBars = "[▉   ]";
  }else if (battery < 75){
    batteryBars = "[▉▉  ]";
  }else if (battery < 95){
    batteryBars = "[▉▉▉ ]";
  }else{
    batteryBars = "[▉▉▉▉]";
  }
  index.replace("[id_placeholder]", (String)lightsaberId);
  index.replace("[battery_placeholder]", (String)battery);
  index.replace("[battery_bars_placeholder]", (String)batteryBars);
  index.replace("[color_placeholder]", bladeColor);
  index.replace("[speed_placeholder]", (String)activationSpeed);
  return index;
}