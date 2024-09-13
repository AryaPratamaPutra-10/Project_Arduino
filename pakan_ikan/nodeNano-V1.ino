/* Komunikasi Arduino dengan Nodemcu (Nodemcu ke Uno)
 * Nanobot Workshop
 * Blogspot : https://nanobotworkshop.blogspot.com/
 * Youtube : https://www.youtube.com/channel/UCEHSFDbLGgwbzPTn22XnYpw
 * Instagram : https://www.instagram.com/nanobot.workshop/
 * Tokopedia : https://www.tokopedia.com/nanobotworkshop
 * Shopee : https://shopee.co.id/nanobot.workshop
 */

#include <SoftwareSerial.h>
SoftwareSerial node(D1,D2); //RX,TX

//Variabel baca data
String data;
char c;

//Variabel kirim data
String inputString = "";
bool stringComplete = false;

void setup() {
  Serial.begin(9600);
  node.begin(9600);
}

void loop() {
//Baca data
  while(node.available()>0){
    delay(10);
    c = node.read();
    data+=c;
  }
  if (data.length()>0){
    Serial.println(data);
    data="";
  }

//Kirim data
  while(Serial.available()){
    char inChar = Serial.read();
    inputString += inChar;
    if(inChar == '\n'){
      stringComplete = true;
    }

    if(stringComplete){
      node.print(inputString);
      inputString = "";
      stringComplete = false;
    }
  }
}
