#include <WiFi.h>
#include <HTTPClient.h>

const int IN1_PIN = 26;                    // the Arduino pin connected to the IN1 pin L298N
const int IN2_PIN = 25;
const int sensorPin = 23;                    // the Arduino pin connected to the IN2 pin L298N
const char *ssid = "";   // The SSID (name) of the Wi-Fsi network you want to connect to
const char *ssidpassword = ""; // The password of the Wi-Fi network
const char *deviceID = "1";                 // random, different for each device

const char *serverPassword = "";   // random, needed for server authentification
const char *serverName = "https://apiIp:8000/api/water_senses";

bool isConnected = false;
int currentHeight = 0;
int currentWaterLevel = 0;
bool isThereWater = false;
int sensorValue;

void setup()
{
  // Code here reset and setup the detector to h0.
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(sensorPin, INPUT_PULLUP);
  // Move(false, 100);
  currentHeight = 0;
  Serial.begin(115200);
  connectToServer();
}

void loop()
{
  sensorValue = digitalRead(sensorPin);
  DetectWater();
  if (currentHeight < 100)
  {
    bool wasThereWater = isThereWater;
    Move(isThereWater, 1);
    DetectWater();
    if (wasThereWater == !isThereWater)
    {
      Serial.println("inverse");
      Move(isThereWater, 10);
    }
  }
  Serial.print("test toto");
  Serial.println(currentWaterLevel);
  sendInfoToServer();
  delay(5000);
}

void Move(bool isUp, int distance)
{
  Serial.println("Moving");
  const int time = distance * 200;
  if (isUp)
  {
    currentHeight += distance;
    if(currentHeight > 100) {
      currentHeight = 100;
    }
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, LOW);
  }
  else
  {
    currentHeight -= distance;
    if(currentHeight < 0) {
      currentHeight = 0;
    }
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, HIGH);
  }
  delay(time);
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  delay(1000);
  DetectWater();
  return;
}

void DetectWater()
{
  if (sensorValue == LOW) { // When the water level is detected (when the sensor reads LOW)
    Serial.println("Water Level Detected");
    currentWaterLevel = currentHeight;
    isThereWater = true;
  } else { // When the water level is not detected (when the sensor reads HIGH)
    Serial.println("Water Level Not Detected");
    isThereWater = false;
  }

  
  return;
}

void connectToServer()
{
  WiFi.begin(ssid, ssidpassword);
  
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connection in progress");
  }
  Serial.println("Connected to the WiFi network");
  isConnected = true;
}

void sendInfoToServer()
{
  if (isConnected)
  {
    HTTPClient http;
    http.begin(String(serverName) + "/" + String(deviceID));
    http.addHeader("Content-Type", "application/merge-patch+json");
    char json[100];
    sprintf(json, "{\"water_level\":%d}", currentWaterLevel);
    int httpResponseCode = http.PATCH(json);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      String responsePayload = http.getString(); // Get the response payload
      Serial.print("Server Response: ");
      Serial.println(responsePayload);
    } else {
      Serial.print("HTTP Error: ");
      Serial.print(httpResponseCode);
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }
    http.end();
    Serial.println("HTTP connection ended.");
  } else {
    Serial.println("Not connected to the WiFi network, trying to reconnect");
    connectToServer();
    if (isConnected)
    {
      sendInfoToServer();
    }         
  }
}