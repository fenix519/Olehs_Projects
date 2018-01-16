#include <Wire.h>
#include <Streaming.h>

void setup() {
  Wire.begin();                // join i2c bus (address optional for master)
  Serial.begin(9600);          // start serial communication at 9600bps
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);    
}


int reading = 0;

void loop() 
{
 //Si514_WriteExemple();
 //delay(10000);
 // Si514_ReadRegMap();.
 // delay(5000);                  // wait a bit since people have to read the output :) 
 Transiver();
 delay(250);
}

void Transiver()
{
  String bufer="";
  byte   Adrr=0;
  byte   Val=0;
  byte   count=0;


  while (Serial.available() == 0);
  bufer = Serial.readString();

  
  if(bufer=="start")
  {
   while(bufer!="stop")
    { 
        digitalWrite(13, HIGH);   // Enter in prog mode  
        while (Serial.available()==0);
        bufer = Serial.readString();
        if(bufer!="stop")
           {                 
             Serial.println(byte(bufer.toInt()));
             if(count%2==0)
                Adrr=byte(bufer.toInt());
             else 
                {
                 Val=byte(bufer.toInt());
                 //RegWrite(85,Adrr,Val);
                }                
           count++;
           }   
       //int val = Serial.parseInt();        
       //Serial.println(byte(val));   
     }
  }
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW

}



void Si514_ReadRegMap()
{
  byte   reg_Adress[]= {0,5,6,7,8,9,10,11,14,128,132,48,49};
  Wire.beginTransmission(85); // transmit to device # (0x55)
  //Wire.write(byte(0x00));
  Wire.endTransmission();      // stop transmitting

  Serial << "Read user register maps of Si514" << endl;
 
  for( int i=0; i < sizeof(reg_Adress) ; i++ )
     {
      Wire.beginTransmission(85); // transmit to device # (0x55)
      Wire.write(reg_Adress[i]);  // write register adress     
      Wire.endTransmission();      
     
      Wire.requestFrom(85, 1); 
      
      if (1 <= Wire.available()) 
        { // if two bytes were received
        reading = Wire.read();        // receive  byte
        Wire.endTransmission();      // stop transmitting
        Serial << "Register" << reg_Adress[i] << ": 0x" << _HEX(reading) << endl;
        }
     }
     Serial << "\n\n" << endl;
}

void RegWrite (byte SlvAdr, byte RegAdress , byte RegValue)
{ 
  Wire.beginTransmission(SlvAdr); // transmit to device # (0x55)
  Wire.write(byte(RegAdress));
  Wire.write(byte(RegValue));
  Wire.endTransmission();      // stop transmitting 
 }


void Si514_WriteExemple()
{
 Serial << "Write user register maps of Si514" << endl;
 
 RegWrite(85,132,0x0);
 
 RegWrite(85,0,0x23);
 RegWrite(85,5,0xCB);
 RegWrite(85,6,0x35);
 RegWrite(85,7,0x50);
 RegWrite(85,8,0x11);
 RegWrite(85,9,0x8);
 RegWrite(85,10,0xE); 
 RegWrite(85,11,0x00);
 
 RegWrite(85,132,0x1);
 RegWrite(85,132,0x4);
 Serial << "\n\n" << endl;
}




