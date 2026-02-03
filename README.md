# Arduino Plant Watering System

An ESP32-based smart plant monitoring system that tracks environmental conditions and soil moisture levels, sending data to a remote server for monitoring and automation.

## Features

- 🌡️ **Temperature Monitoring** - Real-time temperature readings using DHT11 sensor
- 💧 **Humidity Tracking** - Ambient humidity measurement
- 🌱 **Soil Moisture Detection** - Analog soil moisture sensor integration
- 🚰 **Water Level Monitoring** - Track water reservoir levels
- 📡 **WiFi Connectivity** - Wireless data transmission
- 🔗 **HTTP & TCP Communication** - Dual protocol support for server communication

## Hardware Requirements

| Component | Pin | Description |
|-----------|-----|-------------|
| ESP32 Dev Board | - | Main microcontroller |
| DHT11 Sensor | GPIO 4 | Temperature & humidity sensor |
| Soil Moisture Sensor | GPIO 34 | Analog soil moisture reading |
| Water Level Sensor | GPIO 35 | Analog water level reading |

## Wiring Diagram

```
ESP32 Pin    Component
---------    ---------
GPIO 4   --> DHT11 Data Pin
GPIO 34  --> Soil Moisture Sensor (Analog)
GPIO 35  --> Water Level Sensor (Analog)
3.3V     --> Sensors VCC
GND      --> Sensors GND
```

## Software Requirements

- [PlatformIO](https://platformio.org/) IDE or VS Code with PlatformIO extension
- ESP32 board support

## Dependencies

The following libraries are automatically installed via PlatformIO:

- `adafruit/DHT sensor library`
- `adafruit/Adafruit Unified Sensor`

## Installation

1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd ArduinoPlantWatering
   ```

2. **Configure WiFi and Server Settings**
   
   Edit `src/main.cpp` and update the following constants:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   const char* tcpServerIP = "YOUR_SERVER_IP";
   const uint16_t tcpServerPort = 4000;
   const char* serverUrl = "http://YOUR_SERVER_IP:3000/esp32/button";
   ```

3. **Build the project**
   ```bash
   pio run
   ```

4. **Upload to ESP32**
   ```bash
   pio run --target upload
   ```

5. **Monitor Serial Output**
   ```bash
   pio device monitor
   ```

## Data Format

The system sends sensor data as JSON via HTTP POST:

```json
{
  "temperature": 25.5,
  "humidity": 60.0,
  "soil_moisture": 45,
  "water_level": 80
}
```

## Serial Monitor Output

```
------ Sensor Data ------
Temperature: 25.50 °C
Humidity: 60.00 %
Soil Moisture: 45 %
Water Level: 80 %
```

## Configuration

| Parameter | Default | Description |
|-----------|---------|-------------|
| `DHTPIN` | 4 | DHT11 data pin |
| `SOIL_PIN` | 34 | Soil moisture sensor pin |
| `WATER_PIN` | 35 | Water level sensor pin |
| `monitor_speed` | 115200 | Serial baud rate |
| Data interval | 5000ms | Time between readings |

## Project Structure

```
ArduinoPlantWatering/
├── platformio.ini      # PlatformIO configuration
├── README.md           # This file
├── include/            # Header files
├── lib/                # Project-specific libraries
├── src/
│   └── main.cpp        # Main application code
└── test/               # Unit tests
```

## Troubleshooting

- **WiFi Connection Failed**: Verify SSID and password are correct
- **Sensor Reading NaN**: Check DHT11 wiring and ensure proper power supply
- **HTTP Error Codes**: Verify server URL and network connectivity
- **TCP Connection Failed**: Ensure TCP server is running and port is open

## License

This project is open source and available under the MIT License.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
