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
#include "nRF24L01.h" // NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ds3231.h> // calls in the various libries I will need to use

#define LED_PIN2 4 //declares pins for The Leds
#define LED_PIN3 5

const int MAXSIZE = 100;

char Recvmsg[MAXSIZE] = ""; // Creates a character array for the received message to be stored in

int len = 0;

int RecvLen = 0;

RF24 radio(9, 10); // NRF24L01 SPI pins. Pin 9 and 10 on the Nano
const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01 

//LiquidCrystal_I2C lcd1(0x27, 20, 4);  // set the LCD address to 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C lcd1(0x27, 20, 4);  // set the LCD address to 0x3F for a 16 chars and 2 line display


void setup(void)
{
  Serial.begin(9600);

  lcd1.init(); //start the LCD
  lcd1.clear(); // Clears the lcd of old text         
  lcd1.backlight(); // Make sure backlight is on

  Wire.begin(); // Starts the I2C communication  
  
  radio.begin(); // Start the NRF24L01
  radio.openReadingPipe(1,pipe); // Get NRF24L01 ready to receive
  radio.startListening(); // Listen to see if information received
  
  pinMode(LED_PIN2, OUTPUT); // Sets the LED pins as outputs
  pinMode(LED_PIN3, OUTPUT); 
}
void loop(void)
{ 
  if (radio.available()) // Check if there is an incomming message
  {
    len = radio.getDynamicPayloadSize(); // Check the size of the incoming message 
    
    char Recvmsg[MAXSIZE] = "";
    radio.read(&Recvmsg, len); // Place the incoming message into the Recvmsg Character array

       delay(5);
       digitalWrite(LED_PIN3, LOW); // Lights a blue led
       digitalWrite(LED_PIN2, HIGH);
       Serial.println(Recvmsg);
       
       lcd1.setCursor(4, 0); // Prints the message to the LCD 
       lcd1.print("Message:");
       
       lcd1.setCursor(1, 1);
       lcd1.print(Recvmsg);

//        RecvLen = sizeof(Recvmsg);
//        Serial.print(RecvLen);
//
//       if (sizeof(Recvmsg) > 16)
//       
//       {
//        for (int positionCounter = 0; positionCounter < 29; positionCounter++) {
//         // scroll one position right:
//         lcd1.scrollDisplayLeft();
//         // wait a bit:
//         delay(150);
//         }
//       }

          lcd1.setCursor(0,0); // Clears only the message line for a new message to take its place
          // lcd1.print("                ");
          delay(150);

          lcd1.clear(); // clears the whole LCD
  }
  else
  {
    digitalWrite(LED_PIN3, HIGH); //Light a red LED
    digitalWrite(LED_PIN2, LOW);
      
      Serial.println("No Message");
      
        lcd1.setCursor(4, 0); 
        lcd1.print("Message:");
      
        lcd1.setCursor(0, 1);
        lcd1.print("---No Message---"); // Prints "No Message to the LCD"
        delay(150);

        lcd1.clear(); // Clears the whole LCD
  }
 }
