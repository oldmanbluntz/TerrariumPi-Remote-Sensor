# TerrariumPi-Remote-Sensor
Remote Sensor code for an ESP32 development board to transmit data to Terrarium Pi

It uses 3 DS1820 temperature sensors on GPIO14, 1 DHT11 temperature/humidity sensor on GPIO12, and a single BH1750 LUX sensor on I2C.

You will have to add the ESP32 with the boards manager. esp32 by Espressif Systems.

you will need the following libraries installed on ArduinoIDE:
Adafruit Unified Sensor
ArduinoJSON
BH1750
DHT sensor library
DallasTemperature
ESPAsyncWebSrv
onewire
