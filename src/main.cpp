#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include "DHT.h"
// ================= CONFIG =================
const char* ssid = "KKL";
const char* password = "11111111";

// const char* httpServerUrl = "http://10.129.69.1:3000/esp32/button"; // HTTP POST
const char* deviceId = "ESP32_1";

const char* tcpServerIP = "10.129.69.1"; // TCP server IP
const uint16_t tcpServerPort = 4000;

const char* serverUrl = "http://10.129.69.1:3000/esp32/data";  // change to your server

#define DHTPIN 4
#define DHTTYPE DHT11

#define SOIL_PIN 34
#define WATER_PIN 35

DHT dht(DHTPIN, DHTTYPE);

void connectWiFi();
void reconnectTCP();
void connectTCP();
void sendMessageToHTTP();
WiFiClient tcpClient;

// ================= SETUP =================
void setup() {
 Serial.begin(115200);
  delay(1000);
  dht.begin();
  connectWiFi();
  connectTCP();
  reconnectTCP();
}

// ================= LOOP =================
void loop() {
 float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  int soilRaw = analogRead(SOIL_PIN);
  int waterRaw = analogRead(WATER_PIN);

  // Convert analog (0–4095) to percentage
  int soilPercent = map(soilRaw, 4095, 0, 0, 100);
  int waterPercent = map(waterRaw, 0, 4095, 0, 100);

  Serial.println("------ Sensor Data ------");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Soil Moisture: ");
  Serial.print(soilPercent);
  Serial.println(" %");

  Serial.print("Water Level: ");
  Serial.print(waterPercent);
  Serial.println(" %");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.setTimeout(10000); 
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{";
    jsonData += "\"temperature\":" + String(temperature)   + ",";
    jsonData += "\"humidity\":" + String(humidity)  + ",";
    jsonData += "\"soil_moisture\":" + String(soilPercent) + ",";
    jsonData += "\"water_level\":" + String(waterPercent);
    jsonData += "}";
    Serial.println(jsonData);
    int httpResponseCode = http.POST(jsonData);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    http.end();
  }

  delay(5000);  // send every 
  }



// ================= FUNCTIONS =================
void connectWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi Connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ WiFi connection failed");
  }
}

void connectTCP() {
  Serial.print("Connecting to TCP server...");
  if (tcpClient.connect(tcpServerIP, tcpServerPort)) {
    Serial.println("✅ TCP Connected");
    tcpClient.print("REGISTER:" + String(deviceId) + "\n"); // register device
  } else {
    Serial.println("❌ TCP connection failed");
  }
}

void sendMessageToHTTP() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ WiFi not connected, cannot send");
    return;
  }

  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");

  String jsonData = String("{\"deviceId\":\"") + deviceId +
                    String("\",\"message\":\"Button pressed\"}");

  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    Serial.print("✅ HTTP Response Code: ");
    Serial.println(httpResponseCode);
    Serial.println(http.getString());
  } else {
    Serial.print("❌ HTTP Error: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void reconnectTCP() {
  if (tcpClient.connected()) return; // already connected

  Serial.print("Reconnecting TCP...");
  tcpClient.stop(); // close any previous connection

  int retry = 0;
  while (!tcpClient.connect(tcpServerIP, tcpServerPort) && retry < 10) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  if (tcpClient.connected()) {
    Serial.println("✅ TCP Reconnected");
    tcpClient.print("REGISTER:" + String(deviceId) + "\n");
  } else {
    Serial.println("❌ TCP reconnect failed");
  }
}

