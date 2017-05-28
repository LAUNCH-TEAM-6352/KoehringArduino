/******************************************************************
                           cmd     arg
Ax Analog input port       0xAx    0 do not get voltage value , 1 get voltage value
Dx Digital input port      0xDx    1 high   0 low
Moter1 Direction           0xB1    1 anticlockwise 0 clockwise
Moter2 Direction           0xB2    1 anticlockwise 0 clockwise
Moter1 Speed               0xC1    speed value
Moter2 Speed               0xC2    speed value 
servo control              0xEx    duty

M2
D9      ENA
D7      DIRA       

M1
D10     ENB
D8      DIRB

Version: V1.3
******************************************************************/

#include <Wire.h>
#include <Servo.h> 

Servo myservo;
unsigned char cmd=0,arg=0;
int ADC_value=0;
unsigned char Check_State=0;
int DBG = 0;

void setup()
{
  Wire.begin(4);                         //set slave address  4
  Wire.onReceive(I2C_slave_receive_Handler);//Specify the function when i2c slave receives commands 
  Wire.onRequest(I2C_slave_send_Handler);   //Specify the function when i2c slave send data to host
}

void loop()
{
}

// When receiving data from the bus, execute the function
void I2C_slave_receive_Handler(int None)
{
	int ok=0;

  /* 55 aa cmd arg cs*/
	unsigned char header1,header2,cmd,arg,cs;
 
	do{
    waitOnWire();
		header1 = Wire.read(); //receives the last character
    if (DBG)
    {
      Serial.print("millis=");
      Serial.print(millis());
      Serial.print(", header1=");
      Serial.print(header1,HEX);
    }
		if(header1 != 0x55)
			break;

    waitOnWire();
		header2 = Wire.read();
    if (DBG)
    {
      Serial.print(", header2=");
      Serial.print(header2,HEX);
    }
		if(header2 != 0xaa)
			break;

    waitOnWire();
		cmd = Wire.read();
    if (DBG)
    {
      Serial.print(", cmd=");
      Serial.print(cmd,HEX);
    }

    waitOnWire();
		arg = Wire.read();
    if (DBG)
    {
      Serial.print(", arg=");
      Serial.print(arg,HEX);
    }

    waitOnWire();
		cs = Wire.read();
    if (DBG)
    {
      Serial.print(", cs=");
      Serial.print(cs,HEX);
    }
		if(cs == (unsigned char)(header1+header2+cmd+arg)){
			ok = 1;
		}
    if (DBG)
    {
      Serial.print(", ok=");
      Serial.print(ok,HEX);
    }
	} while(0);

  if (DBG)
  {
    Serial.println();
  }

	if(!ok){
		return;
	}
 
  exec(cmd,arg);
}

void waitOnWire()
{
    int count = 0;
    
    // The loop is executed, until the packets left with only one character
    while( Wire.available() == 0)
    {
      if(++count == 1000) break;
      delay(1);
    }
}

void exec(unsigned char cmd, unsigned char arg)
{
	unsigned char x = cmd &0x0F;
	switch(cmd&0xF0){
    // Enable/disable debug:
    case 0x10:
      if (x == 0 && DBG)
      {
        Serial.end();
      }
      else if (x != 0 && !DBG)
      {
        Serial.begin(115200);
      }
      DBG = x;
      break;
    
		case 0xA0:
			if(x > 3) x+=2;
			if(x < 8)
				pinMode(x+14, INPUT);//Set the Ax for analog input port
      if(arg)
        ADC_value=analogRead(x+14);
		  break;
    
		case 0xD0:
			if(x > 3) x+=2;
			if(x < 8){
				pinMode(x+14, OUTPUT);//Set the Ax for digital output port
				digitalWrite(x+14,  arg);
			}
		  break;
    
    case 0xB0: //moter direction
			if(x == 1){//Setting M1 motor direction of rotation
				pinMode(8, OUTPUT);
				if(arg == 0)//Setting M1 motor clockwise rotation
					digitalWrite(8,  LOW);
				else		  //Setting M1 motor anticlockwise rotation
					digitalWrite(8,  HIGH);
			}else if(x == 2){
				pinMode(7, OUTPUT);
				if(arg == 0)//Setting M2 motor clockwise rotation
					digitalWrite(7,  LOW);
				else		  //Setting M2 motor anticlockwise rotation
					digitalWrite(7,  HIGH);				
			}
		  break;
    
		case 0xC0: //set moter speed
			if(x == 1) //M1
				analogWrite(10, arg);
			else //M2
				analogWrite(9,  arg);
		  break;
    
		case 0xE0: //servo control
			if(x < 4){
				myservo.attach(x+14);
				myservo.write(map(arg, 0, 255, 0, 180));
			}
		  break;
      
		default:
		  break;
		}
}

//When the host control slave uploads the data, execute the function
void I2C_slave_send_Handler()
{
        unsigned char buf[5];
        buf[0]=0x55;buf[1]=0xaa;
        buf[2]=ADC_value&0xff;buf[3]=ADC_value>>8;
        buf[4]=buf[0]+buf[1]+buf[2]+buf[3];
		Wire.write((const uint8_t *)buf,5); //Sending expected data to i2c master
        //Serial.print("ADC_value=");Serial.println(ADC_value,HEX);
}
