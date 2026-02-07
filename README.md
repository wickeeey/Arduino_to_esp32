# AutoWesta - Automated Weather Station

A student project to build a DIY weather station using Arduino and ESP32 that measures real-time weather data.

## What Does It Do?

AutoWesta measures:
- **Wind Speed** (m/s) - Using an anemometer ✅
- **Wind Direction** (N, NE, E, SE, S, SW, W, NW) - Using a wind vane ✅
- **Temperature** (°C and °F) - DHT22 sensor ✅
- **Humidity** (%) - DHT22 sensor ✅
- **Rainfall** (mm) - DFRobot rainfall sensor ✅
- **Barometric Pressure** (hPa) - DPS310 sensor ⚠️ (hardware connected, troubleshooting in progress)

## Parts We Used

### Arduino Side (Outdoor Sensors)
- Arduino Uno/Nano
- DHT22 Temperature & Humidity Sensor
- Anemometer (measures wind speed)
- Wind Vane (measures wind direction)
- DFRobot Rainfall Sensor (I2C)

### ESP32 Side (Indoor Module)
- ESP32 DevKit V1
- Grove DPS310 Barometer Sensor

## Wiring Diagram

### DPS310 Barometer → ESP32
```
DPS310 Pin     →  ESP32 Pin
--------------------------------
VCC            →  3.3V
GND            →  GND
SDA            →  Pin 21 (GPIO 21)
SCL            →  Pin 22 (GPIO 22)
```

### Arduino → ESP32 (Serial Communication)
```
Arduino Pin    →  ESP32 Pin
--------------------------------
Pin 11 (TX)    →  Pin 16 (GPIO 16 / RX2)
Pin 10 (RX)    →  Pin 17 (GPIO 17 / TX2)
GND            →  GND
```

### Arduino Sensors
```
DHT22:
- VCC    → Arduino 5V
- GND    → Arduino GND
- DATA   → Arduino Pin 4

Anemometer (Wind Speed):
- Signal → Arduino Pin 2
- VCC    → Arduino 5V
- GND    → Arduino GND

Wind Vane (Direction):
- Signal → Arduino Pin A3
- VCC    → Arduino 5V
- GND    → Arduino GND

Rainfall Sensor (I2C):
- VCC    → Arduino 5V
- GND    → Arduino GND
- SDA    → Arduino A4
- SCL    → Arduino A5
```

## Software You'll Need

### Libraries to Install
**For Arduino:**
- Wire (already included)
- SoftwareSerial (already included)
- DFRobot_RainfallSensor (download from Library Manager)
- DHT sensor library by Adafruit (download from Library Manager)

**For ESP32:**
- Wire (already included)
- Adafruit DPS310 (download from Library Manager)
- Adafruit BusIO (installs automatically with DPS310)

