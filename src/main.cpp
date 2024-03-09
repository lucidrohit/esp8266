#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>


const char *ssid = "ESP8266";
const char *password = "lucidrohit";

const int webSocketPort = 81;
WebSocketsServer webSocket = WebSocketsServer(webSocketPort);

void moveWithAngle(int speed, int angle) {
  // Add your logic to control the rover based on speed and angle
  Serial.printf("Moving with speed: %d, angle: %d\n", speed, angle);
}

void moveAtAngle(int speed, int angle) {
  // Add your logic to control the rover based on speed and angle
  Serial.printf("Moving at angle with speed: %d, angle: %d\n", speed, angle);
}

void twirl(int speed, int rotation) {
  // Add your logic to control the rover based on speed and rotation
  Serial.printf("Twirling with speed: %d, rotation: %s\n", speed, (rotation == 0) ? "clockwise" : "anticlockwise");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch(type) {
    // ... (existing cases)

    case WStype_TEXT:
      Serial.printf("[%u] Received text: %s\n", num, payload);

      // Parse the JSON payload
      DynamicJsonDocument doc(512);
      deserializeJson(doc, payload);

      // Check function name and call corresponding function
      String functionName = doc["function"];
      if (functionName == "moveWithAngle") {
        int speed = doc["speed"];
        int angle = doc["angle"];
        moveWithAngle(speed, angle);
      } else if (functionName == "moveAtAngle") {
        int speed = doc["speed"];
        int angle = doc["angle"];
        moveAtAngle(speed, angle);
      } else if (functionName == "twirl") {
        int speed = doc["speed"];
        int rotation = doc["rotation"];
        twirl(speed, rotation);
      } else {
        Serial.println("Unknown function");
      }

      break;

  }
}


void setup() {
  Serial.begin(115200);
  // Set up Access Point with a fixed IP address
  IPAddress ip(192, 168, 4, 1); // Set the desired IP address
  IPAddress gateway(192, 168, 4, 1); // Set the gateway IP address
  IPAddress subnet(255, 255, 255, 0); // Set the subnet mask
  WiFi.softAPConfig(ip, gateway, subnet);

  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(ip);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}
