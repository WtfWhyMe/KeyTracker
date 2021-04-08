#include <BluetoothSerial.h>

BluetoothSerial blue;

char info;

void setup() {
  Serial.begin(9600);
  blue.begin("ESP Bluetooth");
}

void loop() {
  if (blue.available()) //Check if we receive anything from Bluetooth
  {
    info = blue.read();
    Serial.print("Received:"); 
    Serial.println(info);
  }
}
