#include <SPI.h>
#include <WiFiNINA.h>

// Wi-Fi Credentials
char ssid[] = "Navi";
char pass[] = "NAVPREET"; 

int status = WL_IDLE_STATUS;
WiFiServer server(80);

// Hardware Pin Definitions
const int PIN_LIVING_ROOM = 2;
const int PIN_BATHROOM    = 3;
const int PIN_CLOSET      = 4;

// Function prototype accepting a string argument (Task Step 3)
void toggleLight(String room);

void setup() {
  Serial.begin(9600);

  pinMode(PIN_LIVING_ROOM, OUTPUT);
  pinMode(PIN_BATHROOM, OUTPUT);
  pinMode(PIN_CLOSET, OUTPUT);

  digitalWrite(PIN_LIVING_ROOM, LOW);
  digitalWrite(PIN_BATHROOM, LOW);
  digitalWrite(PIN_CLOSET, LOW);

  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    delay(1000);
  }

  server.begin();
  Serial.println("Wi-Fi Connected successfully!");
  Serial.print("Arduino IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String requestLine = "";

    // Read the primary HTTP request line
    while (client.connected() && client.available()) {
      char c = client.read();
      if (c == '\n') break;
      if (c != '\r') requestLine += c;
    }

    // 1. Handle Preflight OPTIONS requests for CORS support
    if (requestLine.indexOf("OPTIONS") >= 0) {
      client.println("HTTP/1.1 204 No Content");
      client.println("Access-Control-Allow-Origin: *");
      client.println("Access-Control-Allow-Methods: GET, POST, OPTIONS");
      client.println("Access-Control-Allow-Headers: Content-Type");
      client.println("Connection: close\r\n");
    }
    // 2. Process incoming room toggle commands
    else {
      if (requestLine.indexOf("/api/toggle?room=") >= 0) {
        if (requestLine.indexOf("room=living") >= 0) {
          toggleLight("living room");
        } else if (requestLine.indexOf("room=bathroom") >= 0) {
          toggleLight("bathroom");
        } else if (requestLine.indexOf("room=closet") >= 0) {
          toggleLight("closet");
        }
      }

      // Send JSON response with CORS headers enabled
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: application/json");
      client.println("Access-Control-Allow-Origin: *");
      client.println("Connection: close\r\n");

      // Return current pin statuses
      client.print("{\"living\":");
      client.print(digitalRead(PIN_LIVING_ROOM));
      client.print(",\"bathroom\":");
      client.print(digitalRead(PIN_BATHROOM));
      client.print(",\"closet\":");
      client.print(digitalRead(PIN_CLOSET));
      client.println("}");
    }

    client.stop();
  }
}

// Function taking the string argument as required by Task Step 3
void toggleLight(String room) {
  room.trim();
  room.toLowerCase();

  if (room == "living room") {
    digitalWrite(PIN_LIVING_ROOM, !digitalRead(PIN_LIVING_ROOM));
    Serial.println("Living Room toggled");
  } 
  else if (room == "bathroom") {
    digitalWrite(PIN_BATHROOM, !digitalRead(PIN_BATHROOM));
    Serial.println("Bathroom toggled");
  } 
  else if (room == "closet") {
    digitalWrite(PIN_CLOSET, !digitalRead(PIN_CLOSET));
    Serial.println("Closet toggled");
  }
}