### How to Install
1. Download Arduino IDE (it's free!)
2. Add ESP32 board support (Google "ESP32 Arduino IDE setup")
3. Go to Sketch → Include Library → Manage Libraries
4. Search and install the libraries listed above

## How It Works (Simple Version)

1. **Arduino** is connected to outdoor sensors and collects:
   - Temperature & humidity (from DHT22) ✅
   - Wind speed (from anemometer) ✅
   - Wind direction (from wind vane) ✅
   - Rainfall amount (from rain sensor) ✅

2. **Arduino sends this data** to ESP32 through two wires (TX and RX) every 2 seconds ✅

3. **ESP32 receives the data** and shows it on the Serial Monitor ✅

4. **DPS310 barometer** is connected but not yet working (we're troubleshooting this) ⚠️

Think of it like this: Arduino is outside collecting weather data, ESP32 is inside receiving it and displaying everything!

## How the Data is Sent

Arduino sends data like this (CSV format):
```
windSpeed,direction,tempC,tempF,humidity,rainfall
```

For example:
```
2.40,SW,24.5,76.1,65.3,12.75
```
This means: Wind is 2.40 m/s from Southwest, temp is 24.5°C (76.1°F), humidity is 65.3%, and total rain is 12.75mm

## Getting Started

### Step 1: Get the Code
```bash
git clone https://github.com/yourusername/AutoWesta.git
cd AutoWesta
```
Or just download the ZIP file from GitHub and extract it.

### Step 2: Upload to Arduino
- Open `arduino_transmitter.ino` in Arduino IDE
- Connect your Arduino with USB cable
- Select Tools → Board → Arduino Uno (or your board)
- Select Tools → Port → (your Arduino port)
- Click Upload button

### Step 3: Upload to ESP32
- Open `esp32_receiver.ino` in Arduino IDE
- Connect your ESP32 with USB cable
- Select Tools → Board → ESP32 Dev Module
- Select Tools → Port → (your ESP32 port)
- Click Upload button

### Step 4: Connect Everything
- Follow the wiring diagram above
- **Important:** Connect Arduino GND to ESP32 GND (common ground!)
- Use separate USB cables to power both boards

### Step 5: See Your Data
- Keep ESP32 connected to your computer
- Open Tools → Serial Monitor
- Set to 9600 baud
- Watch the weather data come in every 2 seconds!

## What You'll See

When everything is working, your Serial Monitor will show:

```
COMPLETE WEATHER DATA
=====================
Wind Speed: 2.40 m/s
Wind Direction: SW
Temperature: 24.5°C (76.1°F)
Humidity: 65.3%
Total Rainfall: 12.75 mm
```

This updates every 2 seconds with fresh data!

**Note:** Barometer readings will be added once we fix the DPS310 communication issue.

## Project Structure

```
AutoWesta/
├── arduino_transmitter/
│   └── arduino_transmitter.ino
├── esp32_receiver/
│   └── esp32_receiver.ino
├── docs/
│   └── wiring_diagram.png
└── README.md
```

## Known Issues

### DPS310 Barometer
The barometer hardware is connected but not currently reading data. We're working on fixing this. Possible causes:
- I2C address conflict
- Library compatibility issue
- Wiring problem (even though connections look correct)

If you figure this out, please let us know!

## Common Problems & Fixes

### ESP32 shows "Waiting for data..." but nothing comes
- **Fix:** Check if TX and RX wires are crossed (Arduino TX → ESP32 RX)
- **Fix:** Make sure Arduino GND and ESP32 GND are connected together
- **Fix:** Both codes should use 9600 baud rate

### "DPS310 not found!" error
- **Fix:** Check I2C wires (SDA to Pin 21, SCL to Pin 22)
- **Fix:** Make sure DPS310 is connected to 3.3V, NOT 5V!
- **Fix:** Try scanning for I2C devices to see if it's detected

### DHT22 shows "ERROR - Failed to read"
- **Fix:** Check if data wire is connected to Pin 4
- **Fix:** Make sure sensor has power (VCC and GND)
- **Fix:** Wait 2-3 seconds after powering on before reading

### Wind speed always shows 0
- **Fix:** Make sure anemometer is connected to Pin 2
- **Fix:** Try spinning it manually to test
- **Fix:** Check if the sensor needs power

## What We Want to Add Later

- [ ] Fix the DPS310 barometer communication (top priority!)
- [ ] WiFi to send data to the cloud
- [ ] A website to see weather data from anywhere
- [ ] Save data to SD card
- [ ] Send weather updates to our phones
- [ ] Make weather predictions based on pressure changes
- [ ] Add solar panels so it runs on its own
- [ ] Add more sensors (UV index, air quality, etc.)

## Want to Help?

Feel free to fork this project and make it better! If you add cool features or fix bugs, submit a pull request.

## Credits

Thanks to:
- DFRobot for the rainfall sensor library
- Adafruit for their awesome sensor libraries
- Arduino and ESP32 communities for tutorials and help
- Our teacher/professor for guidance

## Questions?

If you're trying to build this and get stuck, open an issue on GitHub and we'll try to help!

---

**AutoWesta** - A student weather station project 🌦️
