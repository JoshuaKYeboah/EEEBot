// master esp32 maze navigation


// /*
//   LiquidCrystal Library - Hello World

//  Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
//  library works with all LCD displays that are compatible with the
//  Hitachi HD44780 driver. There are many of them out there, and you
//  can usually tell them by the 16-pin interface.

//  This sketch prints "Hello World!" to the LCD
//  and shows the time.

//   The circuit:
//  * LCD RS pin to digital pin 12
//  * LCD Enable pin to digital pin 11
//  * LCD D4 pin to digital pin 5
//  * LCD D5 pin to digital pin 4
//  * LCD D6 pin to digital pin 3
//  * LCD D7 pin to digital pin 2
//  * LCD R/W pin to ground
//  * LCD VSS pin to ground
//  * LCD VCC pin to 5V
//  * 10K resistor:
//  * ends to +5V and ground
//  * wiper to LCD VO pin (pin 3)

//  Library originally added 18 Apr 2008
//  by David A. Mellis
//  library modified 5 Jul 2009
//  by Limor Fried (http://www.ladyada.net)
//  example added 9 Jul 2009
//  by Tom Igoe
//  modified 22 Nov 2010
//  by Tom Igoe
//  modified 7 Nov 2016
//  by Arturo Guadalupi

//  This example code is in the public domain.

//  http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

// */
// #include <Keypad.h>
// #include <LiquidCrystal.h>
// #include <string.h>
// #include <stdlib.h>


// // char key;
// const byte ROWS = 4; //four rows
// const byte COLS = 3; //three columns
// // int count =0;
 
 
// char key;
// char inputs [20] ; 
 

 


 


// char current_press; 

// char keys[ROWS][COLS] = {
//   {'1','2','3'},
//   {'4','5','6'},
//   {'7','8','9'},
//   {'*','0','#'}
// };
// // byte rowPins[ROWS] = {12,14, 27, 26}; //connect to the row pinouts of the keypad
// // byte colPins[COLS] = {25, 33, 32}; //connect to the column pinouts of the keypad
// byte rowPins[ROWS] = {32,33, 25, 26}; //connect to the row pinouts of the keypad
// byte colPins[COLS] = {27, 14, 12}; //connect to the column pinouts of the keypad

// Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


// // initialize the library by associating any needed LCD interface pin
// // with the arduino pin number it is connected to
// const int rs = 15, en = 2, d4 = 16, d5 = 17, d6 = 5, d7 = 18;
// LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// void setup() {
//   // set up the LCD's number of columns and rows:
//   lcd.begin(16, 2);
//   // Print a message to the LCD.
//   lcd.print("hello, world!");
 
  
// }

// // void loop() {
// //   // set the cursor to column 0, line 1
// //   // (note: line 1 is the second row, since counting begins with 0):
// //   // lcd.setCursor(0, 1);
// //   // // print the number of seconds since reset:
// //   // lcd.print(millis() / 1000);

// //     char key = keypad.getKey();
// //    if (key){
    
// //      lcd.setCursor(0, 1);
// //      lcd.print(key);
// //    }
  
// // }




// void loop() {
//   // set the cursor to column 0, line 1
//   // (note: line 1 is the second row, since counting begins with 0):
//   // lcd.setCursor(0, 1);
//   // // print the number of seconds since reset:
//   // lcd.print(millis() / 1000);

//    key = keypad.getKey();

     
//    while (key)
//    {
     
      
//       // current_press = key;

//       if (key=='#')
//       {

//         //get the index of the last the string 
//         int last_index = inputs.length()-1;
//         //remove the last index
        
//         inputs.remove(last_index);
        
//       }

//      else if (key =='2'){

//       //  inputs.push_back(key); 

//        //inputs.append(1, key);
//       //  std::cout << inputs;
//       bool retults = inputs.concat(key);

//      }
  


//    }
  

//      lcd.setCursor(0, 1);
//      lcd.print(inputs);
// }






#include <Keypad.h>
#include <LiquidCrystal.h>
#include <string.h>
#include <stdlib.h>
 #include <Wire.h>

#define slave_addr 8 // definr the i2c slave adress

// char key;
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

// int count =0;
char key;
String inputs = "";
 


char current_press; 
char previous_press;

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {32,33, 25, 26}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {27, 14, 12}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int rs = 15, en = 2, d4 = 16, d5 = 17, d6 = 5, d7 = 18;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Wire.begin();
  lcd.begin(16, 2);
  // lcd.print("function: ");
  Serial.begin(9600);
  Serial.println("Arduino Nano is Running"); 
}

