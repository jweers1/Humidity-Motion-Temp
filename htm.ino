// This #include statement was automatically added by the Particle IDE. 
#include "Adafruit_DHT/Adafruit_DHT.h" 
 
// DHT parameters 
#define DHTPIN 3 
#define DHTTYPE DHT11 
#define PIRPIN A0

int tinkerDigitalRead(String pin);
int tinkerDigitalWrite(String command);
int tinkerAnalogRead(String pin);
int tinkerAnalogWrite(String command);

// Variables 
int temperature=50; 
int humidity=50; 
int PirState=0;
int motion=0;

unsigned long currmillis;
unsigned long prevmillis;
unsigned long pirmillis;

// DHT sensor 
DHT dht(DHTPIN, DHTTYPE); 
 
void setup() { 
    
    	//Register all the Tinker functions
	Particle.function("digitalread", tinkerDigitalRead);
	Particle.function("digitalwrite", tinkerDigitalWrite);
	Particle.function("analogread", tinkerAnalogRead);
	Particle.function("analogwrite", tinkerAnalogWrite);
	
    dht.begin(); 
    Serial.begin(9600);
    pinMode(PIRPIN,INPUT);
    
    Particle.variable("Temperature",temperature);
    Particle.variable("Humidity",humidity);
    Particle.variable("Motion",motion);
} 
 
void loop() { 
     
    // Humidity measurement 

    
    currmillis=millis();
    
    if((currmillis-pirmillis)>100){
      PirState=analogRead(PIRPIN); 
    //  Serial.print("PirState: ");
    //  Serial.println(PirState);
    
      if (PirState>motion){
        motion=PirState;
      }
      pirmillis=currmillis;
    }
    //if (PirState==HIGH){
    //    motion=HIGH;
    //}
    
    if ((currmillis-prevmillis)>65000){
    
        temperature = dht.getTempFarenheit(); 
        Serial.print("Temp: ");
        Serial.println(temperature);
        // Humidity measurement 
        humidity = dht.getHumidity();
         Serial.print("Humidity: ");
         Serial.println(humidity);
         Serial.print("Motion");
         Serial.println(motion);
    
     
        // Publish data 
    //    Particle.publish("temperature", String(temperature) + " °F"); 
        
    //    Particle.publish("humidity", String(humidity) + "%"); 
        
    //    Particle.publish("motion",motion);
        
        motion=0;    
        prevmillis=currmillis;
    } 
} 

/*******************************************************************************
 * Function Name  : tinkerDigitalRead
 * Description    : Reads the digital value of a given pin
 * Input          : Pin
 * Output         : None.
 * Return         : Value of the pin (0 or 1) in INT type
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerDigitalRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		pinMode(pinNumber, INPUT_PULLDOWN);
		return digitalRead(pinNumber);
	}
	else if (pin.startsWith("A"))
	{
		pinMode(pinNumber+10, INPUT_PULLDOWN);
		return digitalRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
 * Function Name  : tinkerDigitalWrite
 * Description    : Sets the specified pin HIGH or LOW
 * Input          : Pin and value
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerDigitalWrite(String command)
{
	bool value = 0;
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(command.substring(3,7) == "HIGH") value = 1;
	else if(command.substring(3,6) == "LOW") value = 0;
	else return -2;

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		digitalWrite(pinNumber, value);
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		digitalWrite(pinNumber+10, value);
		return 1;
	}
	else return -3;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogRead
 * Description    : Reads the analog value of a pin
 * Input          : Pin
 * Output         : None.
 * Return         : Returns the analog value in INT type (0 to 4095)
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerAnalogRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		return -3;
	}
	else if (pin.startsWith("A"))
	{
		return analogRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogWrite
 * Description    : Writes an analog value (PWM) to the specified pin
 * Input          : Pin and Value (0 to 255)
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerAnalogWrite(String command)
{
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	String value = command.substring(3);

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		analogWrite(pinNumber, value.toInt());
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		analogWrite(pinNumber+10, value.toInt());
		return 1;
	}
	else return -2;
}
