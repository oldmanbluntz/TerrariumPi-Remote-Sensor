#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebSrv.h>

// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "Password";

// Create an instance of the web server
AsyncWebServer server(80);

// Data pin for the DS18B20 sensors
const int oneWireBus = 14; // D5 on NodeMCU-32

// DHT11 sensor
#define DHTPIN 12 // D6 on NodeMCU-32
#define DHTTYPE DHT11

// BH1750 LUX sensor
BH1750 lightSensor;

// Create instances for temperature sensors
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// Create instance for DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize temperature sensors
  sensors.begin();

  // Initialize BH1750 LUX sensor
  if (!lightSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 initialization failed!");
  }

  // Initialize DHT sensor
  dht.begin();

  // Define web server routes
  server.on("/ds18b20/sensor1#temperature", HTTP_GET, handleDS18B20Sensor1);
  server.on("/ds18b20/sensor2#temperature", HTTP_GET, handleDS18B20Sensor2);
  server.on("/ds18b20/sensor3#temperature", HTTP_GET, handleDS18B20Sensor3);
  server.on("/dht11", HTTP_GET, handleDHT11);
  server.on("/bh1750", HTTP_GET, handleBH1750);

  // Start the server
  server.begin();
}

void loop() {
  // No need for additional loop logic when using AsyncWebServer
}

void handleDS18B20Sensor1(AsyncWebServerRequest *request) {
  // Handle DS18B20 sensor 1 data here
  // You can use sensors.getTempCByIndex(0) for the first sensor, sensors.getTempCByIndex(1) for the second sensor, and so on
  DynamicJsonDocument jsonDoc(256); // Adjust the size as needed
  JsonObject ds18b20Data = jsonDoc.to<JsonObject>();
  float tempC = sensors.getTempCByIndex(0); // Assuming first DS18B20 sensor
  if (tempC != DEVICE_DISCONNECTED_C) {
    ds18b20Data["sensor_type"] = "DS18B20";
    ds18b20Data["temperature_C"] = tempC;
  }
  String jsonString;
  serializeJson(ds18b20Data, jsonString);
  request->send(200, "application/json", jsonString);
}

void handleDS18B20Sensor2(AsyncWebServerRequest *request) {
  // Handle DS18B20 sensor 2 data here
  // You can use sensors.getTempCByIndex(1) for the second sensor, and so on
  DynamicJsonDocument jsonDoc(256); // Adjust the size as needed
  JsonObject ds18b20Data = jsonDoc.to<JsonObject>();
  float tempC = sensors.getTempCByIndex(1); // Assuming second DS18B20 sensor
  if (tempC != DEVICE_DISCONNECTED_C) {
    ds18b20Data["sensor_type"] = "DS18B20";
    ds18b20Data["temperature_C"] = tempC;
  }
  String jsonString;
  serializeJson(ds18b20Data, jsonString);
  request->send(200, "application/json", jsonString);
}

void handleDS18B20Sensor3(AsyncWebServerRequest *request) {
  // Handle DS18B20 sensor 3 data here
  // You can use sensors.getTempCByIndex(2) for the third sensor, and so on
  DynamicJsonDocument jsonDoc(256); // Adjust the size as needed
  JsonObject ds18b20Data = jsonDoc.to<JsonObject>();
  float tempC = sensors.getTempCByIndex(2); // Assuming third DS18B20 sensor
  if (tempC != DEVICE_DISCONNECTED_C) {
    ds18b20Data["sensor_type"] = "DS18B20";
    ds18b20Data["temperature_C"] = tempC;
  }
  String jsonString;
  serializeJson(ds18b20Data, jsonString);
  request->send(200, "application/json", jsonString);
}

void handleDHT11(AsyncWebServerRequest *request) {
  DynamicJsonDocument jsonDoc(256); // Adjust the size as needed
  JsonObject dhtData = jsonDoc.to<JsonObject>();
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (!isnan(humidity) && !isnan(temperature)) {
    dhtData["sensor_type"] = "DHT11";
    dhtData["humidity"] = humidity;
    dhtData["temperature_C"] = temperature;
  }
  String jsonString;
  serializeJson(dhtData, jsonString);
  request->send(200, "application/json", jsonString);
}

void handleBH1750(AsyncWebServerRequest *request) {
  DynamicJsonDocument jsonDoc(256); // Adjust the size as needed
  JsonObject luxData = jsonDoc.to<JsonObject>();
  float lux = lightSensor.readLightLevel();
  if (!isnan(lux)) {
    luxData["sensor_type"] = "BH1750";
    luxData["lux"] = lux;
  }
  String jsonString;
  serializeJson(luxData, jsonString);
  request->send(200, "application/json", jsonString);
}
