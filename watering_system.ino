
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

#define TEMPERATURE_SENSOR_PIN 13 // Arduino pin connected to DS18B20 sensor's DQ pin
#define HUMIDITY_SENSOR_PIN 8
#define RELAY_PIN 12

OneWire oneWire(TEMPERATURE_SENSOR_PIN);  // setup a oneWire instance
DallasTemperature sensors(&oneWire);      // pass oneWire to DallasTemperature library
LiquidCrystal_I2C lcd(0x27, 16, 2);       // I2C address 0x27, 16 column and 2 rows

int humidityResistance;
float tempCelsius;    // temperature in Celsius

void setup() {
  sensors.begin();    // initialize the sensor

  lcd.begin(16, 2); // initialize the lcd

  pinMode(HUMIDITY_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
}

float getCelsiusTemperature() {
  sensors.requestTemperatures();             // send the command to get temperatures
  return sensors.getTempCByIndex(0);  // read temperature in Celsius
}

int getHumidity() {
  return digitalRead(HUMIDITY_SENSOR_PIN);
}

void startWaterPump() {
  digitalWrite(RELAY_PIN, LOW);
}

void stopWaterPump() {
  digitalWrite(RELAY_PIN, HIGH);
}

void displayData() {
  lcd.clear();
  lcd.setCursor(0, 0);       // start to print at the first row -> Temperature
  lcd.print(tempCelsius);    // print the temperature in Celsius
  lcd.print((char)223);      // print ° character
  lcd.print("C");
  
  lcd.setCursor(0, 1);       // start to print at the second row -> Humidity
}

void loop() {
  // gather data from sensors
  tempCelsius = getCelsiusTemperature();
  humidityResistance = getHumidity();
  
  displayData();

   if (tempCelsius >= 18,3 && tempCelsius <= 26.6 ) {
      lcd.print("Temperature is good!");
   } else if (tempCelsius >25){
      lcd.print("Temperature is too high!");
   } else {
      lcd.print("Temperature is too low!");
   }
   
    // Low resistance between the 2 probes means high humidity
    if (humidityResistance == LOW) {
      lcd.print("Humidity high!");
      stopWaterPump();
    } else {  
      lcd.print("Humidity low!");
      startWaterPump();
    }
   
  
  delay(500);
}
