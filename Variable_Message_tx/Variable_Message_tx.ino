/* ----------PINS----------
 *  CE 9
 *  CSN 10
 *  SCK 13
 *  MO 11
 *  MI12
 *  
 *  LCD SDA 5
 *  LCD SCL 4
 *  
 *  Button 5
 *  
 *  LED2 2  
 *  LED3 3
 *  ---------------------*/
#include "nRF24L01.h" //NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h" //calls the different libraries I will need to use 

#define SwitchPin 5 // declares the pins to use for the switch and the LEDs 
#define LED_PIN2 2
#define LED_PIN3 3 

const int MAXSIZE = 100; 

char Sentmsg[MAXSIZE] = ""; //Creates a character Array 100 characters long 

String Message = ""; //The variable that the message sent will be stored in
 
RF24 radio(9,10); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO

const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01 

void setup()
{
  Serial.begin(9600);
  pinMode(SwitchPin, INPUT_PULLUP); // Sets the outputs and inputs of various pins 
  
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
   
  digitalWrite(SwitchPin,HIGH); 
  
  radio.begin(); // Start the NRF24L01
  radio.openWritingPipe(pipe); // Get NRF24L01 ready to transmit
}

void loop()
{
    while(Serial.available() > 0 ) // reads in there is a radio module to send through
    {
      Message = Serial.readString(); //reads if there has been text typed into the serial monitor 
        Serial.print(Message); // places the message typed into the message variable
    }
  
    if (digitalRead(SwitchPin) == LOW) //if the button has been pressed
    {
      Message.toCharArray(Sentmsg, 100); //convert the message to a character array called Sentmsg
      radio.write(Sentmsg, MAXSIZE); // write the message to the radio module
      Serial.print(Sentmsg); 
      digitalWrite(LED_PIN3, LOW); //lights a blue led
      digitalWrite(LED_PIN2, HIGH);
      delay(5);
      
    }
  else
  {
    Serial.println("No Message");
      digitalWrite(LED_PIN3, HIGH); // lights a red led 
      digitalWrite(LED_PIN2, LOW);
      delay(5);
  }
}
