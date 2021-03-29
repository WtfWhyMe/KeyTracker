#include <BluetoothSerial.h>
#include <SoftwareSerial.h>

#define DEBUG 0

//pins
#define SIGFOX_RX 12
#define SIGFOX_TX 14
#define GPS_RX 33
#define GPS_TX 32

//AT commands
#define SEND_COMMAND "AT$SF="
#define GET_ID "AT$I=10"
#define GET_PAC "AT$I=11"

// syntax (rx, tx)
SoftwareSerial gps(GPS_RX, GPS_TX);
SoftwareSerial sigfox(SIGFOX_RX, SIGFOX_TX);
BluetoothSerial blue;

#define messageBufferSize 15
#define IO_OUTPUT 21 // pin connected to a transistor that acts like a switch for LED and Buzzer
uint8_t msg[messageBufferSize];
char info;

void setup() {
  Serial.begin(9600);
  sigfox.begin(9600);
  gps.begin(9600);
  blue.begin("ESP Bluetooth");
  pinMode(IO_OUTPUT, OUTPUT);
  if(DEBUG){
     getIDandPAC();
     Serial.println("end of setup");
  }
} // end of setup

void loop() {

  if (blue.available()) //Check if we receive anything from Bluetooth
  {
    info = blue.read();
    Serial.print("Received:"); 
    Serial.println(info);
  }
  if(info == '1'){
    digitalWrite(IO_OUTPUT, HIGH);
  }
  if(info == '0'){
    digitalWrite(IO_OUTPUT, LOW);
  }

  unsigned long timePassed = millis();
  if(timePassed%1800000 == 0){ // 1800000 miliseconds = 30 min
    //53.7358, -8.9957 sample coord needed1
   // getGpsLoc();
   // sendMessage(msg, messageBufferSize);
  }
} // end of loop

void getGpsLoc(){
  String gpsData = "";
  while(!gps.available()){
    Serial.println("Waiting for gps signal");
    delay(1000);
  }
  while(gps.available()){
     gpsData += gps.read();
  }
    if(DEBUG) {
      Serial.println(gpsData);
    }
    createMessage(gpsData);
}
 

void createMessage(String coord){
  //TODO get gps data and extract lat/long from it
  if(DEBUG){
      Serial.println("msg array:");
    }
  for(int i = 0; i < coord.length(); i++){
    msg[i] = coord[i];
    if(DEBUG){
      Serial.println(msg[i]);
    }
  }
} // end of create message

void sendMessage(uint8_t msg[], int size)
{ // Code Reference https://github.com/luisomoreau/arduinoUno-wisol/blob/master/sigfox-hello-world/sigfox-hello-world.ino
  String response = "";
  String command = SEND_COMMAND;
  String message = "";
  
  Serial.println("Sending...");
  for (int i = 0; i < size; i++){
      message = String(msg[i], HEX);
  
      if (message.length() == 1){
        message = "0" + message;
      }
  
      command += message;
    }
    command += "\n\r";
    
  sigfox.print(command);
  while (!sigfox.available()){      
    Serial.println("Waiting for response");
    delay(1000);
  }
    //Serial.println("sigfox available");
    while(sigfox.available()){
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
      sigfox.print(GET_ID);
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
