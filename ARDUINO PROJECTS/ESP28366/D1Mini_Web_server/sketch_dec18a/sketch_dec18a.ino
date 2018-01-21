#include <MHZ19_uart.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>


const char* ssid = "SilegoUTC_TEST";             //!!!!!!!!!!!!!!!!!!!!! modify this
const char* password = "silego123";                //!!!!!!!!!!!!!!!!!!!!!modify this
 
int ledPin = D4;          //connect led pin to d4 and ground
WiFiServer server(80);

const int rx_pin = D1;  //Serial rx pin no
const int tx_pin = D2; //Serial tx pin no
MHZ19_uart mhz19;

 
void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");


  mhz19.begin(rx_pin, tx_pin);
  mhz19.setAutoCalibration(false);
  while( mhz19.isWarming() ) 
  {
    Serial.print("MH-Z19 now warming up...  status:");Serial.println(mhz19.getStatus());
    delay(1000);
  }
}

 
void loop() {

  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  } 
  if (request.indexOf("/LED=OFF") != -1){
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
 
 
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led pin is now: ");
 
  if(value == HIGH) {
    client.print("On");  
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 4 ON<br>");
  client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 4 OFF<br>");
  client.println("</html>");
 
  delay(1);

  int co2ppm = mhz19.getPPM();
  int temp = mhz19.getTemperature();

  Serial.print("co2: "); Serial.println(co2ppm);
  Serial.print("temp: "); Serial.println(temp);
  
  delay(5000);
  Serial.println("Client disconnected");
  Serial.println("");
 
}

