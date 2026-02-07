#include <Wire.h>
#include <SoftwareSerial.h>
#include "DFRobot_RainfallSensor.h"
#include "DHT.h"

// Rainfall Sensor (I2C)
DFRobot_RainfallSensor_I2C rain(&Wire);

// DHT22 Temperature & Humidity Sensor
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Wind Speed Sensor (Anemometer)
const int SPEED_SENSOR_PIN = 2;
volatile unsigned int pulseCount = 0;
unsigned long lastTime = 0;
float windSpeed = 0;

// Wind Direction Sensor (Wind Vane)
const int DIRECTION_SENSOR_PIN = A3;

// Serial Communication with ESP32
SoftwareSerial espSerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  Wire.begin();
  
  Serial.println("Initializing Weather Station...");
  
  if (!rain.begin()) {
    Serial.println("ERROR: Rainfall sensor init failed");
    while (1);
  }
  Serial.println("Rainfall sensor ready");
  Serial.print("Firmware version: ");
  Serial.println(rain.getFirmwareVersion());
  
  dht.begin();
  Serial.println("DHT22 sensor ready");
  
  pinMode(SPEED_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SPEED_SENSOR_PIN), countPulse, FALLING);
  Serial.println("Anemometer ready");
  
  pinMode(DIRECTION_SENSOR_PIN, INPUT);
  Serial.println("Wind vane ready");
  Serial.println("ESP32 serial connection ready");
  Serial.println("Weather Station Initialized");
  Serial.println();
}

void loop() {
  if (millis() - lastTime >= 2000) {
    detachInterrupt(digitalPinToInterrupt(SPEED_SENSOR_PIN));
    
    windSpeed = (pulseCount * 0.24) / 2.0;
    
    int rawValue = analogRead(DIRECTION_SENSOR_PIN);
    float voltage = (rawValue / 1023.0) * 5.0;
    String direction = getWindDirection(voltage);
    
    float totalRainMM = rain.getRainfall();
    
    float humidity = dht.readHumidity();
    float tempC = dht.readTemperature();
    float tempF = dht.readTemperature(true);
    
    // Send data to ESP32
    espSerial.print(windSpeed, 2);
    espSerial.print(",");
    espSerial.print(direction);
    espSerial.print(",");
    
    if (!isnan(tempC) && !isnan(humidity)) {
      espSerial.print(tempC, 1);
      espSerial.print(",");
      espSerial.print(tempF, 1);
      espSerial.print(",");
      espSerial.print(humidity, 1);
    } else {
      espSerial.print("0,0,0");
    }
    
    espSerial.print(",");
    espSerial.println(totalRainMM, 2);
    
    // Display on Serial Monitor
    Serial.println("WEATHER STATION DATA");
    Serial.print("Wind Speed: ");
    Serial.print(windSpeed, 2);
    Serial.println(" m/s");
    Serial.print("Wind Direction: ");
    Serial.println(direction);
    
    if (!isnan(humidity) && !isnan(tempC)) {
      Serial.print("Temperature: ");
      Serial.print(tempC, 1);
      Serial.print("°C (");
      Serial.print(tempF, 1);
      Serial.println("°F)");
      Serial.print("Humidity: ");
      Serial.print(humidity, 1);
      Serial.println("%");
    } else {
      Serial.println("Temp/Humidity: ERROR");
    }
    
    Serial.print("Total Rainfall: ");
    Serial.print(totalRainMM, 2);
    Serial.println(" mm");
    Serial.print("Raw Dir: ");
    Serial.print(rawValue);
    Serial.print(" | Voltage: ");
    Serial.print(voltage, 2);
    Serial.println("V");
    Serial.println("Data sent to ESP32");
    Serial.println();
    
    pulseCount = 0;
    lastTime = millis();
    attachInterrupt(digitalPinToInterrupt(SPEED_SENSOR_PIN), countPulse, FALLING);
  }
}

void countPulse() {
  pulseCount++;
}

String getWindDirection(float voltage) {
  if (voltage >= 0.0 && voltage < 0.6) {
    return "N";
  } 
  else if (voltage >= 0.6 && voltage < 1.2) {
    return "NE";
  } 
  else if (voltage >= 1.2 && voltage < 1.8) {
    return "E";
  } 
  else if (voltage >= 1.8 && voltage < 2.4) {
    return "SE";
  } 
  else if (voltage >= 2.4 && voltage < 3.0) {
    return "S";
  } 
  else if (voltage >= 3.0 && voltage < 3.6) {
    return "SW";
  } 
  else if (voltage >= 3.6 && voltage < 4.2) {
    return "W";
  } 
  else if (voltage >= 4.2 && voltage <= 5.0) {
    return "NW";
  } 
  else {
    return "Unknown";
  }
}