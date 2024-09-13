
// include library untuk RTC
#include <virtuabotixRTC.h>

//library ESP8266 WiFi
#include <ESP8266WiFi.h>
#include <CTBot.h>

//imisialisasi pin untuk RTC
#define PIN_CLK 5 //D1 -- pinout nodemcu esp822 D1 - GPI05
#define PIN_DAT 4 //D2
#define PIN_RST 2 //D4

//buat object untuk RTC
virtuabotixRTC myRTC( PIN_CLK, PIN_DAT, PIN_RST );

//object untuk CTBot
CTBot myBot;

//konfigurasi to wifi
String ssid = "A05s";
String pass = "RAHASIAA";

//variabel untuk token telegram bot dan variabel untuk ID Telegram
String token = "7203180982:AAGzYk3Q1AOGaS7Njg0psDAkwZsuyyslmcU";
const int id = 1108769658;



void setup() {
  // aktifkan serial
  Serial.begin(9600);
  //tentukan jam, hari, tanggal
  //urutan parameter : detik, menit, jam, hari, tangal, bulan, tahun
  //hari = 1(senin), 2(selasa), 3(rabu), 4(kamis), 5(jumat), 6(sabtu)
  //#define detik
  //myRTC.setDS1302Time( 00, 14, 17, 1, 26, 10, 2020 );

  //konek to telegram melalui wifi
  myBot.wifiConnect( ssid, pass );
  //set telegram token
  myBot.setTelegramToken(token);  

  //uji apakah terkoneksi ke telegram atau tidak
  if( myBot.testConnection() )
     Serial.println("Koneksi Berhasil");
  else
     Serial.println("Koneksi Gagal");
}

void loop() {
   // update waktu
   myRTC.updateTime();

   //sediakan variabel penampung untuk
   //hari, jam, menit, detik, tanggal, bulan, tahun
   
   String hari, jam, menit;
   
   //baca hari
   hari = myRTC.dayofweek ; 
   //uji hari
   if(hari == "1") hari = "Senin";
   else if( hari=="2" ) hari = "Selasa";
   else if( hari=="3" ) hari = "Rabu";
   else if( hari=="4" ) hari = "Kamis";
   else if( hari=="5" ) hari = "Jumat";
   else if( hari=="6" ) hari = "Sabtu";
   else if( hari=="7" ) hari = "Minggu";
   
   
   //baca jam, menit, detik
   jam   = String(myRTC.hours);
   menit = String(myRTC.minutes);
   //gabungkan jadi 1 variabel
   String jam_sekarang = jam + ":" + menit;
    

   //tampilkan di serial monitor
   Serial.println(hari + "," + jam_sekarang);

   //variable object untuk pesan
   TBMessage pesan;

   //uji jika berhasil menerima pesan dari telegram
   if( myBot.getNewMessage(pesan) )
   {
    Serial.println("Pesan Masuk : " + pesan.text);
   }
   
   delay(1000);

}