void loop() {
   key = keypad.getKey();
   Serial.println(inputs); 
   
  //  previous_press = current_press;
  //  current_press = key;
   

  //  if (current_press=='2' && previous_press !='2' )
  //  {
  //       LED_print ('d');
        
  //  }
  //  else if(current_press=='2' && previous_press ='2') 
  // {
  //      LED_print ('f');
  // }   

  while(key){ 
      if (key=='#') {
    
        
        int strLength = inputs.length(); // get the length of the string

         if (strLength > 0) {  // check if the string is non-empty
         inputs.remove(strLength - 1);   // remove the last character
         }
           

           lcd.setCursor(0, 1);
           lcd.print("                  "); 
           
           lcd.setCursor(0, 1);
           lcd.print(inputs); 

        
             

      } 
       else if  (key =='2') {
 
          bool results = inputs.concat(key);

           ////////////////////////////////////////////////////////////////////////
           lcd.setCursor(0, 1);
           lcd.print("                  ");           
           lcd.setCursor(0, 1);
           lcd.print(inputs);   
           LED_print('d');

        
         
      }

      else if  (key =='8') {

        bool results = inputs.concat(key);
        //////////////////////////////////////////////////////////////////////////////////
        lcd.setCursor(0, 1);
        lcd.print("                  "); 
        lcd.setCursor(0, 1);
        lcd.print(inputs);
        LED_print('d');
           
             
         
      }
      else if  (key =='6') {

        bool results = inputs.concat(key);
        //////////////////////////////////////////////////////////////////////////////
        lcd.setCursor(0, 1);
        lcd.print("                  "); 
        lcd.setCursor(0, 1);
        lcd.print(inputs);
        LED_print('f');
           
             
         
      }     
      else if  (key =='4') {

        bool results = inputs.concat(key);
        ///////////////////////////////////////////////////////////////
        lcd.setCursor(0, 1);
        lcd.print("                  "); 
        lcd.setCursor(0, 1);
        lcd.print(inputs); 
        LED_print('f');  
             
         
      }  
      else if  (key =='1') {

        bool results = inputs.concat(key);
        ///////////////////////////////////////////////////////////////
        lcd.setCursor(0, 1);
        lcd.print("                  "); 
        lcd.setCursor(0, 1);
        lcd.print(inputs);
        LED_print('f');           
             
         
      } 
       else if  (key =='3') {

        bool results = inputs.concat(key);
        ///////////////////////////////////////////////////////////////
        lcd.setCursor(0, 1);
        lcd.print("                  "); 
        lcd.setCursor(0, 1);
        lcd.print(inputs);   
        LED_print('f');     
         
      }  
       else if  (key =='5') {

        bool results = inputs.concat(key);
        ///////////////////////////////////////////////////////////////
        lcd.setCursor(0, 1);
        lcd.print("                  "); 
        lcd.setCursor(0, 1);
        lcd.print(inputs);   
        LED_print('f');
      }
      else if  (key =='7') {

        bool results = inputs.concat(key);
        ///////////////////////////////////////////////////////////////
        lcd.setCursor(0, 1);
        lcd.print("                  "); 
        lcd.setCursor(0, 1);
        lcd.print(inputs);   
        LED_print('f');     
         
      } 
      else if  (key =='9') {

        bool results = inputs.concat(key);
        ///////////////////////////////////////////////////////////////
        lcd.setCursor(0, 1);
        lcd.print("                  "); 
        lcd.setCursor(0, 1);
        lcd.print(inputs);   
        LED_print('f');     
         
      }  

      else if  (key =='0') {

        bool results = inputs.concat(key);
        ///////////////////////////////////////////////////////////////
        lcd.setCursor(0, 1);
        lcd.print("                  "); 
        lcd.setCursor(0, 1);
        lcd.print(inputs);
        LED_print('f'); 
        }
                  
      else if (key=='*')
      {

       
      //  Wire.beginTransmission(slave_addr);
      //  Wire.write(inputs);
      //  Wire.endTransmission(); 

         Wire.beginTransmission(slave_addr);
         Wire.write(inputs.c_str());
         Wire.endTransmission();

        lcd.setCursor(0, 1);
        lcd.print(inputs + " Sent ");
        inputs = "";
      }           
      key = keypad.getKey();

  }      
}
// 



void LED_print (char a)
{
  if(a=='d')
  {
        lcd.setCursor(0, 0);
        lcd.print("                  ");           
        lcd.setCursor(0, 0);
        lcd.print("distance: ");
  }

    if(a=='f')
  {
        lcd.setCursor(0, 0);
        lcd.print("                  ");           
        lcd.setCursor(0, 0);
        lcd.print("function: ");
  }
}


void update_screen()
{
           lcd.setCursor(0, 1);
           lcd.print("                  ");           
           lcd.setCursor(0, 1);
           lcd.print(inputs); 
}
