#include <BluetoothSerial.h>
#include <SoftwareSerial.h>

#define DEBUG 0

//pins
#define SIGFOX_RX 12
#define SIGFOX_TX 14

//AT commands
#define SEND_COMMAND "AT$SF="
#define GET_ID "AT$I=10\n\r"
#define GET_PAC "AT$I=11\n\r"

// syntax (rx, tx)
SoftwareSerial sigfox(SIGFOX_RX, SIGFOX_TX);
BluetoothSerial blue;

#define messageBufferSize 12
#define IO_OUTPUT 21 // pin connected to a transistor that acts like a switch for LED and Buzzer

uint8_t msg[messageBufferSize];
char info;

void setup() {
  delay(10000);
  Serial.begin(9600);
  sigfox.begin(9600);
  blue.begin("ESP Bluetooth");
  pinMode(IO_OUTPUT, OUTPUT);
  setMessage();
  if(DEBUG){
     getIDandPAC();
     Serial.println("end of setup");
  }
  Serial.println("end of setup");
} // end of setup

void loop() {
  if (blue.available()) //Check if we receive anything from Bluetooth
  {
    info = blue.read();
    Serial.print("Received:"); 
    Serial.println(info);
    
    if(info == '1'){
      digitalWrite(IO_OUTPUT, HIGH);
    }
    if(info == '0'){
      digitalWrite(IO_OUTPUT, LOW);
    }
  }
    
  /*
   * This if block acts as a non-blocking delay and the code inside will be executed every 30 min
   * It is needed to limit number of messages that are sent to sigfox backend as there are limitations
   * and reduce power consumption.
  */
 // unsigned long timePassed = millis();
  //if(timePassed%1800000 == 0){ // 1800000 miliseconds = 30 min
   sendMessage(msg, messageBufferSize);
   delay(10000);
  //}
} // end of loop

void setMessage(){
  //53.73579 -8.99580
  msg[0]=0x53;
  msg[1]=0xF7;
  msg[2]=0x35;
  msg[3]=0x79;
  msg[4]=0x00;
  msg[5]=0xA8;
  msg[6]=0xF9;
  msg[7]=0x95;
  msg[8]=0x80;
}

void sendMessage(uint8_t msg[], int size){ 
  // Code Reference https://github.com/luisomoreau/arduinoUno-wisol/blob/master/sigfox-hello-world/sigfox-hello-world.ino
  String response = "";
  String command = SEND_COMMAND;
  String message = "";
  
  Serial.println("Sending...");
   for (int i = 0; i < size; i++){
      message = String(msg[i], HEX);
      command += message;
    }
    command += "\n\r";
    
    if(DEBUG){
      Serial.println(message);
      Serial.println(command);
    }
  //Send command
  sigfox.print(command);
  
  while (!sigfox.available()){      
    Serial.println("Waiting for response");
    delay(1000);
  }
    while(sigfox.available()){
      //collect response received
      char c = (char)sigfox.read();
      response += c;
      delay(10);
    }
    Serial.println(response);
}// end of send message


void getIDandPAC(){
    String id = "";
    String pac = "";
    sigfox.print(GET_ID);
    while (!sigfox.available()){      
      Serial.println("Waiting for ID");
      delay(1000);
    }
      while(sigfox.available()){
        char c = (char)sigfox.read();
        id += c;
        delay(10);
      }
      Serial.println(id);
      sigfox.print(GET_PAC);
    while(!sigfox.available()){      
      Serial.println("Waiting for PAC");
      delay(1000);
    }
    while(sigfox.available()){
      char c = (char)sigfox.read();
      pac += c;
      delay(10);
    }
    Serial.println(pac);
}// end of get id and pac
