/* Komunikasi Arduino dengan Nodemcu (Uno ke Nodemcu)
 * Nanobot Workshop
 * Blogspot : https://nanobotworkshop.blogspot.com/
 * Youtube : https://www.youtube.com/channel/UCEHSFDbLGgwbzPTn22XnYpw
 * Instagram : https://www.instagram.com/nanobot.workshop/
 * Tokopedia : https://www.tokopedia.com/nanobotworkshop
 * Shopee : https://shopee.co.id/nanobot.workshop
 */

#include <SoftwareSerial.h>
SoftwareSerial uno(0,1); //RX,TX

//Variabel kirim data
String inputString = "";
bool stringComplete = false;

//Variabel baca data
String data;
char c;

void setup() {
  Serial.begin(9600);
  uno.begin(9600);
}

void loop() {
//Kirim data
  while(Serial.available()){
    char inChar = Serial.read();
    inputString += inChar;
    if(inChar == '\n'){
      stringComplete = true;
    }

    if(stringComplete){
      uno.print(inputString);
      inputString = "";
      stringComplete = false;
    }
  }

//Baca data
  while(uno.available()>0){
    delay(10);
    c = uno.read();
    data+=c;
  }
  
  if (data.length()>0){
    Serial.println(data);
    data="";
  }  
}
