#include <Servo.h> 
#include <SPI.h>
#include "Ucglib.h"           
#include <QMC5883LCompass.h>

#define  trigPin   6       
#define  echoPin   5        
#define  ServoPin  3         
int Ymax = 240;              
int Xmax = 320;              

int Xcent = Xmax / 2;       
int base = 210;              
int scanline = 185;         

Servo baseServo; 
//Ucglib_ILI9341_18x240x320_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
Ucglib_ILI9341_18x240x320_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);

QMC5883LCompass compass;

void setup(void)
{
 
      ucg.begin(UCG_FONT_MODE_SOLID); 
      ucg.setRotate90();             
      
      pinMode(trigPin, OUTPUT);      
      pinMode(echoPin, INPUT);       
      Serial.begin(9600);            
      baseServo.attach(ServoPin);   
    
      
      ucg.setFontMode(UCG_FONT_MODE_TRANSPARENT);
      ucg.setColor(0, 0, 100, 0);
      ucg.setColor(1, 0, 100, 0);
      ucg.setColor(2, 20, 20,20);
      ucg.setColor(3, 20, 20, 20);
      ucg.drawGradientBox(0, 0, 320, 240);
      ucg.setPrintDir(0);
      ucg.setColor(0, 5, 0);
      ucg.setPrintPos(70,120);
      ucg.setFont(ucg_font_logisoso32_tf);  
      ucg.print("Group 06");
      ucg.setColor(0, 255, 0);
      ucg.setPrintPos(70,120);
      ucg.print("Group 06");
      ucg.setFont(ucg_font_courB14_tf);
      ucg.setColor(20, 255, 20);
      ucg.setPrintPos(90,200);
      ucg.print("Testing...");
      baseServo.write(90);
    
    
      for(int x=0;x<180;x+=5)
          { baseServo.write(x);
            delay(50);
           }
      ucg.print("OK!");
      delay(500);
      ucg.setColor(0,0, 0, 0);
      ucg.setColor(1,0, 0, 0);
      ucg.setColor(2,0, 0, 0);
      ucg.setColor(3,0, 0, 0);
      ucg.drawGradientBox(0, 0, 320, 240);
      delay(10);
    
    
      //ucg.clearScreen();
      cls();
      ucg.setFontMode(UCG_FONT_MODE_SOLID);
      ucg.setFont(ucg_font_helvR08_hr);   // or freedoomr10_tr

      compass.init();
      compass.setCalibration(-117, 73, -105, 42, -30, 85);
  
}


void cls()
{
 
  ucg.setColor(0, 0, 0, 0);
  for(int s=0;s<240;s++)
  {
    ucg.drawHLine(0,s,320);
    delay(1);
  }
    
  //ucg.drawBox(0, 0, 160, 60);

}


int calculateDistance()
{ 
      long duration;
    
      digitalWrite(trigPin, LOW); 
      delayMicroseconds(2);
      
      digitalWrite(trigPin, HIGH); 
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      
      duration = pulseIn(echoPin, HIGH);

      Serial.println(duration*0.034/2);
      return duration*0.034/2;
}

void fix_font() 
{
      ucg.setColor(0, 180, 0);
      ucg.setPrintPos(144,44);
      ucg.print("1.00");
      ucg.setPrintPos(144,100);
      ucg.print("0.60");
      ucg.setPrintPos(144,165);
      ucg.print("0.30");
}

void getDirection(int angle, char* direction) {
    if (angle > 160 && angle <= 180) {
        strcpy(direction, "E");
    } else if (angle > 140 && angle <= 160) {
        strcpy(direction, "ESE");
    } else if (angle > 120 && angle <= 140) {
        strcpy(direction, "SE");
    } else if (angle > 100 && angle <= 120) {
        strcpy(direction, "SSE");
    } else if (angle > 80 && angle <= 100) {
        strcpy(direction, "S");
    } else if (angle > 60 && angle <= 80) {
        strcpy(direction, "SSW");
    } else if (angle > 40 && angle <= 60) {
        strcpy(direction, "SW");
    } else if (angle > 20 && angle <= 40) {
        strcpy(direction, "WSW");
    } else if (angle >= 0 && angle <= 20) {
        strcpy(direction, "W");
    } else {
        strcpy(direction, "Undefined");
    }
}


