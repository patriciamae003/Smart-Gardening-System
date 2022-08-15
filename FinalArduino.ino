#include <DHT.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

//Temperature & Humidity Sensor
#define DHT_TYPE DHT11
#define DHT_PIN A0
// Soil Moisture Sensor pin
#define SOIL_PIN A1
// Rain Sensor pin
#define RAIN_PIN A2
// pH Sensor pin
#define PH_PIN A3

int phVal = 0;
unsigned long int avgVal;
int buffer_arr[10], phTemp;
float phFinal;
float calibration_value = 21.34 - 0.8;
int chk;
float temp;
float hum;
int soil;
int rain;

DHT dht(DHT_PIN, DHT_TYPE);


SoftwareSerial arduino(9, 10);
//SoftwareSerial arduino2(1, 0);

//12, 11, 5, 4, 3, 2
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  arduino.begin(9600);
  //LCD Contrast (6, 0) or (6, 60)
  //analogWrite(1, 0);
  lcd.begin(16, 2);
  //while(!Serial);
  dht.begin();
}

void loop() {

  lcd.setCursor(0, 0);
  lcd.print("   Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("    Greenie!");
  
  if(arduino.available() > 0) {
    int state = arduino.parseInt();

    // LCD Rain Display
    if (state == 8){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Rain Level: ");
      lcd.print(rain);
      delay(2000);
    }
    // LCD pH Display
    if (state == 7){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("pH Level: ");
      lcd.print(phFinal);
      delay(2000);
    }
    // LCD Soil Display
    if (state == 6){
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Soil Moisture: ");
      lcd.setCursor(6, 1);
      lcd.print(soil);
      delay(2000);
    }
    // LCD Temp/Humidity Display
    if (state == 5){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(temp);
      lcd.print((char)223);
      lcd.print("F");
      lcd.setCursor(0, 1);
      lcd.print("Humidity: ");
      lcd.print(hum);
      lcd.print("%");
      delay(2000);
    }
    
    // LCD Temp/Humidity Data OFF
    if (state == 4){
      delay(10);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Welcome to");
      lcd.setCursor(0, 1);
      lcd.print("Greenie!");
      delay(2000);
    }
    
    /*
    // Nutrient Pump Control
    if (state == 3){
      digitalWrite(A5, HIGH);
      Serial.println("Nutrient Pump ON");
    }
    if (state == 2){
      digitalWrite(A5, LOW);
      Serial.println("Nutrient Pump OFF"); 
    }
    
    // Water Pump Control
    if (state == 1){
      digitalWrite(7, HIGH);
      Serial.println("LED ON");
    }
    if (state == 0){
      digitalWrite(7, LOW);
      Serial.println("LED OFF");
    }
    */
  }
  
  if(Serial.available() == 0) {
    chk = dht.read(DHT_PIN);
    temp = (dht.readTemperature() * 9/5) + 32;
    delay(100);
    hum = dht.readHumidity();
    delay(100);
    soil = analogRead(SOIL_PIN);
    delay(100);
    rain = analogRead(RAIN_PIN);
    delay(100);
    ph_sensor();
    delay(100);
    arduino.print(temp);
    arduino.print("T");
    arduino.print(hum);
    arduino.print("H");
    arduino.print(soil);
    arduino.print("S");
    arduino.print(rain);
    arduino.print("R");
    arduino.print(phFinal);
    arduino.print("P");
    arduino.print("\n");  
    delay(2000);
  }
}

void ph_sensor() {
  for(int i=0;i<10;i++) { 
    buffer_arr[i]=analogRead(PH_PIN);
    delay(30);
  }
  
  for(int i=0;i<9;i++) {
    for(int j=i+1;j<10;j++) {
      if(buffer_arr[i]>buffer_arr[j]) {
        phTemp=buffer_arr[i];
        buffer_arr[i]=buffer_arr[j];
        buffer_arr[j]=phTemp;
      }
    }
  }
  
  avgVal=0;
  for(int i=2;i<8;i++) {
    avgVal+=buffer_arr[i];
  }
  float volt=(float)avgVal*5.0/1024/6; 
  phFinal = -5.70 * volt + calibration_value;
}
