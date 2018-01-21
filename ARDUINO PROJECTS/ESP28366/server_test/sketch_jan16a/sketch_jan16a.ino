#include <ESP8266WiFi.h>





const char* ssid = "ADA";
const char* password ="cascad519";
 
int ledPin = D5;
WiFiServer server(80);

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
  Serial.println("/");}
 
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
 
//  int value = LOW;
//  if (request.indexOf("/LED=ON") != -1) {
//    digitalWrite(ledPin, HIGH);
//    value = HIGH;
//  } 
//  if (request.indexOf("/LED=OFF") != -1){
//    digitalWrite(ledPin, LOW);
//    value = LOW;
//  }

//  // Return the response
//  client.println("HTTP/1.1 200 OK");
//  client.println("Content-Type: text/html");
//  client.println(""); //  do not forget this one
//  client.println("<!DOCTYPE HTML>");
//  client.println("<html>");
// 
//  client.print("Led pin is now: ");
// 
//  if(value == HIGH) {
//    client.print("On");  
//  } else {
//    client.print("Off");
//  }
//  
//  client.println("<br><br>");
//  client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 5 ON<br>");
//  client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 5 OFF<br>");
//  client.println("</html>");


 client.println(generateHTML());
 
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
 }

String generateHTML()
{
String myHTML;
myHTML="";
myHTML+="<html>";
myHTML += "  <head> ";
myHTML += "<script type=\"text/javascript\" src=\"https://www.gstatic.com/charts/loader.js\"></script>";
myHTML += "   <script type=\"text/javascript\">";
myHTML += "      google.charts.load('current', {'packages':['gauge']});";
myHTML += "      google.charts.setOnLoadCallback(drawChart);";
myHTML += "      function drawChart() {";
myHTML += "";
myHTML += "        var data = google.visualization.arrayToDataTable([";
myHTML += "          ['Label', 'Value'],";
myHTML += "          ['Temperature', 10],";
myHTML += "          ['CO2', 600],";
myHTML += "          ['Presure', 10]";
myHTML += "        ";
myHTML += "        ]);";
myHTML += "";
myHTML += "        var options = {";
myHTML += "          width: 400, height: 120,";
myHTML += "          redFrom: 90, redTo: 100,";
myHTML += "          yellowFrom:75, yellowTo: 90,";
myHTML += "          minorTicks: 5";
myHTML += "        };";
myHTML += "";
myHTML += "        var chart = new google.visualization.Gauge(document.getElementById('chart_div'));";
myHTML += "";
myHTML += "        chart.draw(data, options);      ";
myHTML += "        ";
myHTML += "      }";
myHTML += "    </script>";
myHTML += "  </head>";
myHTML += "  <body>";
myHTML += "    <div id=\"chart_div\" style=\"width: 400px; height: 120px;\"></div>";
myHTML += "  </body>";
myHTML += "</html>";
//sensor.requestTemperatures(); 
//myHTML.replace("@id_1@", String(sensor.getTempCByIndex(0), 1));
return(myHTML);
}
