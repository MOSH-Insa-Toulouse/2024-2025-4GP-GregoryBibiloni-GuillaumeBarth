#define Capt A0 
float result = 0;
#include <SPI.h>

// Define the MCP41100 OP command bits (only one POT)
// Note: command byte format xxCCxxPP, CC command, PP pot number (01 if selected) 
#define MCP_NOP 0b00000000
#define MCP_WRITE 0b00010001
#define MCP_SHTDWN 0b00100001

const int ssMCPin = 10; // Define the slave select for the digital pot

#define WAIT_DELAY 150

void SPIWrite(uint8_t cmd, uint8_t data, uint8_t ssPin) // SPI write the command and data to the MCP IC connected to the ssPin
{
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0)); //https://www.arduino.cc/en/Reference/SPISettings
  
  digitalWrite(ssPin, LOW); // SS pin low to select chip
  
  SPI.transfer(cmd);        // Send command code
  SPI.transfer(data);       // Send associated value
  
  digitalWrite(ssPin, HIGH);// SS pin high to de-select chip
  SPI.endTransaction();
}

void setup() {
  // put your setup code here, to run once:
pinMode(Capt, INPUT);
Serial.begin (9600);
Serial.println("start");

  pinMode (ssMCPin, OUTPUT); //select pin output
  digitalWrite(ssMCPin, HIGH); //SPI chip disabled
  SPI.begin(); 
  Serial.println(F("[MCP Digital Pot Test]")); //print to serial
}



void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 256; i++) 
  {
    Serial.println(i);
    SPIWrite(MCP_WRITE, i, ssMCPin);
    delay(WAIT_DELAY);
    result=analogRead(Capt);
Serial.println(result);
  }
  delay (WAIT_DELAY);
}
