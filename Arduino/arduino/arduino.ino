/*
 
# = 35
= = 61
 
x = 120
y = 121
z = 122
 
b = 98
 
 */
#include <SoftwareSerial.h>

  
SoftwareSerial mySerial(10, 11); // RX, TX
 
int red = 5;
int green = 6;
int blue = 3;
int command;

 
void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
 
mySerial.begin(57600);
Serial.println("test");
}

void controlR(int value)
{
  value+=1;
  analogWrite(red, value*4);
}

void controlG(int value)
{
  value-=63;
  analogWrite(green, value*4);
}

void controlB(int value)
{
  value-=127;
  analogWrite(blue, value*4);
}

void specialCommand(int value)
{

  switch(value)
  {
  case 192:
  switchOff();
  break;
  case 193:
  switchOn();
  break;
  case 194:
  stroboscopeOn();
  break;
  case 195:
  stroboscopeOff();
  default:
  break; 
  }
}
void switchOff(){}
void switchOn(){}
void stroboscopeOn(){}
void stroboscopeOff(){}

 
void loop() // run over and over
{
  if(mySerial.available())
  {
    command = mySerial.read();

    if(command<=63)
      controlR(command);
    else if(command <=127)
      controlG(command);
     else if(command<=191)
      controlB(command);
     else
      specialCommand(command);
      
    }

  
}
