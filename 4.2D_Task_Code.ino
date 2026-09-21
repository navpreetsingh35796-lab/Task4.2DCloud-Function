#include <WiFiNINA.h>

// Wi-Fi credentials
const char ssid[] = "Navi";
const char pass[] = "NAVPREET";

// Pin assignments for the three room LEDs
const int PIN_LIVING_ROOM = 2;
const int PIN_BATHROOM    = 3;
const int PIN_CLOSET      = 4;

// LED state tracking variables
bool stateLivingRoom = false;
bool stateBathroom   = false;
bool stateCloset     = false;

WiFiServer server(80);

// Required function: takes a string argument and toggles the matching LED
void toggleRoom(String room) {
  room.trim();
  room.toLowerCase();

  if (room == "living room") {
    stateLivingRoom = !stateLivingRoom;
    digitalWrite(PIN_LIVING_ROOM, stateLivingRoom ? HIGH : LOW);
    Serial.println("Living Room light toggled: " + String(stateLivingRoom ? "ON" : "OFF"));
  } else if (room == "bathroom") {
    stateBathroom = !stateBathroom;
    digitalWrite(PIN_BATHROOM, stateBathroom ? HIGH : LOW);
    Serial.println("Bathroom light toggled: " + String(stateBathroom ? "ON" : "OFF"));
  } else if (room == "closet") {
    stateCloset = !stateCloset;
    digitalWrite(PIN_CLOSET, stateCloset ? HIGH : LOW);
    Serial.println("Closet light toggled: " + String(stateCloset ? "ON" : "OFF"));
  } else {
    Serial.println("Unrecognized room command: " + room);
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial && millis() < 3000);

  // Configure LED pins as outputs
  pinMode(PIN_LIVING_ROOM, OUTPUT);
  pinMode(PIN_BATHROOM, OUTPUT);
  pinMode(PIN_CLOSET, OUTPUT);

  // Ensure LEDs start OFF
  digitalWrite(PIN_LIVING_ROOM, LOW);
  digitalWrite(PIN_BATHROOM, LOW);
  digitalWrite(PIN_CLOSET, LOW);

  // Connect to the Wi-Fi network
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  server.begin();
  Serial.println("\nWi-Fi connection established.");
  Serial.print("Server running at: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // Parse incoming HTTP GET requests and extract room parameter
    if (request.indexOf("GET /toggle?room=living+room") >= 0 || request.indexOf("GET /toggle?room=living%20room") >= 0) {
      toggleRoom("living room");
    } else if (request.indexOf("GET /toggle?room=bathroom") >= 0) {
      toggleRoom("bathroom");
    } else if (request.indexOf("GET /toggle?room=closet") >= 0) {
      toggleRoom("closet");
    }

    // Send HTTP header response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();

    // Send the HTML control panel directly to the client browser
    client.println("<!DOCTYPE html>");
    client.println("<html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'>");
    client.println("<style>");
    client.println("body { font-family: Arial, sans-serif; text-align: center; padding: 25px; background: #eef2f5; }");
    client.println(".panel { background: white; padding: 25px; border-radius: 10px; max-width: 320px; margin: auto; box-shadow: 0 4px 10px rgba(0,0,0,0.1); }");
    client.println("h2 { color: #333; margin-top: 0; }");
    client.println("a { display: block; margin: 12px 0; padding: 12px; background: #007bff; color: white; text-decoration: none; border-radius: 6px; font-weight: bold; }");
    client.println(".bath { background: #28a745; }");
    client.println(".closet { background: #6f42c1; }");
    client.println("</style></head><body>");
    client.println("<div class='panel'>");
    client.println("<h2>Linda's Light Controls</h2>");
    client.println("<p>Remote toggles for carers</p>");
    client.println("<a href='/toggle?room=living%20room'>Toggle Living Room</a>");
    client.println("<a class='bath' href='/toggle?room=bathroom'>Toggle Bathroom</a>");
    client.println("<a class='closet' href='/toggle?room=closet'>Toggle Closet</a>");
    client.println("</div></body></html>");

    delay(5);
    client.stop();
  }
}
