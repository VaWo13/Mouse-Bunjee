#include <Arduino.h>

#define LED_RED 9
#define LED_GREEN 10
#define LED_BLUE 11
#define cooldown_refresh 400


unsigned int RGB_rotation_time = ((6 * 1000) / 360);    //rotation seconds * 1000 miliseconds / 360 degrees
float RGB_Brightness = 1;


bool RGBRotate = true;
int enable_lights = 1;
int angle = 0;  //angle from 0-360
int cooldown = cooldown_refresh;
unsigned long counter = 0;

int color_R = 0;
int color_G = 0;
int color_B = 0;


void pulse();
void RGB();

void setup()
{
  // put your setup code here, to run once:

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  Serial.begin(115200);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    String settings = Serial.readString();
    switch (settings[0])
    {
      case 's':
        RGB_rotation_time = ((settings.substring(2).toFloat() * 1000) / 360);    //rotation seconds * 1000 miliseconds / 360 degrees
        RGBRotate = true;

        Serial.println("Values can be floating point numbers also 0.1 geht auch... kannste aber auch 100 machn");
        break;
      case 'b':
        RGB_Brightness = settings.substring(2).toFloat() / 100; 

        Serial.println("Values range from 0 to 100");
        break;
      case 'c':
        color_R = settings.substring(2).toInt();
        color_G = settings.substring(6).toInt();
        color_B = settings.substring(10).toInt();
        RGBRotate = false;
        
        Serial.println("3 Values range from 0 to 255  numbers smaller than 100 must have '0' added in front -> 069 007");
        break;
      
      default:
        break;
    }
  }

  if ((millis() - counter) >= RGB_rotation_time)
  {
    counter = millis();
    angle ++;
    cooldown --;


    if (angle > 360)
    {
      angle = 1;
      Serial.println("alla");   //used to keep the USB port from ignoring the device
      
      attachInterrupt(digitalPinToInterrupt(2), pulse, RISING);
      if (cooldown <= 0)
      {
        enable_lights = 0;
      }
    }


    if (RGBRotate)
    {
      RGB();
    }
    else
    {
      analogWrite(LED_RED, color_R * enable_lights);  
      analogWrite(LED_GREEN, color_G * enable_lights); 
      analogWrite(LED_BLUE, color_B * enable_lights);
    }
  }
}

void pulse()
{
  detachInterrupt(digitalPinToInterrupt(2));
  cooldown = cooldown_refresh;
  enable_lights = 1;
}

void RGB()
{
  analogWrite(LED_RED, map((sin(angle * PI / 180) + 1) * 100, 0, 200, 0, 255 * RGB_Brightness * enable_lights));
  analogWrite(LED_GREEN, map((sin((angle + 120) * PI / 180) + 1) * 100, 0, 200, 0, 255 * RGB_Brightness * enable_lights));
  analogWrite(LED_BLUE, map((sin((angle + 120 * 2) * PI / 180) + 1) * 100, 0, 200, 0, 255 * RGB_Brightness * enable_lights));
}
