# ESP8266 Sensor Monitoring System with MQTT and OLED Display

## Overview

This project utilizes an ESP8266 microcontroller to create a sensor monitoring system that counts the number of activations from three sensors. It connects to a WiFi network and publishes sensor data to an MQTT broker. An OLED display shows the current state of the sensors and their counts, while robust debounce handling ensures accurate readings.

## Features

- **WiFi Connectivity**: Connects to a specified WiFi network.
- **MQTT Integration**: Publishes sensor counts to an MQTT topic for remote monitoring.
- **Debouncing Logic**: Uses interrupts and debounce mechanisms to ensure accurate sensor activation detection.
- **OLED Display**: Displays sensor states and counts for easy monitoring.
- **Flexible Sensor Inputs**: Supports multiple sensors with individual count tracking.

## Hardware Requirements

- ESP8266 board (e.g., NodeMCU, Wemos D1 Mini)
- Three momentary push buttons or other digital sensors
- OLED display (compatible with I2C, e.g., SSD1306)
- Breadboard and jumper wires for connections

## Wiring Diagram

- **Sensor 1**: Connect to GPIO 0 (D3)
- **Sensor 2**: Connect to GPIO 2 (D4)
- **Sensor 3**: Connect to GPIO 4 (D2)
- **OLED Display**: Connect to the I2C SDA and SCL pins on the ESP8266

## Installation

1. **Set Up the Hardware**:
   - Connect the ESP8266 to your computer.
   - Wire the components according to the specified GPIO pins.

2. **Install Required Libraries**:
   - Make sure you have the following libraries installed in your Arduino IDE:
     - `ESP8266WiFi`
     - `PubSubClient`
     - `ArduinoJson`
     - `Adafruit_GFX`
     - `Adafruit_SSD1306`

3. **Configure WiFi and MQTT Settings**:
   - Update the `ssid`, `password`, and `mqtt_server` variables in the code with your network credentials and MQTT broker information.

4. **Upload the Code**:
   - Open the Arduino IDE, load the provided code, and upload it to your ESP8266 board.

## Usage

1. Once powered on, the device connects to the specified WiFi network.
2. Press any of the sensors to trigger a count. Each sensor's activation increments its respective counter.
3. The current state and count of each sensor are displayed on the OLED screen.
4. The system publishes the sensor counts to the MQTT topic for remote monitoring.

## Functions Overview

- `ISR1()`, `ISR2()`, `ISR3()`: Interrupt service routines for each sensor, counting activations and handling debouncing.
- `setup_wifi()`: Connects the ESP8266 to the WiFi network.
- `setup_mqtt()`: Initializes MQTT client and sets the callback.
- `loop()`: Main loop that updates the OLED display and handles MQTT connectivity.
- `setAlarm_and_Publish()`: Publishes the sensor count data to the MQTT topic.
- `setMqttReconnectInterval()`: Checks and maintains the MQTT connection.

## Notes

- Ensure that the MQTT broker is running and accessible to the ESP8266 for proper functionality.
- The debounce threshold can be adjusted by changing the `debounceDelay` variable in the code.

## Contributing

Contributions are welcome! Feel free to submit issues or pull requests for enhancements or bug fixes.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
