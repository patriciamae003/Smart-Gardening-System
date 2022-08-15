#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Espalexa.h>
#include <SoftwareSerial.h>
#include <FirebaseESP8266.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

//#define WIFI_SSID "Samsung Galaxy S20+" // your wifi SSID
//#define WIFI_PASSWORD "samsungs20password" //your wifi PASSWORD

#define WIFI_SSID "MySpectrumWiFid0-2G" // your wifi SSID
#define WIFI_PASSWORD "shorttruck165" //your wifi PASSWORD

#define FIREBASE_HOST "greenie-dc274-default-rtdb.firebaseio.com" // change here
#define FIREBASE_AUTH "2K5AK56HoEIo7W90HqmoxnuC2h9XR2uwzG4V043q"  // your private key
FirebaseData firebaseData;

SoftwareSerial nodemcu(D1, D2);
//SoftwareSerial nodemcu(3, 1);

// prototypes
boolean connectWifi();

//callback functions
void parse_data();
void thirdLightChanged(uint8_t brightness);
void fourthLightChanged(uint8_t brightness);
void fifthLightChanged(uint8_t brightness);
void sixthLightChanged(uint8_t brightness);


// device names
//String Device_1_Name = "Water Pump";
//String Device_2_Name = "Nutrient Pump";
String Device_3_Name = "Temperature Display";
String Device_4_Name = "Soil Display";
String Device_5_Name = "Level Display";
String Device_6_Name = "Rain Display";


boolean wifiConnected = false;

Espalexa espalexa;

// Variables
char c;
String dataIn;
int indexOfTemp, indexOfHum, indexOfSoil, indexOfRain, indexOfPh;
String tempReading, humReading, soilReading, rainReading, phReading;
//int counter = 1;
//String soil1, soil2, soil3, soil4, soil5;
//int stopValue = 1;

void setup ()
{
  Serial.begin(9600);
  nodemcu.begin(9600);
  
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if (wifiConnected)
  {
    // Define your devices here.
    //espalexa.addDevice(Device_1_Name, firstLightChanged); //simplest definition, default state off
    //espalexa.addDevice(Device_2_Name, secondLightChanged); //simplest definition, default state off
    espalexa.addDevice(Device_3_Name, thirdLightChanged); //simplest definition, default state off
    espalexa.addDevice(Device_4_Name, fourthLightChanged); //simplest definition, default state off
    espalexa.addDevice(Device_5_Name, fifthLightChanged); //simplest definition, default state off
    espalexa.addDevice(Device_6_Name, sixthLightChanged); //simplest definition, default state off

    espalexa.begin();
  }
  else
  {
    while (1)
    {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);     
}

void loop ()
{ 
  //Alexa Loop
  espalexa.loop();
  delay(1000);
  
  // Sensor Readings
  while (nodemcu.available()>0) {
    c = nodemcu.read();
    
    if (c == '\n') 
      break;
        
    else 
      dataIn += c;
  }
     
  if (c == '\n') {
    parse_data(); 
    c = 0;
    dataIn = "";
  }
  
  Firebase.setString(firebaseData, "Sensors/Temperature", tempReading);
  Firebase.setString(firebaseData, "Sensors/Humidity", humReading);
  Firebase.setString(firebaseData, "Sensors/Soil", soilReading);
  Firebase.setString(firebaseData, "Sensors/Rain", rainReading);
  Firebase.setString(firebaseData, "Sensors/pH", phReading);
  
  /*
  Firebase.setString(firebaseData, "AverageCount/Soil1", soil1);
  Firebase.setString(firebaseData, "AverageCount/Soil2", soil2);
  Firebase.setString(firebaseData, "AverageCount/Soil3", soil3);
  Firebase.setString(firebaseData, "AverageCount/Soil4", soil4);
  Firebase.setString(firebaseData, "AverageCount/Soil5", soil5);
  */
}

//our callback functions
/*
void firstLightChanged(uint8_t brightness)
{
  //Control the device
  if (brightness)
  {
    if (brightness == 255)
    {
      // Turn On Water Pump
      nodemcu2.write("1");
    }
  }
  else
  {
    // Turn Off Water Pump
    nodemcu2.write("0");
  }
}

//our callback functions
void secondLightChanged(uint8_t brightness)
{
  //Control the device
  if (brightness)
  {
    if (brightness == 255)
    {
      // Turn On Nutrient Pump
      nodemcu2.write("3");
    }
  }
  else
  {
    // Turn Off Nutrient Pump
    nodemcu2.write("2");
  }
}
*/

//our callback functions
void thirdLightChanged(uint8_t brightness)
{
  //Control the device
  if (brightness)
  {
    if (brightness == 255)
    {
      // Turn On LCD Temperature/Humidity Display
      nodemcu.write("5");
    }
  }
  else
  {
    // Turn Off LCD Temperature/Humidity Display
    nodemcu.write("4");
  }
}

//our callback functions
void fourthLightChanged(uint8_t brightness)
{
  //Control the device
  if (brightness)
  {
    if (brightness == 255)
    {
      // Turn On LCD Soil Display
      nodemcu.write("6");
    }
  }
  else
  {
    // Turn Off LCD Soil Display
    nodemcu.write("4");
  }
}


//our callback functions
void fifthLightChanged(uint8_t brightness)
{
  //Control the device
  if (brightness)
  {
    if (brightness == 255)
    {
      // Turn On LCD pH Display
      nodemcu.write("7");
    }
  }
  else
  {
    // Turn Off LCD pH Display
    nodemcu.write("4");
  }
}

//our callback functions
void sixthLightChanged(uint8_t brightness)
{
  //Control the device
  if (brightness)
  {
    if (brightness == 255)
    {
      // Turn On LCD Rain Display
      nodemcu.write("8");
    }
  }
  else
  {
    // Turn Off LCD Rain Display
    nodemcu.write("4");
  }
}

void parse_data() {
  indexOfTemp = dataIn.indexOf("T");
  indexOfHum = dataIn.indexOf("H");
  indexOfSoil = dataIn.indexOf("S");
  indexOfRain = dataIn.indexOf("R");
  indexOfPh = dataIn.indexOf("P");

  tempReading = dataIn.substring(0, indexOfTemp);
  humReading = dataIn.substring(indexOfTemp+1, indexOfHum);
  soilReading = dataIn.substring(indexOfHum+1, indexOfSoil);
  rainReading = dataIn.substring(indexOfSoil+1, indexOfRain);
  phReading = dataIn.substring(indexOfRain+1, indexOfPh);

  /*
  if (counter == 1) {
    soil1 = soilReading;
    counter++;
  }

  if (counter == 2) {
    soil2 = soilReading;
    counter++;
  }

  if (counter == 3) {
    soil3 = soilReading;
    counter++;
  }

  if (counter == 4) {
    soil4 = soilReading;
    counter++;
  }

  if (counter == 5) {
    soil5 = soilReading;
    counter = 0;
  }
  */
  
}


// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false; 
      break;
    }
    i++;
  }
  Serial.println("");
  if (state) {
    Serial.print("Connected to ");
    Serial.println(WIFI_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}
