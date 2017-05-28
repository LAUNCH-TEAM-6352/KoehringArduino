#include <DFRobot.h>
#include <IIC1.h>

DFrobotEdison Motor;

int ledValue = LOW;
bool runMotor = true;
bool debug = true;

void setup() {
  if (runMotor)
  {
    Motor.begin(M1); /*Initializes the motor drive*/
    //Motor.stop();
  }
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if (debug)
  {
    Motor.debugOn();
  }
  else
  {
    Motor.debugOff();
  }
  
  if (runMotor)
  {
    //Motor.setDirection(CLOCKWISE); /*Motor clockwise rotation*/
    Motor.setSpeed(255 / 3);
  }
  blink(5, 1);

  if (runMotor)
  {
    //Motor.stop();
    //Motor.setDirection(CLOCKWISE); /*Motor clockwise rotation*/
    Motor.setSpeed(255 / 3 * 2);
  }
  blink(5, 2);

  if (runMotor)
  {
    //Motor.stop();
    //Motor.setDirection(ANTICLOCKWISE); /*Motor anticlockwise rotation*/
    Motor.setSpeed(255);
  } 
  blink(5, 3);

  if (runMotor)
  {
    Motor.stop(); /*Stop*/
  }
  blink(5, 0);
}

// Blinks the led for the specified number of
// seconds at the specified rate of blinks/sec
void blink(int duration, int bps)
{
  if (bps < 1)
  {
    digitalWrite(LED_BUILTIN, LOW);
    delay(duration * 1000);
  }
  else
  {
    int delayTime = 500 / bps;
    for (int i = 0; i < duration * bps; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(delayTime);
      digitalWrite(LED_BUILTIN, LOW);
      delay(delayTime);      
    }
  }
}


