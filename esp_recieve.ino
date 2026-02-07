#include <Wire.h>
#include <Adafruit_DPS310.h>

// DPS310 Barometer
Adafruit_DPS310 dps;

// Weather data structure
struct WeatherData {
  float windSpeed;
  String windDirection;
  float tempC;
  float tempF;
  float humidity;
  float rainfall;
  float pressure;
  float altitude;
  float baroTempC;
};

WeatherData currentWeather;

void setup() {
  Serial.begin(9600);   // USB Serial Monitor
  Serial2.begin(9600, SERIAL_8N1, 16, 17);  // RX2=GPIO16, TX2=GPIO17
  Wire.begin(21, 22);   // SDA=21, SCL=22
  delay(1000);
  
  Serial.println("AutoWesta ESP32 Receiver");
  
  // Initialize DPS310
  if (!dps.begin_I2C()) {
    Serial.println("ERROR: DPS310 sensor not found!");
    while (1) delay(10);
  }
  Serial.println("DPS310 Barometer ready");
  
  // Configure DPS310
  dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
  dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
  
  Serial.println("Waiting for data from Arduino...");
  Serial.println();
}

void loop() {
  // Read barometer data
  sensors_event_t temp_event, pressure_event;
  
  if (dps.temperatureAvailable() && dps.pressureAvailable()) {
    dps.getEvents(&temp_event, &pressure_event);
    currentWeather.baroTempC = temp_event.temperature;
    currentWeather.pressure = pressure_event.pressure;
    
    // Calculate altitude
    currentWeather.altitude = 44330.0 * (1.0 - pow(currentWeather.pressure / 1013.25, 0.1903));
  }
  
  // Receive data from Arduino
  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    data.trim();
    
    if (data.length() > 0) {
      parseWeatherData(data);
      displayWeatherData();
    }
  }
}

void parseWeatherData(String data) {
  int commaPos;
  String values[6];
  
  for (int i = 0; i < 6; i++) {
    commaPos = data.indexOf(',');
    if (commaPos != -1) {
      values[i] = data.substring(0, commaPos);
      data = data.substring(commaPos + 1);
    } else {
      values[i] = data;
    }
  }
  
  currentWeather.windSpeed = values[0].toFloat();
  currentWeather.windDirection = values[1];
  currentWeather.tempC = values[2].toFloat();
  currentWeather.tempF = values[3].toFloat();
  currentWeather.humidity = values[4].toFloat();
  currentWeather.rainfall = values[5].toFloat();
}

void displayWeatherData() {
  Serial.println("COMPLETE WEATHER DATA");
  Serial.println("=====================");
  
  // Wind
  Serial.print("Wind Speed: ");
  Serial.print(currentWeather.windSpeed, 2);
  Serial.println(" m/s");
  Serial.print("Wind Direction: ");
  Serial.println(currentWeather.windDirection);
  
  // Temperature & Humidity
  Serial.print("Temperature: ");
  Serial.print(currentWeather.tempC, 1);
  Serial.print("°C (");
  Serial.print(currentWeather.tempF, 1);
  Serial.println("°F)");
  Serial.print("Humidity: ");
  Serial.print(currentWeather.humidity, 1);
  Serial.println("%");
  
  // Rainfall
  Serial.print("Total Rainfall: ");
  Serial.print(currentWeather.rainfall, 2);
  Serial.println(" mm");
  
  // Barometric Pressure (NEW!)
  Serial.print("Pressure: ");
  Serial.print(currentWeather.pressure, 2);
  Serial.println(" hPa");
  Serial.print("Altitude: ");
  Serial.print(currentWeather.altitude, 1);
  Serial.println(" m");
  Serial.print("Barometer Temp: ");
  Serial.print(currentWeather.baroTempC, 1);
  Serial.println("°C");
  
  Serial.println();
}