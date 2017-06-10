#include <DFRobot.h>
#include <IIC1.h>

DFrobotEdison Motor;

int ledValue = LOW;
bool runMotor = true;
bool sendDebug = false;
bool debug = true;
unsigned long lastSpeedChange = 0;
unsigned long lastLedChange = 0;
unsigned long lastDebugChange = 0;

long speeds[] = { 0, 64, 128, 192, 255 };
int numSpeeds = 5;
int speedIndex = 0;
unsigned long ledPeriods[] = { 1000000, 500, 250, 125, 1000000 };
unsigned long debugPeriod = 5000;
unsigned long speedPeriod = 5000;

void setup()
{
  Serial.begin(115200);
  speedIndex = numSpeeds - 1;
  if (runMotor)
  {
    Motor.begin(M1); /*Initializes the motor drive*/
  }
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  // Get current clock time:
  unsigned int curMillis = millis();

  //Serial.println(curMillis);

  // See if time to update debug flag:
  if (lastDebugChange == 0 || (curMillis - lastDebugChange) >= debugPeriod)
  {
    if (sendDebug)
    {
      debug ? Motor.debugOn() : Motor.debugOff();
      delay(20);
      Serial.print("speeds=");
      for (int i = 0; i < numSpeeds; i++)
      {
        if (i > 0)
        {
          Serial.print(", ");
        }
        Serial.print(speeds[i]);
      }
      Serial.println();
    }
    lastDebugChange = curMillis;
  }
  
  // See if time to advance to next speed:
  if (lastSpeedChange == 0 || (curMillis - lastSpeedChange) >= speedPeriod)
  {
    // Advance speed index, wrapping back to 0 if necessary:
    speedIndex = speedIndex >= numSpeeds-1 ? 0 : speedIndex + 1;
    Serial.print("curMillis: ");
    Serial.print(curMillis);
    Serial.print(", speedIndex: ");
    Serial.println(speedIndex);
    
    if (runMotor)
    {
      // Always set motor speed:
      Motor.setSpeed(speeds[speedIndex]);
      delay(20);
    }

    // Turn LED on or off as appropriate:
    ledValue = speedIndex == 0 ? LOW : HIGH;
    digitalWrite(LED_BUILTIN, ledValue);

    // Remember the last time things were changed:
    lastLedChange = curMillis;
    lastSpeedChange = curMillis;
  }
  else if ((curMillis - lastLedChange) >= ledPeriods[speedIndex])
  {
    // It is time to change the LED value:
    ledValue = ledValue == LOW ? HIGH : LOW;
    digitalWrite(LED_BUILTIN, ledValue);
    lastLedChange = curMillis;
  }

  if (runMotor)
  {
    // Always set motor speed:
    Motor.setSpeed(speeds[speedIndex]);
    delay(20);
  }
}


