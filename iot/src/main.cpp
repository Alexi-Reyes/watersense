#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

const int IN1_PIN = 26;                    // the Arduino pin connected to the IN1 pin L298N
const int IN2_PIN = 25;                    // the Arduino pin connected to the IN2 pin L298N
const char *ssid = "Galaxy A54 5G C289";   // The SSID (name) of the Wi-Fi network you want to connect to
const char *ssidpassword = "pierriclegay"; // The password of the Wi-Fi network
const deviceID = "987123";                 // random, different for each device

const char *serverPassword = "XYZABC123!";   // random, needed for server authentification
const char *serverName = "http://XXXXXXXXX"; //"http://192.168.123.206:3000/poids" in poubelle

int currentHeight = 0;
int currentWaterLevel = 0;
bool isThereWater = false;

void setup()
{
    // Code here reset and setup the detector to h0.
    pinMode(IN1_PIN, OUTPUT);
    pinMode(IN2_PIN, OUTPUT);
    Move(false, 100);
    currentHeight = 0;
}
void loop()
{
    DetectWater();
    if (currentHeight < 100 && !isThereWater)
    {
        const wasThereWater = isThereWater;
        Move(isThereWater, 1);
        if (wasThereWater = !isThereWater)
        {
            Move(isThereWater, 1);
        }
    }
    sendInfoToServer();
    delay(60000);
}
void Move(bool isUp, int distance)
{
    const int time = distance / 5;
    if (isUp)
    {
        currentHeight += distance;
        digitalWrite(IN1_PIN, HIGH);
        digitalWrite(IN2_PIN, LOW);
    }
    else
    {
        currentHeight -= distance;
        digitalWrite(IN1_PIN, LOW);
        digitalWrite(IN2_PIN, HIGH);
    }
    delay(time * 1000);
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);
    delay(1000);
    DetectWater();
    return;
}
void DetectWater()
{
    // TODO GIKFUN LOGIC
    isThereWater = true;
    currentWaterLevel = currentHeight;
    return;
}
void connectToServer()
{
    WiFi.begin(ssid, ssidpassword);
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Connected to the WiFi network");
        return true;
    }
    else
    {
        Serial.println("Connection failed");
        return false;
    }
}
void sendInfoToServer()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, serverName);
        http.addHeader("Content-Type", "application/json");
        char json[100];
        sprintf(json, "{\"deviceId\":\"%s\",\"password\":\"%s\",\"waterHeight\":%d}", deviceId, serverPassword, currentWaterLevel);
        int httpResponseCode = http.POST(json);
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        http.end();
    } else {
        Serial.println("Not connected to the WiFi network, trying to reconnect");
        const bool isconnected = connectToServer();
        if (isconnected)
        {
            sendInfoToServer();
        }         
    }
}