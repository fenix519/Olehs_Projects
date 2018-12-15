#include "iostm8l151k6.h"



void SomeDelay()
{
  for (unsigned long delay_count=0; delay_count<50000; delay_count++);
};  

int main( void )
{
  PC_DDR_bit.DDR7 = 1; 
  PC_CR1_bit.C17 = 1;
 
  
  while (1) 
  {
   PC_ODR_bit.ODR7 = 1; 
   SomeDelay(); 
   PC_ODR_bit.ODR7 = 0; 
   SomeDelay();
  };
  
  return 0;
}
