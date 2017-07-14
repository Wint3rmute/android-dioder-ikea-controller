#include <SoftwareSerial.h>

  
SoftwareSerial mySerial(10, 11); // RX, TX
 
int red = 5;
int green = 6;
int blue = 3;
byte command;
bool stroboscopeActive = false;
bool stroboscopeLedsActive = false;
int lastBlinkTime;

int rV, gV, bV;

void led(int color, int value)
{

  analogWrite(color, gammaCorrection(value));
  
}

int gammaCorrection(int input)
{
  //return input;
  float fInput = input;
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
    Serial.println("test");
    lastBlinkTime = millis();


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
  rV = value*4;
  led(red, rV);
}

void controlG(int value)
{
  value-=64;
  gV = value*4;
  led(green, gV);
}

void controlB(int value)
{
  value-=128;
  bV = value*4;
  led(blue, bV);
}


void switchOn(){}
void stroboscopeOn(){stroboscopeActive = true;}
void stroboscopeOff(){stroboscopeActive = false;}

 

void specialCommand(int value)
{
  Serial.print("SPECIAL ");
  Serial.println(value);

  switch(value)
  {
  case 193:
  switchOff();
  break;
  case 194:
  stroboscopeOn();
  break;
  case 195:
  stroboscopeOff();
  break;
  case 200:
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
  delay(10);
  mySerial.flush();
  
  
  }

void loop() // run over and over
{
  if(mySerial.available())
  {
    command = mySerial.read();
    Serial.println(command);

    if(command<=63)
      controlR(command);
    else if(command <=127)
      controlG(command);
     else if(command<=191)
      controlB(command);
     else
      specialCommand(command);
      
    }

    if(stroboscopeActive)
    {
       if(millis()-lastBlinkTime>300)
       {
        lastBlinkTime=millis();
        if(stroboscopeLedsActive)
        {
        Serial.println("off");
          switchOff();
          stroboscopeLedsActive = false;
          }
          else
          {
            Serial.println("on");
            led(red, rV);
            led(green, gV);
            led(blue, bV);
            stroboscopeLedsActive = true;
        
            
            }
        
        }
        

    }


  
}

