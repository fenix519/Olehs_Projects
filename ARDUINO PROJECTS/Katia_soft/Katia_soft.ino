#include <Servo.h>
#include <LiquidCrystal.h>

int servo_pin = 46;
int motor_pin = 50;
int direction_pin = 52;
int enable_pin = 48;



int steps = 0;
int servo_up_point = 175;
int servo_down_point = 90;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
Servo myservo;  // create servo object to control a servo

void servo_move(int angle){
  int start_angle = myservo.read(); 
 if (angle > start_angle){
   while(start_angle<=angle){
    myservo.write(start_angle);
    delay(20);
    start_angle = start_angle+4;
  }
 }else if(angle < start_angle){
 while(start_angle>=angle){
    myservo.write(start_angle);
    delay(20);
    start_angle = start_angle-4;
 }
 }
}

void setup() {

  pinMode(motor_pin, OUTPUT);
  pinMode(direction_pin, OUTPUT);

  myservo.attach(servo_pin);
  servo_move(servo_up_point);
  Serial.begin(9600);

  Serial.print("Stepper motor is OK\r\n ");

    // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2); 
  // Print a message to the LCD.   
  lcd.setCursor(0,0); 
  lcd.print("Characterization"); 
 
}

void loop() {
   int x; 
  x = analogRead (0); 
      
  if (Serial.available())
  {
    steps = Serial.parseInt();
    if (steps != 0) {
      
      StepsOn(steps);
      delay(100);
 
      Serial.print("steps: ");
      Serial.println(steps);
      lcd.setCursor(8,1);
      lcd.setCursor(0,1); 
      lcd.print("steps:  0       "); 
      steps = 0;
              
    }
    else
    { 
      Serial.print("steps: ");
      Serial.println(steps);
    }
    
    
  }
  if (x < 60) { 
    steps = steps +166; 
     lcd.setCursor(0,1); 
    lcd.print("steps:          "); 
    lcd.setCursor(8,1);
    lcd.print(steps);
    delay(300);    
  } 
  else if (x < 200) { 
    lcd.setCursor(8,1);
    lcd.print ("Up    "); 
    servo_move(servo_up_point);
    delay(500);
  } 
  else if (x < 400){
    lcd.setCursor(8,1); 
    lcd.print ("Down  "); 
    servo_move(servo_down_point);
    delay(500);
  } 
  else if (x < 600){ 
    steps = steps -166; 
    lcd.setCursor(0,1); 
    lcd.print("steps:          "); 
    lcd.setCursor(8,1);
    lcd.print(steps);
    delay(300);    
  } 
  else if (x < 800){ 
       
    StepsOn(steps);
    lcd.setCursor(0,1); 
    lcd.print("steps:  0      "); 
    steps = 0;
     
  }  
}

void StepsOn(int steps_to_move)
{
  digitalWrite(enable_pin, LOW);
  int speedStep = 12;//15
  int steps_left = abs(steps_to_move);

  if (steps_to_move > 0) { 
    digitalWrite(direction_pin, HIGH);  
  }
  if (steps_to_move < 0) {
    digitalWrite(direction_pin, LOW);
  }
  servo_move(servo_down_point);
  delay(1000);
  while (steps_left > 0)
  {
    digitalWrite(motor_pin, HIGH);
    delayMicroseconds(3);
    digitalWrite(motor_pin, LOW);
    delay(speedStep);
    steps_left--;
  }
  digitalWrite(enable_pin, HIGH);
  delay(1000);
  servo_move(servo_up_point);
}





