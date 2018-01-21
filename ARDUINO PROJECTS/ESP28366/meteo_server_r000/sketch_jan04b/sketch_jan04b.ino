#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "ESP8266WebServer.h"


#include  "Arduino.h"



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



void setup() 
{
  Serial.begin(115200);
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");
}


void loop() 
{

  
int n=0;
bool acess_flag_a=false;
uint32_t free;


network_type * networks = envirument_scan(&n);
network_type * router_acess_point;

if (n!=0) // Are networks ? 
  {  
   sort(networks, &n);
   router_acess_point = chose_best(networks, acess_list,& n ,&acess_flag_a);
  }

   
if(n!=0 && acess_flag_a) // Are known networks ? 
      {
       Serial.print("Have founded acess point:");
       Serial.println(router_acess_point->Str_SSID);
       }

if( n==0 || !acess_flag_a)   
  Serial.println("Haven't founded acess point. Have to create acess point");  

free=system_get_free_heap_size();// - ALLOCATED_RAM;
Serial.println(free);

//  1- networks haven't found. 
//  2- networks have found but not opened or known acess point.


//for(int i=0; i<n; i++)
//{  
//   Serial.print(i);
//   Serial.print(": ");
//   Serial.print(networks[i].Str_SSID);
//   Serial.print(" ");
//   Serial.print(networks[i].Power);
//   Serial.print(" ");
//   Serial.println(networks[i].acces_flag);
//   
//   delay(50);
//}
delete(networks);

Serial.println("\n\n\n");
delay(5000);

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


