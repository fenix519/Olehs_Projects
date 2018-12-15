#include <SPI.h>
#include <Wire.h>
#include <Encoder.h>
//#include <EEPROM.h>
#include <radio.h>
#include <si4703.h>

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define FIX_BAND     RADIO_BAND_FM   ///< The band that will be tuned by this sketch is FM.
#define FIX_STATION  10080            ///< The station that will be tuned by this sketch is 89.30 MHz.
#define FIX_VOLUME   4               ///< The volume that will be set by this sketch is level 4.
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(11, 10, 9, 8, 7);


Encoder knobFrequency(3, 4);
Encoder knobVolume(5, 6);
//
SI4703 radio;    // Create an instance of Class for Si4703 Chip
   
  int frequency = 0; //radio.getFrequency();  
  int volume = 0;// radio.getVolume();
  int frequency_step = 10;
  int max_frequency  = 10800;
  int min_frequency = 8700;
  int max_volume  = 15;
  int min_volume = 0;
  int f_dir=1;
  int v_dir=1;

  long positionFrequency  = -999;
  long positionVolume = -999;


void my_display()
{
   display.clearDisplay();
   display.println("");
   display.print("FRQ:");
   display.print(double(double(frequency)/100));
   display.println(" MHz");
   display.println("");
   display.print("VOL:");
   display.print(double((double(volume)*100)/max_volume));
   display.println("%");
   display.display();
}


void setup() 
  {
    Serial.begin(9600);
   // ----------------- Display setup -------------------------------------------
   display.begin();
   // init done
    // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.setRotation(2);
  display.clearDisplay();
  display.println("Oleh's radio ");
  display.println("reciver V01");
  display.println("12/10/2018");
  display.display();
  delay(2000);
  //------------------- Radio setup -------------------------------------------- 

  radio.debugEnable();
  // Initialize the Radio 
  radio.init();
  // Set all radio setting to the fixed values.
  radio.setBandFrequency(FIX_BAND, FIX_STATION);
  radio.setVolume(FIX_VOLUME);
  radio.setMono(false);
  radio.setMute(false);
  frequency = radio.getFrequency(); 
  volume = radio.getVolume();
  
  my_display();
  }

void loop() {
  // put your main code here, to run repeatedly:
  long newFrequency, newVolume;
  newFrequency = knobFrequency.read();
  newVolume = knobVolume.read();
  
  if (newFrequency != positionFrequency || newVolume != positionVolume)  
  {
    Serial.print("Left = ");
    Serial.print(newFrequency);
    Serial.print(", Right = ");
    Serial.print(newVolume);
    Serial.println();
  

   if((positionFrequency%4==0) && (newFrequency != positionFrequency))
   {
      if(newFrequency > positionFrequency)
        f_dir=1;
      if(newFrequency < positionFrequency) 
        f_dir=-1;
    frequency += f_dir*10; 
        if( frequency > max_frequency)
          frequency = min_frequency; 
        if( frequency < min_frequency)
          frequency = max_frequency;
          
    radio.setBandFrequency(FIX_BAND, frequency);
    my_display(); 
   }

   if((positionVolume%4==0) && (newVolume != positionVolume))
   {
      if(newVolume > positionVolume)
        v_dir=1;
      if(newVolume < positionVolume)
        v_dir=-1;
      volume += v_dir;
      if( volume > max_volume )
        volume = max_volume;
      if( volume < min_volume )
        volume = min_volume;   
      radio.setVolume(volume);
      my_display();
   }
    positionFrequency = newFrequency;
    positionVolume = newVolume;
   }
  }
