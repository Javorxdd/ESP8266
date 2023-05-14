#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Safrankovi";
const char* password = "Monika3168";

ESP8266WebServer server(80);

float temperature = 0.0;
float humidity = 0.0;
String rainStatus = "Neznámý";  // Příznak indikující, zda prší nebo ne

String specialCharsToHtmlEntities(String input) {
  String output = "";
  for (size_t i = 0; i < input.length(); i++) {
    if (input[i] == '<') {
      output += "&lt;";
    } else if (input[i] == '>') {
      output += "&gt;";
    } else if (input[i] == '&') {
      output += "&amp;";
    } else if (input[i] == '\"') {
      output += "&quot;";
    } else if (input[i] == '\'') {
      output += "&#39;";
    } else {
      output += input[i];
    }
  }
  return output;
}


void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Připojování k Wi-Fi...");
  }

  Serial.println("Připojeno k Wi-Fi");
  Serial.print("IP adresa: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Server spuštěn");
}

void loop() {
  while (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    // Zpracování přijatých dat (např. rozdělení teploty a vlhkosti)

    // Aktualizování hodnot teploty a vlhkosti
    if (data.startsWith("Temperature:")) {
      temperature = data.substring(12).toFloat();
    } else if (data.startsWith("Humidity:")) {
      humidity = data.substring(9).toFloat();
    } else if (data.startsWith("RainStatus:")) {
      rainStatus = data.substring(11);
    }
  }

  server.handleClient();
}

void handleRoot() {
  String jsonString = generateJsonString();
  
  // Send JSON response
  server.send(200, "application/json", jsonString);
}


String generateJsonString() {
  // Vytvoření JSON objektu
  StaticJsonDocument<200> jsonDoc;

  // Naplnění daty
  jsonDoc["temperature"] = temperature;
  jsonDoc["humidity"] = humidity;
  jsonDoc["rainStatus"] = rainStatus;

  // Převod JSON objektu na řetězec
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  return jsonString;
}

