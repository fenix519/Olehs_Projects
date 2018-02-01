#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "ESP8266WebServer.h"
#include  "Arduino.h"


extern "C" 
{
#include "user_interface.h"
}

struct network_type
{
  String Str_SSID;
  String Str_Password;
  int Power;
  bool acces_flag;
};

//Knowinig acces list
network_type acess_list[] = { 
                              {"ADA","cascad519", 0, true }, 
                              {"SilegoUTC2","silego123", 0, true },
                              {"SilegoUTC","~silegoUTC123", 0, true }, 
                              {"SilegoUTC_TEST","silegoUTC123", 0, true }
                             };

network_type acess_point = {"Meteo station","", 0, true };



struct data_point
{
  float temperature;
  float hyumidity; 
  float co2;
  float presure;
};

WiFiServer server(80);


void setup() 
{
  int n=0;
  bool acess_flag_a=false;
  network_type * router_acess_point;

  char ssid[] = "access";
  char password[] = "password";
  
  
  Serial.begin(115200);
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  network_type * networks = envirument_scan(&n);
 
  if (n!=0) // Are networks ? 
  {  
   sort(networks, &n);
   router_acess_point = chose_best(networks, acess_list,& n ,&acess_flag_a);
  }

  if(n!=0 && acess_flag_a) // Are known networks ? 
      {
       Serial.print("Have founded acess point:");
       Serial.println(router_acess_point->Str_SSID);
       //----------------------------------------------------------------------------------------
       // Connect to WiFi network
          
       Serial.println();
       Serial.println();


       router_acess_point->Str_SSID.toCharArray(ssid, router_acess_point->Str_SSID.length()+1);
       router_acess_point->Str_Password.toCharArray(password, router_acess_point->Str_Password.length()+1);

       Serial.print("Connecting to ");
       Serial.println("");
       Serial.println(ssid);
       Serial.println(password);
       Serial.println("");

       
       WiFi.begin(ssid, password); 
       while (WiFi.status() != WL_CONNECTED) 
       {delay(500);
        Serial.print(".");}

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
       //----------------------------------------------------------------------------------------      
       }

  if( n==0 || !acess_flag_a)   
      {Serial.println("Haven't founded acess point. Have to create acess point");}

  Serial.println("Setup done");
}



void loop() 
{

uint32_t freeRAM; 


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

  freeRAM = system_get_free_heap_size();
  Serial.print(" freeRAM :");
  Serial.print(freeRAM);
  Serial.println(" bytes ");
  Serial.println("\n\n\n");

  //client.println(generateHTML());


  chart_generator(client);
  
  //client.close();
  delay(0.5);
  Serial.println("Client disconnected");
  Serial.println("");

}


network_type * envirument_scan(int *n)
{
  // ----------------------------------------------------------------
  network_type * buf;
  Serial.println("scan start");
  // WiFi.scanNetworks will return the number of networks found
  *n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (*n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(*n);
    Serial.println(" networks found");
    buf= new network_type[*n];  

    for (int i = 0; i<*n; i++)
    {
      buf[i].Str_SSID = WiFi.SSID(i);
      buf[i].Str_Password = "";
      buf[i].Power = WiFi.RSSI(i);
      buf[i].acces_flag = false;
        
      //Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(50);
    }
  }
return(buf);
}


void sweep_points(network_type* a, network_type* b)
{  
  String SSID_buffer = "";
  String Passwor_buffer = "";
  int  Power_buffer = 0;
  bool bool_buffer = false;

  SSID_buffer = a->Str_SSID;
  Passwor_buffer = a->Str_Password;
  Power_buffer = a->Power;
  bool_buffer = a->acces_flag;

  a->Str_SSID = b->Str_SSID;
  a->Str_Password = b->Str_Password;
  a->Power = b->Power;
  a->acces_flag = b->acces_flag;

  b->Str_SSID = SSID_buffer;
  b->Str_Password = Passwor_buffer;
  b->Power = Power_buffer;
  b->acces_flag = bool_buffer;
 }


void sort(network_type buf[], int *n)
{
  for(int x = 0; x < *n; x++)
   for(int y = 0; y < *n-1; y++)
   
     if((buf[y].Power) < (buf[y+1].Power))
       sweep_points( &buf[y], &buf[y+1]);
}

network_type* chose_best(network_type envirument_network[], network_type acess_list[], int* n ,bool * is_acess)
{
  int f_number=0;
  *is_acess= false;
  
  for(int x = 0; x < *n; x++)
  {
   for(int y = 0; y < *n; y++)
   {
    *is_acess = envirument_network[x].Str_SSID.equals(acess_list[y].Str_SSID);
    f_number = y;
    if (*is_acess) break;
   }
  if (*is_acess) break;
  }
  return(&acess_list[f_number]);
}







