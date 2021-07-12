//Main Program for Patient Bed Detection
//This project was designed to check the patient bed position of the Amaris and VisuMax laser

#include <EEPROM.h>

int eeprom_address_RL = 0; // Eeprom memory position for RL calibration value
int eeprom_address_OP = 1; // Eeprom memory position for OP calibration value

int relay_RL = 13; // relay RL connected to pin 13
int relay_OP = 12; // relay OP connected to pin 12


int led_R = 11; // LED right connected to pin 11
int led_L = 10; // LED left connected to pin 10
int led_OP = 9; // LED OP connected to pin 9
int button_RL = 3; // programing button for RL calibration connected to pin 3
int button_OP = 2; // programing button for OP calibration connected to pin 2


int trigger_RL = 7; // pin trigger sensor RL connected to pin 7
int echo_RL = 6; // pin eco sensor RL connected to pin 6

int trigger_OP = 5; // 
int echo_OP = 4;


long time_RL;
long distance_RL;
long cal_distance_RL; // RL distance set as a toggle distance
long cal_time_RL;

long time_OP;
long distance_OP;
long cal_distance_OP; // OP distace set as a toggle distance 
long cal_time_OP;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(relay_RL, OUTPUT);
pinMode(relay_OP, OUTPUT);
pinMode(led_R, OUTPUT);
pinMode(led_L, OUTPUT);
pinMode(led_OP, OUTPUT);
pinMode(trigger_RL, OUTPUT);
pinMode(echo_RL, INPUT);
pinMode(trigger_OP, OUTPUT);
pinMode(echo_OP, INPUT);

pinMode(button_RL,INPUT); // set button RL as an Input
pinMode(button_OP,INPUT); // set button OP as an Input

digitalWrite(relay_OP, LOW);
digitalWrite(relay_RL, HIGH);
digitalWrite(led_R, HIGH);
digitalWrite(led_L, LOW);
digitalWrite(led_OP,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

////////////////////// check the buttonRL if pressed in order to calibrate /////////////////////

while (digitalRead(button_RL)== HIGH){
  calibrate_RL();
}

/////////////////////// check the button OP if pressed in order to calibrate ////////////////////

while (digitalRead(button_OP)== HIGH){
  calibrate_OP();
}



/////////////////////// Read the EEProm Address for RL and OP //////////////////////////////////////

cal_distance_RL = EEPROM.read(eeprom_address_RL); // read the eeprom and write the value of the calibration distance RL
cal_distance_OP = EEPROM.read(eeprom_address_OP); // read the eeprom and write the value of the calibration distance RL


read_RL(); // read the RL sensor subrutine
delay(1); // delay between readings
read_OP(); // read the OP sensor subrutine

/////////////////// print distance on serial monitor//////////////////

Serial.print("Distance RL: ");
Serial.println(distance_RL);
Serial.print("Distance OP: ");
Serial.println(distance_OP);
Serial.println(" ");
//Serial.print("Calibrate Distance RL: ");
//Serial.println(cal_distance_RL);
//Serial.print("Calibrate Distance OP: ");
//Serial.println(cal_distance_OP);
//Serial.println(" ");
delay(500);


//////////////////////////// PATIENT BED IN OP POSITION?? ///////////////////////////

if (distance_OP <= cal_distance_OP) /// Checking if the actual OP distance is less than Calibrate OP distance
{
  digitalWrite(relay_OP, HIGH);   // set ON relay OP -- > Patient bed in OP possition 
  digitalWrite(led_OP, HIGH); // set ON led OP --- > Patient bed in OP possition
    }

else
{
  digitalWrite(relay_OP, LOW);   // set OFF Relay OP --- > Patient bed is not in OP position
  digitalWrite(led_OP, LOW); // set OFF LED OP -- > Patient bed is not in OP position
  }


/////////////////////// PATIENT BED IS EN LEFT OR RIGHT EYE //////////////////////////

if (distance_RL <= cal_distance_RL) // Check if Patient bed is in LEFT EYE
{
  digitalWrite(relay_RL, HIGH);   // SET ON Relay RL -- > This means the patient bed is on LEFT EYE POSITION
  digitalWrite(led_R, LOW); // SET OFF Right Eye LED
  digitalWrite(led_L, HIGH); // SET ON Left Eye LED

}

else
{
  digitalWrite(relay_RL, LOW);   // SET OFF Relay RL -- > This means the patient bed is on RIGHT EYE POSITION
  digitalWrite(led_R, HIGH); // SET ON Right Eye LED 
  digitalWrite(led_L, LOW); // SET OFF Left Eye LED

  }
  
}
 


/////////////////////////////////////////////////////// READ RL SENSOR SUBRUTINE ////////////////////////////////


 void read_RL(){
  
digitalWrite(trigger_RL, LOW);
delayMicroseconds(2);
digitalWrite(trigger_RL,HIGH);
delayMicroseconds(10);
digitalWrite(trigger_RL,LOW);
time_RL = pulseIn(echo_RL,HIGH);
distance_RL = time_RL*0.034/2;

  
  }


 /////////////////////////////////////////////////////// READ OP SUBRUTINE ////////////////////////////////


 void read_OP(){

digitalWrite(trigger_RL, LOW);
delayMicroseconds(2);
digitalWrite(trigger_RL,HIGH);
delayMicroseconds(10);
digitalWrite(trigger_RL,LOW);
time_OP = pulseIn(echo_RL,HIGH);
distance_OP = time_OP*0.034/2;
  
  }
 
///////////////////////////////////////////////////////CALIBRATE RL SUBRUTINE ///////////////////////////////////////////////////////////////////////

void calibrate_RL(){


//set tigger RL and OP in LOW
digitalWrite(trigger_RL, LOW);
delayMicroseconds(2);


// trigger 10 microsecods pin trigger RL and OP, and read echo time

digitalWrite(trigger_RL,HIGH);
delayMicroseconds(10);
digitalWrite(trigger_RL,LOW);
cal_time_RL = pulseIn(echo_RL,HIGH);



// Calculate distance

cal_distance_RL = cal_time_RL*0.034/2;
EEPROM.write(eeprom_address_RL, cal_distance_RL);

Serial.print("Calibrate Distance RL: ");
Serial.println(cal_distance_RL);
Serial.println(" ");
delay(500);

digitalWrite(led_OP,LOW);
digitalWrite(led_R, HIGH);
digitalWrite(led_L, HIGH);
delay(100);
digitalWrite(led_R, LOW);
digitalWrite(led_L, LOW);
delay(100);

}


///////////////////////////////////////////////////////CALIBRATE OP SUBRUTINE ///////////////////////////////////////////////////////////////////////

void calibrate_OP(){


//set tigger RL and OP in LOW
digitalWrite(trigger_RL, LOW);
delayMicroseconds(2);


// trigger 10 microsecods pin trigger RL and OP, and read echo time

digitalWrite(trigger_RL,HIGH);
delayMicroseconds(10);
digitalWrite(trigger_RL,LOW);
cal_time_OP = pulseIn(echo_RL,HIGH);



// Calculate distance

cal_distance_OP = cal_time_OP*0.034/2;
EEPROM.write(eeprom_address_OP, cal_distance_OP);

Serial.print("Calibrate Distance OP: ");
Serial.println(cal_distance_OP);
Serial.println(" ");
delay(500);

digitalWrite(led_R, LOW);
digitalWrite(led_L, LOW);
digitalWrite(led_OP, HIGH);
delay(100);
digitalWrite(led_OP, LOW);
delay(100);

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
