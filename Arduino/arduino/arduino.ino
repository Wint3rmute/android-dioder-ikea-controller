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

char input[2];
 
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

int convertTo1024(char value)
{
  //Serial.println((int)((float)value*10.24));
  //return gammaCorrect((int)((float)value*2.55));
  return (int)((float)value*2.55);
  }

 
void loop() // run over and over
{

  if(mySerial.available()>=2)
  {
    mySerial.readBytes(input, 2);
     
    Serial.print((char)input[0]);
    Serial.print(" ");
    Serial.println((int)input[1]);
     

    switch(input[0])
    {
      case 'r':
        analogWrite(red, 1023-convertTo1024(input[1]));
        break;
      case 'g':
        analogWrite(green, 1023-convertTo1024(input[1]));
        break;
      case 'b':
analogWrite(blue, 1023-convertTo1024(input[1]));
      
        break;
      }
  }
  
}
