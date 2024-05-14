#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


const char* ssid = "genestor_connect";
const char* password = "13mAh457*";

// Replace with the IP address of your Express.js server
const char* serverAddress = "http://localhost:27070"; // Change the port if needed

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  
  sendSensorDataToServer();
}

void loop() {

}

void sendSensorDataToServer() {
  String jsonPayload = "{\"sensor\": \"value\"}"; // Replace with your data

  HTTPClient http;
  http.begin(serverAddress + "/esp8266");
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(jsonPayload);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("HTTP Error: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}
