# 1 "/home/jimi/git/ARDUINO/mshome-ent-nodemcu/mshome-ent-nodemcu.ino"
# 2 "/home/jimi/git/ARDUINO/mshome-ent-nodemcu/mshome-ent-nodemcu.ino" 2
# 3 "/home/jimi/git/ARDUINO/mshome-ent-nodemcu/mshome-ent-nodemcu.ino" 2
//#include <ESP8266WebServer.h>
# 5 "/home/jimi/git/ARDUINO/mshome-ent-nodemcu/mshome-ent-nodemcu.ino" 2
# 6 "/home/jimi/git/ARDUINO/mshome-ent-nodemcu/mshome-ent-nodemcu.ino" 2

const char *ssid = JMConnection::ssid(); // Your wifi Name
const char *password = JMConnection::ssid_pass(); // Your wifi Password

const char *host = JMConnection::host(); // Your pc or server (database) IP, example : 192.168.0.0 , if you are a windows os user, open cmd, then type ipconfig then look at IPv4 Address.
const int port = JMConnection::host_port();
const bool isHttps = JMConnection::isHttps();

// put your main code here, to run repeatedly:
HTTPClient http; // Declare object of class HTTPClient
WiFiClient client;

void setup()
{
    // put your setup code here, to run once:

    delay(1000);
    Serial.begin(115200);
    WiFi.mode(WIFI_OFF); // Prevents reconnection issue (taking too long to connect)
    delay(1000);
    WiFi.mode(WIFI_STA); // This line hides the viewing of ESP as wifi hotspot

    WiFi.begin(ssid, password); // Connect to your WiFi router
    Serial.println("");

    Serial.print("Connecting");
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }

    // If connection successful show IP address in serial monitor
    Serial.println("");
    Serial.println("Connected to Network/SSID");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP()); // IP address assigned to your ESP
}

void loop()
{

    // http.begin(client, protocol + host + ":" + port + "/mshome-ent/index.php");
    http.begin(client, host, port, "/mshome-ent/index.php", isHttps);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpCode = http.GET(); // Send the request
    String payload = http.getString(); // Get the response payload

    // Serial.println("LDR Value=" + ldrvalue);
    Serial.println(httpCode); // Print HTTP return code
    Serial.println(payload); // Print request response payload

    http.end(); // Close connection

    delay(5000);
}
