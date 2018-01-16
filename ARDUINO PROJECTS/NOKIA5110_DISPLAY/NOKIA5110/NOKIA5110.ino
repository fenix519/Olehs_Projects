#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "LowPower.h"


//U8GLIB_PCD8544 u8g(8, 4, 7, 5, 6);  // CLK=8, DIN=4, CE=7, DC=5, RST=6
Adafruit_PCD8544 display = Adafruit_PCD8544(5, 6, 7, 8, 9);

void setup() {
    display.begin();
    display.clearDisplay();
    ADCSRA = 0;  // disable ADC
    
}

const char LCDcontrast = 55;
 
void loop() {





     ADCSRA = 0;  // disable ADC
     display.begin();
     display.setContrast(LCDcontrast);
    display.setTextSize(1); // размер текста 1
    display.setTextColor(BLACK); // цвет текста темный
    display.println("Hello, world!"); // отображаем надпись
    display.setTextSize(2); // увеличиваем шрифт на 1
    display.println(random(1,10000)); // выводим число от 1 до 10000
    display.setTextSize(3);
    display.print(random(1,10000));
    display.display();
    delay(1000); // задержка в секунду

    for (int j =0; j<1; j++ )
       LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
    display.clearDisplay(); // очищаем дисплей
}
