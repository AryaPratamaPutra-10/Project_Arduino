#include <Servo.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <time.h>
#include <virtuabotixRTC.h>
#include <EEPROM.h>
#include <RTClib.h>

//RTC
RTC_DS1302 rtc,


// Replace these with your WiFi and Firebase credentials
#define WIFI_SSID "SARAGI"
#define WIFI_PASSWORD "anakrantau"
#define FIREBASE_PROJECT_ID "spelet-2"
#define FIREBASE_API_KEY "AIzaSyDUcXO7DvLTbf18axRPWaoRAeJB4675guo"

//pin RTC
#define PIN_CLK 14 //D5
#define PIN_DAT 4 //D2
#define PIN_RST 2 //D4

//rtc ds1302
virtuabotixRTC myRTC( PIN_CLK, PIN_DAT, PIN_RST );
char dataHari[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String day;
int jaml, menitl, jam2, menit2;
int jamMakan, menitMakan;
int makan,b,c;

String alarmDay;
int alarmHour, alarmMinute;

//servo
Servo myServo;
int servoPin = D1; // Adjust according to your wiring

String userId = "1mSvczgSzaOc713D4YvL98TYKoK2"; // Your specific user ID

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Attach servo
  myServo.attach(servoPin);
  myServo.write(0);

  //myRTC.setDS1302Time();

  // Set up NTP time synchronization
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  //EEPROM
  EEPROM.begin(6);
  jaml = EEPROM.read(0);
  menitl = EEPROM.read(1);
  jam2 = EEPROM.read(2);
  menit2 = EEPROM.read(3);
  jamMakan = EEPROM.read(4);
  menitMakan = EEPROM.read(5);
  delay(100);
}

void loop() {
  //update time
  myRTC.updateTime();

  DateTime now = rtc.now();
  hari = dataHari[now.dayofTheWeek()];


  //format current time as HH:MM
  char currentTime[6];
  sprintf(currentTime, "%02d:%02d", myRTC.hours, myRTC.minutes);

  //Determine current day index (0 
  int currentDay = myRTC.dayofweek - 1;

  String url = "https://firestore.googleapis.com/v1/projects/" + String(FIREBASE_PROJECT_ID) + "/databases/(default)/documents/alarms?key=" + String(FIREBASE_API_KEY);


  // Get current time
  //struct tm timeInfo;
  //if (!getLocalTime(&timeInfo)) {
    //Serial.println("Failed to obtain time");
    //delay(1000);
    //return;
  //}

  // Format current time as HH:MM
  //char currentTime[6];
  //strftime(currentTime, sizeof(currentTime), "%H:%M", &timeInfo);

  // Determine current day index (0 = Sunday, 6 = Saturday)
  //int currentDay = timeInfo.tm_wday;

  // Construct the URL for the Firestore REST API request
  //String url = "https://firestore.googleapis.com/v1/projects/" + String(FIREBASE_PROJECT_ID) + "/databases/(default)/documents/alarms?key=" + String(FIREBASE_API_KEY);

  // Initialize HTTP client
  HTTPClient http;
  WiFiClient client;
  http.begin(client, url); // Specify the URL
  int httpCode = http.GET(); // Make the GET request

  if (httpCode == HTTP_CODE_OK) { // Check if the request was successful
    String payload = http.getString();
    Serial.println(payload);

    // Parse JSON response
    //DynamicJsonDocument doc(1024);
    //deserializeJson(doc, payload);

    // Parse JSON response
    DynamicJsonDocument doc(4096); // Adjust size if necessary
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    // Iterate over each alarm document
    for (JsonObject alarm : doc["documents"].as<JsonArray>()) {
      String alarmUserId = alarm["fields"]["userId"]["stringValue"];
      if (alarmUserId == userId) {
        bool alarmActive = alarm["fields"]["alarmActive"]["booleanValue"];
        String alarmTime = alarm["fields"]["time"]["stringValue"];
        JsonArray alarmDays = alarm["fields"]["days"]["arrayValue"]["values"].as<JsonArray>();

        // Check if alarm is active and matches current time
        if (alarmActive && alarmTime == currentTime) {
          // Check if today is one of the specified alarm days
          for (JsonVariant day : alarmDays) {
            if (day["stringValue"] == getDayName(currentDay)) {
              Serial.println("Activating Servo");
              myServo.write(90); // Open the servo
              delay(5000);       // Keep it open for 5 seconds
              myServo.write(0);  // Close the servo
              delay(60000);      // Wait 1 minute before checking again
              break; // Exit the loop once servo is activated for the day
            }
          }
        }
      }
    }
  } else {
    Serial.printf("Failed to get data, error: %d\n", httpCode);
  }

  http.end(); // Close the connection
  delay(1000); // Delay before next loop iteration
}


String getDayName(int dayIndex) {
  switch (dayIndex) {
    case 0: return "Sunday";
    case 1: return "Monday";
    case 2: return "Tuesday";
    case 3: return "Wednesday";
    case 4: return "Thursday";
    case 5: return "Friday";
    case 6: return "Saturday";
    default: return "";
  }
}
