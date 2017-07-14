#include <SoftwareSerial.h>
#include <elapsedMillis.h>

  
SoftwareSerial mySerial(10, 11); // RX, TX
 
int red = 5;
int green = 6;
int blue = 3;
byte command;
bool isOn = true;

elapsedMillis timeElapsed;
bool stroboscopeActive = false;
bool stroboscopeLedsActive = false;
int stroboscopeSpeed = 300;

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

switchOn();

}

void controlR(int value)
{
  rV = value*4;
}

void controlG(int value)
{
  value-=64;
  gV = value*4;
}

void controlB(int value)
{
  value-=128;
  bV = value*4;
}


void switchOn(){isOn = true;}

void switchOff()
{
  analogWrite(red,0);
  analogWrite(green,0);
  analogWrite(blue,0);
  //delay(5);
  mySerial.flush();
  isOn = false;
  
  }

void stroboscopeOnSlow()
{
  stroboscopeSpeed = 400;
  stroboscopeActive = true;
}

void stroboscopeOnMedium()
{
  stroboscopeSpeed = 220;
  stroboscopeActive = true;
}

void stroboscopeOnEpilepsy()
{
  stroboscopeSpeed = 100;
  stroboscopeActive = true;
}

void colorPhase(int interval)
{
  while(true)
  {
      
      for(int i = 0; i < 256; i++)
    {
       if(mySerial.read() == 201)
      {fadeToBlack(); {fadeToBlack(); return;}}
     
     gV=i;
     led(green,gV); 
     delay(interval);
      }

      

      for(int i = 255; i >= 0; i--) //red?
    {
       if(mySerial.read() == 201)
      {fadeToBlack(); {fadeToBlack(); return;}}
     
     rV=i;
     led(red,rV); 
     delay(interval);
      }

    
   
      
    for(int i = 0; i < 256; i++)
    {
        if(mySerial.read() == 201)
      {fadeToBlack(); return;}
      
     bV=i;
     led(blue,bV); 
     delay(interval);
      }

      

      
    for(int i = 255; i >= 0; i--)
    {
       if(mySerial.read() == 201)
      {fadeToBlack(); return;}
     
     gV=i;
     led(green,gV); 
     delay(interval);
      }

     
    
    for(int i = 0; i < 256; i++)
    {

       if(mySerial.read() == 201)
      {fadeToBlack(); return;}
  
     rV=i;
     led(red,rV); 
     delay(interval);
      }

  

    for(int i = 255; i >= 0; i--)
    {

          if(mySerial.read() == 201)
      {fadeToBlack(); return;}
  
     bV=i;
     led(blue,bV); 
     delay(interval);
    }
   
  }
}

  
void stroboscopeOff()
{
  stroboscopeActive = false; 
  led(red,rV);
  led(green,gV);
  led(blue,bV);
}

void fadeToBlack()
{
  while(rV>0 || gV>0 || bV>0)
  {
    
  delay(7);

    if(rV!=0)
      rV--;

    if(gV!=0)
    gV--;


    if(bV!=0)
    bV--;

    
  led(red, rV);
  led(green, gV);
  led(blue, bV);

      
  }


}

 

void specialCommand(int value)
{
  Serial.print("SPECIAL ");
  Serial.println(value);

  switch(value)
  {
  case 193:
  stroboscopeOff();
  switchOff();
  break;
  
  case 194:
  stroboscopeOnSlow();
  break;
  
  case 195:
  stroboscopeOff();
  break;
  
  case 196:
  stroboscopeOnMedium();
  break;
  
  case 197:
  stroboscopeOnEpilepsy();
  break;

  case 198:
  switchOn();
  break;

  case 199:
  fadeToBlack();
  break;

  case 200: //and 201 for colorPhase -> stop
  fadeToBlack();
    for(int i = 0; i < 256; i++)
    {
       rV=i;
       led(red,rV); 
       delay(10);
    }

    colorPhase(10);
  break;
  
  default:
  break; 
  }
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
       if(timeElapsed > stroboscopeSpeed)
       {
          timeElapsed = 0;
          if(stroboscopeLedsActive)
          {
            //Serial.println("off");
            switchOff();
            stroboscopeLedsActive = false;
          }
          else
          {
            //Serial.println("on");
            led(red, rV);
            led(green, gV);
            led(blue, bV);
            stroboscopeLedsActive = true;
          }
       }
    }else if(isOn)
    {
      led(green, gV);
      led(blue, bV);
      led(red, rV);
  }
}

