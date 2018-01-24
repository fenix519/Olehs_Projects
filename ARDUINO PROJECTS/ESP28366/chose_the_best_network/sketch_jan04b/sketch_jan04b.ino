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

  char ssid[] = "My_Test";
  char password[] = "aaa";
  
  
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



//       Serial.print("Connecting to ");
//       Serial.println(router_acess_point->Str_SSID);

//       /router_acess_point->Str_SSID.toCharArray(ssid, router_acess_point->Str_SSID.length());                          <- --------------------------problem place--------------------- 
//       /router_acess_point->Str_Password.toCharArray(password, router_acess_point->Str_Password.length());

       Serial.print("Connecting to ");
       Serial.println(router_acess_point->Str_SSID);


       router_acess_point->Str_SSID.toCharArray(ssid, router_acess_point->Str_SSID.length()+1);
       router_acess_point->Str_Password.toCharArray(password, router_acess_point->Str_Password.length()+1);

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

  client.println(generateHTML());
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

//char* string2char(String command){
//    if(command.length()!=0){
//        char *p = const_cast<char*>(command.c_str());
//        return p;
//    }
//}


String generateHTML()
{
String myHTML;
String balast; 
uint32_t freeRAM;

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
myHTML += "          ['Memory', MEM_VALUE]";
myHTML += "        ]);";
myHTML += "";
myHTML += "        var options = {";
myHTML += "          width: 400, height: 120,";
myHTML += "          redFrom: 90, redTo: 200,";
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

freeRAM = system_get_free_heap_size();
Serial.print(" freeRAM :");
Serial.print(freeRAM);
Serial.println(" bytes ");

//sensor.requestTemperatures(); 
myHTML.replace("MEM_VALUE", "32");
return(myHTML);
}