void chart_generator(WiFiClient client)
{

    float timeStamp[] = {0,1,2,3,4,5,6,7,8,9};
    float tempC[]     = {1,1,1,1,1,1,1,1,1,1};
    float tempHum[]   = {1,1,1,1,1,1,1,1,1,1};
    float tempPress[] = {2,2,2,2,2,2,2,2,2,2};
    float tempCo2[]   = {2,2,2,2,2,2,2,2,2,2};

    int count = int(sizeof(timeStamp)/ sizeof(float));
    int numberOfRows = 10;

    String htmlContent;
  
    //Standard html header stuff here.
    htmlContent = ("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n"); 
    htmlContent += ("<html><head><title>Meteo station</title>\n");

    //load google charts and create a button
    htmlContent += ("<script type=\"text/javascript\" src=\"https://www.google.com/jsapi?autoload={'modules':[{'name':'visualization','version':'1','packages':['corechart']}]}\"></script>\n");
    htmlContent += ("<button id=\"change-chart\"></button>");
    htmlContent += ("<script type=\"text/javascript\"> google.setOnLoadCallback(drawChart);\n");
    htmlContent += ("var button = document.getElementById('change-chart');");

    //Create a function to draw the chart and then add the data into a table
    htmlContent += ("function drawChart() {var data = google.visualization.arrayToDataTable([\n");
    htmlContent += ("['Local Time', 'Temperature C', 'Hyumidity %', 'Pressure mmHg', 'CO2 ppm'],\n");
    //Send what we have to the client (web browser)
    client.print(htmlContent);

    //Here we loop through the temp and pressure data to place it into the html source code
    for (int i = 0; i< count ; i++)
    {
      htmlContent = ("[new Date(" + String(timeStamp[i]) +  "000)," + String(tempC[i]) +  "," + String(tempHum[i]) + "," + String(tempPress[i])+ "," + String(tempCo2[i]) + "],\n");
      client.print(htmlContent);
    }
    htmlContent = ("]);\n");

    //Continue to build the rest of the web page.  Here we create three function that the buttons uses to dsiplay the chart data.
    htmlContent += ("function drawChartCelsius() {var tempCview = new google.visualization.DataView(data);\n tempCview.setColumns([0,1]);\n chart.draw(tempCview, optionsCelsius);\n button.innerText = 'Change to Humidity';\n  button.onclick = drawChartHumidity;}\n");
    htmlContent += ("function drawChartHumidity() {var tempHyum = new google.visualization.DataView(data);\n tempHyum.setColumns([0,2]);\n chart.draw(tempHyum, optionsHumidity);\n button.innerText = 'Change to Pressure';\n button.onclick = drawChartPressure;}\n");
    htmlContent += ("function drawChartPressure() {var tempPressureView = new google.visualization.DataView(data);\n tempPressureView.setColumns([0,3]);\n chart.draw(tempPressureView, optionsPressure);\n button.innerText = 'Change to CO2';\n button.onclick = drawChartCO2;}\n");
    htmlContent += ("function drawChartCO2() {var tempCO2View = new google.visualization.DataView(data);\n tempCO2View.setColumns([0,4]);\n chart.draw(tempCO2View, optionsCO2);\n button.innerText = 'Change to Celsius';\n button.onclick = drawChartCelsius;}\n");

    //specify date format and then update x labels with this time format
    htmlContent += ("var formatter = new google.visualization.DateFormat({ formatType: 'short',timeZone: 0});\n  formatter.format(data, 0);\n");
    htmlContent += ("// Set X-Axis Labels\nvar xTicks = [];\n");
    htmlContent += ("for (var i = 0; i < data.getNumberOfRows(); i++) {\n");
    htmlContent += ("   xTicks.push({\n    v: data.getValue(i, 0),\n    f: data.getFormattedValue(i, 0) });\n}\n");

    //Here are three chart options used for each chart.  E.g. colour, chart title, etc..


    htmlContent += ("var optionsPressure = {'height': 320,chartArea:{top:20, height:\"60%\"},hAxis:{gridlines:{color:'transparent'},ticks:xTicks,slantedText: true,slantedTextAngle :70,textStyle:{fontSize: 11} },vAxis:{format:\"##,### mmHg\"},series:{1:{curveType:'function'},0:{color:'orange'}},legend:{position: 'none'},title:'Pressure in mmHg' };\n");  
    htmlContent += ("var optionsCelsius = {'height': 320,chartArea:{top:20,  height:\"60%\"},hAxis:{gridlines:{color:'transparent'},ticks:xTicks,slantedText: true,slantedTextAngle :70,textStyle:{fontSize: 11} },vAxis:{format:\"##.## C\"},series:{1:{curveType:'function'},0:{color:'red'}},legend:{position: 'none'},title:'Temperature in Celsius' };\n");
    htmlContent += ("var optionsHumidity = {'height': 320,chartArea:{top:20, height:\"60%\"},hAxis:{gridlines:{color:'transparent'},ticks:xTicks,slantedText: true,slantedTextAngle :70,textStyle:{fontSize: 11} },vAxis:{format:\"##.## %\"},series:{0:{curveType: 'function'},0:{color:'Blue'}},legend:{position: 'none'},title: 'Humidity in %'};\n");
    htmlContent += ("var optionsCO2 = {'height': 320,chartArea:{top:20, height:\"60%\"},hAxis:{gridlines:{color:'transparent'},ticks:xTicks,slantedText: true,slantedTextAngle :70,textStyle:{fontSize: 11} },vAxis:{format:\"##.## ppm\"},series:{0:{curveType: 'function'},0:{color:'gray'}},legend:{position: 'none'},title: 'CO2 in ppm'};\n");
    
    client.print(htmlContent);    
  
    //Draw chart 
    htmlContent = ("var chart = new google.visualization.LineChart(document.getElementById('curve_chart'));drawChartCelsius();}\n");
    htmlContent += ("</script>\n");

    //Page heading
    htmlContent += ("<font color=\"#000000\"><body><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\">\n");
    htmlContent += ("<div id=\"curve_chart\" style=\"width: 800px; height: 300px\"></div>");

//    //Display the data and time for first and last reading
//    htmlContent += ("<BR><BR>First reading at : ");
//    timeAndDate(timeStamp[0],htmlContent);    
//    htmlContent += ("<BR>Most recent reading : ");
//    timeAndDate(timeStamp[count-1],htmlContent);     
//    htmlContent += ("<BR></body></html>\n");
    client.print(htmlContent);
}