void fix()
{

      ucg.setColor(0, 180, 0);
    
      ucg.drawDisc(Xcent, base+1, 3, UCG_DRAW_ALL); 
      ucg.drawCircle(Xcent, base+1, 210, UCG_DRAW_UPPER_LEFT);
      ucg.drawCircle(Xcent, base+1, 210, UCG_DRAW_UPPER_RIGHT);
      ucg.drawCircle(Xcent, base+1, 135, UCG_DRAW_UPPER_LEFT);
      ucg.drawCircle(Xcent, base+1, 135, UCG_DRAW_UPPER_RIGHT);
      ucg.drawCircle(Xcent, base+1, 70, UCG_DRAW_UPPER_LEFT);
      ucg.drawCircle(Xcent, base+1, 70, UCG_DRAW_UPPER_RIGHT);
      ucg.drawLine(0, base+1, Xmax,base+1);
     
      ucg.setColor(0, 180, 0);
     
       for(int i= 40;i < 300; i+=2)
       {

        if (i % 10 == 0) 
          ucg.drawLine(185*cos(radians(i))+Xcent,base - 185*sin(radians(i)) , 205*cos(radians(i))+Xcent,base - 205*sin(radians(i)));
        
        else
        
         ucg.drawLine(195*cos(radians(i))+Xcent,base - 195*sin(radians(i)) , 205*cos(radians(i))+Xcent,base - 205*sin(radians(i)));
         
       }
          
     
       ucg.setColor(0,200,0);
       ucg.drawLine(0,0,0,36);
       for(int i= 0;i < 5; i++)
       {
          ucg.setColor(0,random(200)+50,0);
          ucg.drawBox(2,i*8,random(28)+3,6);
       }

       ucg.setColor(0,180,0);
       ucg.drawFrame(292,0,28,28);
       ucg.setColor(0,60,0);
       ucg.drawHLine(296,0,20);
       ucg.drawVLine(292,4,20);
       ucg.drawHLine(296,52,20);
       ucg.drawVLine(318,4,20);
        
       ucg.setColor(0,220,0);
       ucg.drawBox(296,4,8,8);
       ucg.drawBox(296,16,8,8);
       ucg.drawBox(308,16,8,8);
       ucg.setColor(0,100,0);
       ucg.drawBox(308,4,8,8);

       ucg.setColor(0,90,0);
       ucg.drawTetragon(124,220,116,230,196,230,204,220);
       ucg.setColor(0,160,0);
       ucg.drawTetragon(134,220,126,230,186,230,194,220);
       ucg.setColor(0,210,0);
       ucg.drawTetragon(144,220,136,230,176,230,184,220);
}



void loop(void)
{
  
  int distance;

  fix(); 
  fix_font(); 

  for (int x=180; x > 4; x-=2){      
     
      baseServo.write(x);             
      
     
      int f = x - 4; 
      ucg.setColor(0, 255, 0);
      ucg.drawLine(Xcent, base, scanline*cos(radians(f))+Xcent,base - scanline*sin(radians(f)));
      f+=2;
      ucg.setColor(0, 128, 0);
      ucg.drawLine(Xcent, base, scanline*cos(radians(f))+Xcent,base - scanline*sin(radians(f)));
      f+=2;
      ucg.setColor(0, 0, 0);
      ucg.drawLine(Xcent, base, scanline*cos(radians(f))+Xcent,base - scanline*sin(radians(f)));
      ucg.setColor(0,200, 0);
     
      distance = calculateDistance();

      compass.read();
      int x_value = compass.getX();
      int y_value = compass.getY();
      int z_value = compass.getZ();

      char direction[20];
      getDirection(x, direction);
     
      if (distance < 100)
      {
        ucg.setColor(255,0,0);
        ucg.drawDisc(2.2*distance*cos(radians(x))+ Xcent,-2.2*distance*sin(radians(x))+base, 1, UCG_DRAW_ALL);
      }
      else
      { 
        ucg.setColor(255,255,0);
        ucg.drawDisc(208*cos(radians(x))+Xcent,-208*sin(radians(x))+base, 1, UCG_DRAW_ALL);
      }
    
           
     
      // Serial.print(x); 
      // Serial.print("    ,   ");
      // Serial.println(distance); 
     

      if (x > 70 and x < 110)  fix_font(); 


      ucg.setColor(255,255,  0);
      ucg.setPrintPos(20,230);
      ucg.print("Direction: "); 
      ucg.setPrintPos(70,230);
      //ucg.print(x);
      ucg.print(direction);
      ucg.setPrintPos(210,230);
      ucg.print("Angle: "); 
      ucg.setPrintPos(240,230);
      ucg.print(x);
      ucg.print(" ");
      ucg.print((char)0xB0);
      // ucg.print(direction[2]);
      ucg.print("  ");
      ucg.setPrintPos(270,230);
      ucg.print("     ");
      ucg.print(distance);
      ucg.print(" cm    "); 
      
  }
 
 delay(50);
 cls();

}