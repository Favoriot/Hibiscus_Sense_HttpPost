#include <WiFi.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

char ssid[] = "your ssid";
char password[] = "your password";

unsigned long start, finished, elapsed;

char server[] = "apiv2.favoriot.com";
String PATH = "/v2/streams";
int port = 80;
String apikey = "";
char device_developer_id[] = "";

WiFiClient client;

void setup() {
   Serial.begin(115600);
   WiFi.begin(ssid,password);

   while(WiFi.status() != WL_CONNECTED){
     Serial.print(".");
     delay(500); 
   }
   printWifiStatus();
   if(!bme.begin()){
     Serial.println("Failed to find BME280 chip"); 
   }
}

void printWifiStatus(){
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
 }


String dataStreams()
{   
     // Data streams format
    String json = "{\"device_developer_id\":\"" + String(device_developer_id) + "\",\"data\":{";

     json += "\"temperature\":\"" + String(bme.readTemperature()) + "\"";
    
     json += "}}";
    
    return json;
}

void sendStreams(String streams){
    if (client.connect(server, port)) {   // If there's a successful connection
    Serial.println("STATUS : Sending data...");
    client.println("POST "+ PATH + " HTTP/1.1");
    client.println("Host: " + String(server));
    client.print(String("apikey:"));
    client.println(apikey);
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    int thisLength = streams.length();
    client.println(thisLength);
    client.println("Connection: close");
    client.println();
    client.println(streams);
    Serial.println("STATUS : Data sent!");  //display sent status
    }
    else {
    // If you didn't get a connection to the server:
    Serial.println(F("connection failed"));
  }
}

void loop() {
    start=millis();  //start counting time
    String json = dataStreams();
    Serial.println("Data Streams:"+json);
    sendStreams(json);
    finished=millis();  //stop counting time
    elapsed=(finished-start);  //calculate time taken to send data
    Serial.print("PROCESSING TIME : " + String(elapsed) + " ms\n");  //display time taken
    Serial.print("**************************************************\n");
    delay(5000);
    
}
