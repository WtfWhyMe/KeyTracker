#include <SoftwareSerial.h>

#define rxPin 12
#define txPin 14
#define SEND_COMMAND "AT$SF="
unsigned char buffer[64]; 
int count = 0;
SoftwareSerial Sigfox(rxPin, txPin);

//Message buffer
uint8_t msg[12];

void setup() {
  Serial.begin(9600);
  //pinMode(rxPin, INPUT);
  //pinMode(txPin, OUTPUT);
  Sigfox.begin(9600);
  delay(10000); // delay to have time to connect pins back
  Serial.println("end of setup");
}

void loop() {
  msg[0]=0xC0;
  msg[1]=0xFF;
  msg[2]=0xEE;

  sendMessage(msg, 3);
  delay(100000);
}//end of loop

void sendMessage(uint8_t msg[], int size)
{
  String response = "";
  String command = "";
  String message;
  command += SEND_COMMAND;
  Serial.println("Inside sendMessage");

  Serial.println("Sending...");
    //Serial.println(sigfoxCommand);
  for (int i = 0; i < size; i++)
    {
      message = String(msg[i], HEX);
  
      if (message.length() == 1)
      {
        message = "0" + message;
      }
  
      command += message;
    }
    command += "\n\r";
    
  Sigfox.print(command);
  while (!Sigfox.available())
  {      
    Serial.println("Waiting for response");
    delay(1000);
  }
    //Serial.println("sigfox available");
    while(Sigfox.available()){
     char c = (char)Sigfox.read();
      response += c;
      delay(10);
    }
    Serial.println(response);
}
