


#include <RTClib.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <Wire.h>


const char* ssid = "SARAGI";
const char* password = "anakrantau";

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
ESP8266WebServer server(80);
Servo myservo;

DateTime alarmTime;
bool alarmSet = false;


void setup() {
  Serial.begin(115200);
  

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  server.on("/endpoint", handleEndpoint);
  server.begin();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  //server.on("/open_servo", []() {
    //myservo.write(90); // Open the servo to 45 degrees
    //delay(10000);       // Wait for 5 seconds
    //myservo.write(0);  // Close the servo
    //server.send(200, "text/plain", "Servo action completed");
  //});

  //server.on("/", handleRoot);
  //server.on("/setAlarm", handleSetAlarm);
  //server.begin();    
  //Serial.println("HTTP Server started");

  //if (!rtc.GetIsRunning()) {
    //Serial.println("RTC tidak berjalan, mengatur waktu.");
    //Rtc.SetIsRunning(true);
  //}

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
    }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    }

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  
  server.on("/get_time", handleGetTime);
  server.on("/set_time", handleSetTime);
  server.on("/move_servo", handleMoveServo);
  server.on("/set_alarm", handleSetAlarm);
  server.begin();
  Serial.println("HTTP server started");

 // RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  //if (Rtc.GetDateTime() != compiled) {
     //Serial.println("RTC tidak sinkron dengan waktu kompilasi, mengatur waktu.");
     //Rtc.SetDateTime(compiled);
  //}
  myservo.attach(14); // Connect the servo signal wire to D5

 }
    
void loop() {
  server.handleClient();
  if (alarmSet) {
    DateTime now = rtc.now();
    if (now >= alarmTime) {
      moveServoToAngle(90);
      alarmSet = false;
    }
  }
}

void handleGetTime() {
  DateTime now = rtc.now();
 String currentTime = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " +
                       String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
               
  server.send(200, "text/plain", currentTime);

  Serial.println("Handling get_time request");
  } 

  
void handleSetTime() {
  if (server.hasArg("year") && server.hasArg("month") && server.hasArg("day") &&
      server.hasArg("hour") && server.hasArg("minute") && server.hasArg("second")) {
    int year = server.arg("year").toInt();
    int month = server.arg("month").toInt();
    int day = server.arg("day").toInt();
    int hour = server.arg("hour").toInt();
    int minute = server.arg("minute").toInt();
    int second = server.arg("second").toInt();

      rtc.adjust(DateTime(year, month, day, hour, minute, second));
      server.send(200, "text/plain", "Time set successfully");
    } 
      else {
      server.send(400, "text/plain", "Missing Parameters");
      Serial.println("Invalid request to set alarm");
    }

    Serial.println("Handling set_time request");
  }

  void handleMoveServo() {
    if (server.hasArg("angle")) {
      int angle = server.arg("angle").toInt();
      myservo.write(angle);
      server.send(200, "text/plain", "Servo moved");
  } else {
    server.send(400, "text/plain", "Missing angle parameter");
  }
  Serial.println("Handling move_servo request");
  }

  void handleSetAlarm() {
  if (server.hasArg("alarm")) {
    String alarmString = server.arg("alarm");
    int year, month, day, hour, minute, second;
    if (sscanf(alarmString.c_str(), "%d-%d-%d-%d-%d-%d", &year, &month, &day, &hour, &minute, &second) == 6) {
      alarmTime = DateTime(year, month, day, hour, minute, second);
      alarmSet = true;
      server.send(200, "text/plain", "Alarm set successfully");
    } else {
      server.send(400, "text/plain", "Invalid alarm format");
    }
  } else {
    server.send(400, "text/plain", "Missing alarm parameter");
  }
  Serial.println("Handling set_alarm requestt");
}

  void moveServoToAngle(int angle) {
    myservo.write(angle);
    delay(1000);
    myservo.write(0);
  }

    void handleEndpoint() {
      if (server.method() == HTTP_GET) {
        String paramValue = server.arg("parameter");

        server.send(200, "text/plain", "GET request handle");
      } else if (server.method() == HTTP_POST) {
        String requestBody = server.arg("plain");

        server.send(200, "text/plain", "POST request handled");
      } else {
        server.send(405, "text/plain", "Method Not Allowed");
      }
    }
 
  
