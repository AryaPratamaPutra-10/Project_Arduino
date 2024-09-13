 //include library blynk
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

#define BLYNK_TEMPLATE_ID "TMPL69G0Medb6"
#define BLYNK_TEMPLATE_NAME "NODEMCU"
#define BLYNK_AUTH_TOKEN "fWnIkEQ9w6ji57DSKMnxKLwRuew0XzH9"



char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "TAK MODAL";
char pass[] = "kurakura";

// set virtual pin dari blynk
#define VIRTUAL_PIN_PAKAN_IKAN V0

BLYNK_WRITE(V0) {
  digitalWrite(D1, param.asInt());
}


Servo myservo;

//buat sebuah variabel untuk menampung datastream StatusPakan
int StatusPakan;

void setup(){
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  myservo.attach(5);

  //posisi awal servo
  myservo.write(0);

  pinMode(D1, OUTPUT);
  Blynk.begin("fWnIkEQ9w6ji57DSKMnxKLwRuew0XzH9", "TAK MODAL", "kurakura");

  

  //koneksi ke wifi 
  WiFi.begin("TAK MODAL", "kurakura");
  //uji apakah berhasil konek atau tidak
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
   }

   Serial.println("WiFi Terkoneksi");

//apabila terkoneksi, koneksikan perangkat ke Blynk
Blynk.begin(BLYNK_AUTH_TOKEN, "TAK MODAL", "kurakura");

Serial.println("Blynk Terkoneksi");
}

void BeriPakan()
{
  //proses perputaran tempat pakan
  for(int posisi=0; posisi<=180; posisi++)
  {
    //set posisi servo berdasarkan variabel posisi
    myservo.write(0);
    delay(10);
    }

    //proses perputaran tempat pakan back to early
    for(int posisi=180; posisi>=0; posisi--)
    {
      myservo.write(180);
      delay(10);
    }
  }
void loop() {
  //jalnkan aplikasi blynk dari NodeMCU
  Blynk.run();

  Serial.println("Status Pakan : " + String(StatusPakan));

  //jika StatusPakan = 1 maka beri pakan
  if(StatusPakan == 1)
  {
    //panggil void BeriPakan, 1 siklus perputaran sampai kembali ke
    //posisi awal
    BeriPakan();
    //kembalikan StatusPakan menjadi 0 setelah diberi pakan
    Blynk.virtualWrite(V0, 0);
    myservo.write(0);
    delay(15);
  }
}
