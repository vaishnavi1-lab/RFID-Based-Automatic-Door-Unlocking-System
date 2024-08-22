/* RFID Door Lock System using Arduino and RFID Module
    www.Electroduino.com
 */
 
 //Include library
 #include <SPI.h>
 #include <MFRC522.h>
 #include <LiquidCrystal.h>
 #include <Servo.h>

 #define RST_PIN 9 // Define RST pin of RFID Connnected to Digital Pin 9
 #define SS_PIN 10 // Define RST pin of RFID Connnected to Digital Pin 10

 #define Sensor_Pin 7 // Define IR Sensor pin Connnected to Digital Pin 7

 #define Red_LED 2   // Define Red LED Connnected to Digital Pin 2
 #define Green_LED 3 // Define Green LED Connnected to Digital Pin 3
 #define Buzzer 4    // Define Buzzer pin Connnected to Digital Pin 4

 byte readCard[4];
 //Unique ID of RFID Tag, which you want to give access. 
 String MasterTag = "A9591AB4";  // REPLACE this Tag ID with your Tag ID!!!
 String tagID = "";

 // Create instances
 MFRC522 mfrc522(SS_PIN, RST_PIN);
 LiquidCrystal lcd(A0, A1, A2, A3, A4, A5); // Define LCD Display pin Connnected to Arduino Pin 
 Servo myServo; // Servo motor

 void setup() 
  {
   // Initiating
   SPI.begin(); // SPI bus
   mfrc522.PCD_Init(); // MFRC522
   lcd.begin(16, 2); // //Initialise the LCD to 16x2 Character Format
   Serial.begin(9600); //open the serial port at 9600 baudrate.
  
   myServo.attach(6);  // declare Servo Connnected to Digital Pin 6
  
   //Set Components pin modes
   pinMode(Sensor_Pin,INPUT);
   pinMode(Red_LED, OUTPUT);
   pinMode(Green_LED, OUTPUT);
   pinMode(Buzzer, OUTPUT);

   myServo.write(10); // Initial lock position of the servo motor
  
   digitalWrite(Red_LED, HIGH);    // Red LED on
   digitalWrite(Green_LED, LOW); // Green LED off
   digitalWrite(Buzzer, LOW);    // Buzzer off
  
   //Print beginning message on LCD Display
   lcd.setCursor(0,0);
   lcd.print("ELECTRODUINO.COM");
   lcd.setCursor(0, 1);
   lcd.print(" RFID DOOR LOCK ");
   delay(2000);
  
   digitalWrite(Buzzer, HIGH);// Buzzer on
   delay(50);
   digitalWrite(Buzzer, LOW);// Buzzer off
  
   printNormalModeMessage(); // print Normal Mode Message on LCD Display
  }

 void loop() 
  {
    int sensorValue = digitalRead(Sensor_Pin); // Read IR Sensor Output
    Serial.print (sensorValue); // IR Sensor Output on Serial Monitor

    // If Door is Closed
    if (sensorValue == 0)
     {    
       myServo.write(10);        // Initial lock position of the servo motor
       printNormalModeMessage(); // print Normal Mode Message on LCD Display
       digitalWrite(Red_LED, HIGH);  // Red LED on
       digitalWrite(Green_LED, LOW); // Green LED off
 
       //Wait until new tag is available
       while (getID()) 
        {
         // If RFID Card or Key fob tag is Correct   
         if (tagID == MasterTag) 
          {
            myServo.write(170); // // Initial unlock position of the servo motor
            
            //Print message on LCD Display
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("     Welcome    ");
            lcd.setCursor(0,1);
            lcd.print("  Door is Open  ");
              
            Green_LED_Blink(); // Green LED & Buzzer function
            digitalWrite(Red_LED, LOW); // Red LED off
            digitalWrite(Buzzer, LOW);  // Buzzer off
          }
         // If RFID Card or Key fob tag is Wrong      
         if (tagID != MasterTag) 
          {
            //Print message on LCD Display
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("   Wrong Card   ");
            lcd.setCursor(0,1);
            lcd.print("  Access Denied! ");
            delay(2000); 
             
            Red_LED_Blink2();// Red LED & Buzzer function2

            //Print message on LCD Display
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("     Please     ");
            lcd.setCursor(0,1);
            lcd.print("   Try Again!   ");
            delay(2000);
       
            digitalWrite(Green_LED, LOW); // Green LED off
            digitalWrite(Buzzer, LOW);    // Buzzer off
            printNormalModeMessage();     // print Normal Mode Message on LCD Display
          }
        }
     }
     
    // If door is open...
    else 
     {
      myServo.write(170); // // Initial unlock position of the servo motor
      
      //Print message on LCD Display
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  Door is Open! ");
      lcd.setCursor(0, 1);
      lcd.print(" Close The Door ");
      
      Red_LED_Blink1(); // Red LED & Buzzer function2
     }
  }
  

  //Read new tag if available
  boolean getID() 
   {
    // Getting ready for Reading PICCs
    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return false;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    return false;
    }
    tagID = "";
    for ( uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
    //readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
    }
    tagID.toUpperCase();
    mfrc522.PICC_HaltA(); // Stop reading
    return true;
   }
 // print Normal Mode Message on LCD Display Function
 void printNormalModeMessage()
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("    Welcome   ");
   lcd.setCursor(0, 1);
   lcd.print("Scan Your Card>>");
  }
  
 // Red LED & Buzzer function1 
 void Red_LED_Blink1() 
  {
   for( int i = 0; i < 2; i = i +1 )  
    {  
     digitalWrite(Red_LED, HIGH); // Red LED on
     digitalWrite(Buzzer, HIGH);  // Buzzer on
     delay(700);   
     digitalWrite(Red_LED, LOW); // Red LED off
     digitalWrite(Buzzer, LOW);  // Buzzer off
     delay(700);  
    // We can adjust the delay time accordingly  
    }  
  }

 // Red LED & Buzzer function2
 void Red_LED_Blink2() 
  {
   for( int i = 0; i < 5; i = i +1 )  
    {  
     digitalWrite(Red_LED, HIGH); // Red LED on
     digitalWrite(Buzzer, HIGH);  // Buzzer on 
     delay(100);   
     digitalWrite(Red_LED, LOW); // Red LED off
     digitalWrite(Buzzer, LOW);  // Buzzer off
     delay(100);  
     // We can adjust the delay time accordingly  
    }  
  }

 // Green LED & Buzzer function
 void Green_LED_Blink() 
  {
   for( int i = 0; i < 15; i = i +1 )  
    {  
     digitalWrite(Green_LED, HIGH); // Green LED on
     digitalWrite(Buzzer, HIGH);    // Buzzer on
     delay(150);   
     digitalWrite(Green_LED, LOW); // Green LED off
     digitalWrite(Buzzer, LOW);    // Buzzer off
     delay(150);  
     // We can adjust the delay time accordingly  
    }  
  }
