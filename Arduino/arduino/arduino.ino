#include <SoftwareSerial.h>

  
SoftwareSerial mySerial(10, 11); // RX, TX
 
int red = 5;
int green = 6;
int blue = 3;
int command;

void led(int color, int value)
{

  analogWrite(color, gammaCorrection(value));
  
  }

int gammaCorrection(int input)
{
  //return input;
  float fInput = input;
  Serial.println(fInput/255.0  );
  //Serial.println(pow((float)input/(float)255, 2.2));
  return int(255 * pow(fInput/255.0, 2.2));
}

void shortBlink(int r, int g, int b, int interval)
{
  switchOff();
  led(red, r);
  led(green, g);
  led(blue, b);
  
  delay(interval);
  switchOff();
  delay(interval);
}

 
void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
 
mySerial.begin(57600);
//Serial.println("test");

switchOff();
delay(2000);

shortBlink(255,0,0,500);
shortBlink(0,255,0,500);
shortBlink(0,0,255,500);

analogWrite(red, 255);
analogWrite(green, 255);
analogWrite(blue, 255);
delay(700);



for(int i = 255; i >= 0; i--)
{
  
  led(red,i);
  led(green,i);
  led(blue,i);
  delay(4);
  
  }


}

void controlR(int value)
{
  //value+=1;
  led(red, value*4);
}

void controlG(int value)
{
  value-=64;
  led(green, value*4);
}

void controlB(int value)
{
  value-=128;
  led(blue, value*4);
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
void switchOff()
{
  analogWrite(red,0);
  analogWrite(green,0);
  analogWrite(blue,0);
  
  }
void switchOn(){}
void stroboscopeOn(){}
void stroboscopeOff(){}

 
void loop() // run over and over
{
  if(mySerial.available())
  {
    command = mySerial.read();
    //Serial.write(command);

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
