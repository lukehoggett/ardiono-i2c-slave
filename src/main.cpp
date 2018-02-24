/**
 * I2C Slave to run on a Microview with Rapsberry Pi master
 */
#include <Arduino.h>
#include <Wire.h>
#include <MicroView.h>
#include "main.h"


#define I2C_DEVICE_ADDRESS 0x20

#define START_OF_DATA 0x10
#define END_OF_DATA 0x20

void setup() {
  
  // uView.begin();
  Serial.begin(9600);           // start serial for output
  Wire.begin(I2C_DEVICE_ADDRESS);                // join i2c bus with address 0x20
  Wire.onReceive(receiveEvent); // register event
}

void loop() {
  // Serial.println("loop");
  delay(1000);
  // if (Wire.available() > 0) {
  //   Serial.print(Wire.read(), HEX); Serial.print(" ");
  // }
  // return;

  // processPageMessage();
  processSimpleMessage();
  
}

void processSimpleMessage() {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
    Serial.println(" ");
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.print("last ");
  Serial.println(x);         // print the integer
}

void processPageMessage() {
  if (Wire.available() > 0) {
    Serial.print("Available: ");
    Serial.println(Wire.available());
  }
  
  if (Wire.available() > 26) { // when buffer full, process data. 66 =  start byte + colour + 64 pixel data + end byte
    Serial.print("Buffer full: ");
    Serial.println(Wire.available());
    // error check - make sure our data starts with the right byte
    byte start = Wire.read();
    Serial.print("Start ");
    Serial.print(START_OF_DATA);
    Serial.print(" ");
    Serial.println(start);
    if (start != START_OF_DATA) {
      //else handle error by reading remaining data until end of data marker (if available)
      Serial.println("Wire read NOT equals START_OF_DATA");
      byte notEnd = Wire.read();
      Serial.println(notEnd);
      while (Wire.available() > 0 && Wire.read() != END_OF_DATA) {}
      return;
    } else {
      Serial.println("Wire read equals START_OF_DATA");
    }

    byte c = Wire.read(); //read our color byte so we know if these are the R, G or B pixels.
    Serial.print("Color byte: ");
    Serial.println(c, HEX);
    //depending on c read pixels in as R G or B
    //read red display data
    if (c == 0) {
      Serial.println("Start reading red: ");
      for (byte x = 0; x < 8; x++){
        for (byte y = 0; y < 8; y++){
           // PixelRGB *p = Colorduino.GetPixel(x,y);
           // p->r = Wire.read();
           byte r = Wire.read();
           Serial.print(r, HEX);
           Serial.print(" ");
        }
      }
      Serial.println(" ");
      Serial.println("Finish reading red: ");
    }
    
    //read green display data
    if (c == 1) {
      Serial.print("Start reading green: ");
      for (byte x = 0; x < 8; x++){
        for (byte y = 0; y < 8; y++){
          // PixelRGB *p = Colorduino.GetPixel(x,y);
          //  p->g = Wire.read(); 
          byte g = Wire.read();
          Serial.print(" ");
          Serial.print(g, HEX);
          Serial.print(" ");
        }
      }
      Serial.println(" Finish reading green: ");
    }
    
    //read blue display data
    if (c == 2) {
      Serial.print("Start reading blue: ");
      for (byte x = 0; x < 8; x++){
        for (byte y = 0; y < 8; y++){
           // PixelRGB *p = Colorduino.GetPixel(x,y);
           // p->b = Wire.read(); 
           byte b = Wire.read();
           Serial.print(" ");
           Serial.print(b, HEX);
           Serial.print(" ");
           
        }
        
      }
      Serial.println(" Finish reading blue: ");
    }
    
    //read end of data marker
    if (Wire.read() == END_OF_DATA) {
      Serial.println("Wire read equals END_OF_DATA -->");
      
      Serial.println(" ,,");
      //if colour is blue, then update display
      if (c == 2) {
        // Colorduino.FlipPage();
      }
    } 
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  // Serial.print("receiveEvent: ");
  // Serial.print(howMany);
  // Serial.println("...");
  
  // processSimpleMessage();
}
