
// include the library code:
//#include <LiquidCrystal.h>
#include <Wire.h>
#include "SparkFunHTU21D.h"
#include <SFE_BMP180.h>
#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>         //http://www.arduino.cc/playground/Code/Time  
#include "LowPower.h"
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Streaming.h>




  
//#define ALTITUDE 289.0 // Altitude of Lviv in meters


// initialize the library with the numbers of the interface pins
//Create an instance of the object

// You will need to create an SFE_BMP180 object, here called "pressure":

Adafruit_PCD8544 display = Adafruit_PCD8544(5, 6, 7, 8, 9);




HTU21D myHumidity;
SFE_BMP180 pressure;

double baseline; // baseline pressure
//unsigned char  repl=0;
const char LCDcontrast = 55;
int PowPin = 4;

void setup() {
  // Print a message to the LCD.
  
  //for( int i=0; i< 1 ; i++)     // Sleep  for charg super cap
  //      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 

  pinMode(PowPin, OUTPUT); // Power on Sensors and RTC
  digitalWrite(PowPin, HIGH);
  
  display.begin();
  display.clearDisplay(); 
  display.setContrast(LCDcontrast);
  display.setTextSize(1); 
  display.setTextColor(BLACK); 
  display<<"Load energy." << "\n" << "Please wait !";
  display.display(); 
  
  delay(250);
  
  myHumidity.begin();
  pressure.begin();
  baseline = getPressure();
  //SetTime();
}

void loop() {


  digitalWrite(PowPin, LOW);  // Power off Sensors and RTC  
  for( int i=0; i<4 ; i++)
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 

  
  digitalWrite(PowPin, HIGH);  // Power on Sensors and RTC
  delay(50);

  // read data from sensors 
  float humd = myHumidity.readHumidity();
  float temp = myHumidity.readTemperature();
  double m_pressure = getPressure();
  double m_altitude = pressure.altitude(m_pressure,baseline);
  tmElements_t tm;
  setSyncProvider(RTC.get); 

  display.clearDisplay(); 
  display.setContrast(LCDcontrast);
  display.setTextSize(1); 
  display.setTextColor(BLACK); 
  display << year() << " " << monthShortStr(month()) << " " << day() << "\n"; 
  display  << dayShortStr(weekday()) << " " <<  hour() << ":" << minute() << "\n"; 
  display << "\n"; 
  display << "Tmp:" <<  temp << "C" <<  "\n" ;
  display << "Hdt:" <<  humd << "%" <<  "\n" ;
  display << "Pr:" <<  m_pressure/1.3332239 << "mmHg" <<  "\n" ;
  display.display(); 
  
}


void SetTime()
{  
  tmElements_t tm; // set time
  tm.Hour = 19;             
  tm.Minute = 56;
  tm.Second = 00;
  tm.Day = 29;
  tm.Month = 07;
  tm.Year = CalendarYrToTm(2016);    
  RTC.write(tm); 
}

double getPressure()
{
  char status;
  double T,P,p0,a;

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        
      }
    }
  }
}
