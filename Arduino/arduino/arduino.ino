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
  fadeToBlack();
    for(int i = 0; i < 256; i++)
    {
       rV=i;
       led(red,rV); 
       delay(interval);
    }

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
  switchOn();
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

void smg()
{
  
  switchOff();
  delay(10);
  for(int i = 0; i < 5; i++)
  {
  led(red, 255);
  led(green, 130);
  led(blue, 70);
  delay(65);
  switchOff();
  delay(65);
  }
  switchOn();
}

void police()
{
  switchOff();
  switchOn();


  rV = 0;
  bV = 255;
  bool blueToRed =  true;
  int exitCommand = 0;
  mySerial.flush();
  while(true)
  {
    exitCommand = mySerial.read();
    switch(exitCommand)
    {
      case 199:
      fadeToBlack();
      return;

      case 193:
      switchOff();
      switchOn();
      return;
      
      
      default:
      break;
      }

    if(blueToRed){
    led(red, rV++);
    led(blue, bV--);  

    if(bV==0)
    blueToRed = false;
    
    delay(3);
    
    }else
    {
    led(red, rV--);
    led(blue, bV++);  

    if(rV==0)
    blueToRed = true;
    
    delay(3);
    }
  }
}

void stun()
{
  led(red, 255);
  led(green,200);
  led(blue,100);
  delay(500);

   for(float i = 0; i < 18.45; i+=0.01)
   {
    led(red, sin(i)*255);
    led(green, sin(i)*255);
    led(blue, sin(i)*255);
    delay(2);
    }
  
  switchOn();
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
    colorPhase(9);
  break;

  case 202: //
    colorPhase(13);
  break;

  case 203:
    colorPhase(1);
  break;

  case 204:
    smg();
  break;
  
  case 205:
    stun();
  break;
  
  case 206:
    police();
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

