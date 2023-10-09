#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "PASSWORD";

// Replace with your Raspberry Pi's IP address and port
const char* serverAddress = "Terrarium Pi ip";
const int serverPort = 8090; //replace with your port

// Define endpoints for each sensor
const String ds18b20Endpoint = "/ds18b20";
const String dht11Endpoint = "/dht11";
const String bh1750Endpoint = "/bh1750";

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
}

void loop() {
  // Read DS18B20 sensors and send data
  sendSensorData(ds18b20Endpoint, readDS18B20Data());

  // Read DHT11 sensor and send data
  sendSensorData(dht11Endpoint, readDHT11Data());

  // Read BH1750 LUX sensor and send data
  sendSensorData(bh1750Endpoint, readBH1750Data());

  delay(60000); // Adjust the delay as needed
}

// Function to read DS18B20 sensor data and return as JSON
String readDS18B20Data() {
  DynamicJsonDocument jsonDoc(256); // Adjust the size as needed
  JsonObject ds18b20Data = jsonDoc.to<JsonObject>();
  float tempC = sensors.getTempCByIndex(0); // Assuming only one DS18B20 sensor
  if (tempC != DEVICE_DISCONNECTED_C) {
    ds18b20Data["sensor_type"] = "DS18B20";
    ds18b20Data["temperature_C"] = tempC;
  }
  String jsonString;
  serializeJson(ds18b20Data, jsonString);
  return jsonString;
}

// Function to read DHT11 sensor data and return as JSON
String readDHT11Data() {
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
  return jsonString;
}

// Function to read BH1750 LUX sensor data and return as JSON
String readBH1750Data() {
  DynamicJsonDocument jsonDoc(256); // Adjust the size as needed
  JsonObject luxData = jsonDoc.to<JsonObject>();
  float lux = lightSensor.readLightLevel();
  if (!isnan(lux)) {
    luxData["sensor_type"] = "BH1750";
    luxData["lux"] = lux;
  }
  String jsonString;
  serializeJson(luxData, jsonString);
  return jsonString;
}

// Function to send sensor data via HTTP GET request
void sendSensorData(String sensorEndpoint, String sensorData) {
  String url = "http://" + String(serverAddress) + ":" + String(serverPort) + sensorEndpoint + "#" + sensorData;
  HTTPClient http;

  if (http.begin(url)) {
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error in HTTP request. HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Failed to connect to server");
  }
}
