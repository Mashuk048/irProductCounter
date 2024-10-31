/*
 Since my debouncing code works on interrupt on CHANGE, so whenever it was missing the second interrupt(change of level shifting), it would make the "state" variable remain stuck as TRUE and long press detection loop runs endlessly making the device unstable.
 So to solve this, here i am forcefully altering the state to false=0(Depressed) inside the long press detecton loop to stop it from running endlessly.
 Device behaviour: Now any infirm or improper button press will set off the long press detection loop and once counter reaches the threshold(after 3 seconds), it stops the siren that was mistakenly fired up.
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Ticker.h>

// Connect to the WiFi
const char* ssid = "DataSoft_WiFi";//DataSoft_WiFi
const char* password = "support123";
const char* mqtt_server = "broker.datasoft-bd.com";
const int mqttPort = 1883;
const char* did = "PC101";

//mqtt topic
const char* resTopic = "dsiot/pc/count";

#define DEBUG 1

#if DEBUG
#define  P_R_I_N_T(x)   Serial.println(x)
#else
#define  P_R_I_N_T(x)
#endif



//OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);





//MQTT
unsigned long lastReconnectTime = 0;
WiFiClient espClient;
PubSubClient client(espClient);



#define ledPin 2 //D4  ONBOARD LED
//const int sensorPin = 14;//D5
const int sensor1Pin = 0;//D5
const int sensor2Pin = D4;//D5
const int sensor3Pin = D3;//D5




// Holds the current button state. 
volatile int sensor1State = 1, sensor2State = 1,sensor3State = 1;//FOR INPUT PULLED HIGH

//
volatile byte interruptCounter1 = 0;
volatile byte interruptCounter2 = 0;
volatile byte interruptCounter3 = 0;
unsigned int cA,cB;

boolean showA = false;
boolean showB = false;

// Holds the last time debounce was evaluated (in millis).
volatile long lastDebounceTime = 0;

// The delay threshold for debounce checking.
const int debounceDelay = 30;

//snooze time
//unsigned long interval = 60000; //in ms.  5 minutes snooze time default




//TIMER FOR RPM
unsigned long lastRpmCalDoneTime = 0;



// Gets called by the interrupt.
void   ICACHE_RAM_ATTR   ISR1() {
  // Get the pin reading.
  int reading = digitalRead(sensor1Pin);

  // Ignore dupe readings.
  if(reading == sensor1State) return;

  boolean debounce = false;
  
  // Check to see if the change is within a debounce delay threshold.
  if((millis() - lastDebounceTime) <= debounceDelay) {
    debounce = true;
  }

  // This update to the last debounce check is necessary regardless of debounce state.
  lastDebounceTime = millis();

  // Ignore reads within a debounce delay threshold.
  if(debounce) return;

  // All is good, persist the reading as the state.
  sensor1State = reading;


  if(!sensor1State){//if button pin is read 1(High Pulse) or read 0(LOW Pulse), change accordingly and remember it is a guaranteed press 

      interruptCounter1++;
      Serial.print("Counter A: ");
      P_R_I_N_T(interruptCounter1);
      
      //    if(isAlarming){P_R_I_N_T("Already Alarming!");}else{setAlarm_and_Publish("SOS",1);}

  }
  
// Work with the value now.
//  Serial.println("button: " + String(reading));
  
}//ISR ENDS




// Gets called by the interrupt.
void   ICACHE_RAM_ATTR   ISR2() {
  // Get the pin reading.
  int reading = digitalRead(sensor2Pin);

  // Ignore dupe readings.
  if(reading == sensor2State) return;

  boolean debounce = false;
  
  // Check to see if the change is within a debounce delay threshold.
  if((millis() - lastDebounceTime) <= debounceDelay) {
    debounce = true;
  }

  // This update to the last debounce check is necessary regardless of debounce state.
  lastDebounceTime = millis();

  // Ignore reads within a debounce delay threshold.
  if(debounce) return;

  // All is good, persist the reading as the state.
  sensor2State = reading;


  if(!sensor2State){//if button pin is read 1(High Pulse) or read 0(LOW Pulse), change accordingly and remember it is a guaranteed press 

      interruptCounter2++;
      Serial.print("Counter B: ");
      P_R_I_N_T(interruptCounter2);
      showB = true;
      cA=interruptCounter3;

  }
  
// Work with the value now.
//  Serial.println("button: " + String(reading));
  
}//ISR ENDS




// Gets called by the interrupt.
void   ICACHE_RAM_ATTR   ISR3() {
  // Get the pin reading.
  int reading = digitalRead(sensor3Pin);

  // Ignore dupe readings.
  if(reading == sensor3State) return;

  boolean debounce = false;
  
  // Check to see if the change is within a debounce delay threshold.
  if((millis() - lastDebounceTime) <= debounceDelay) {
    debounce = true;
  }

  // This update to the last debounce check is necessary regardless of debounce state.
  lastDebounceTime = millis();

  // Ignore reads within a debounce delay threshold.
  if(debounce) return;

  // All is good, persist the reading as the state.
  sensor3State = reading;


  if(!sensor3State){//if button pin is read 1(High Pulse) or read 0(LOW Pulse), change accordingly and remember it is a guaranteed press 
//    if(isAlarming){P_R_I_N_T("Already Alarming!");}else{setAlarm_and_Publish("SOS",1);}
      interruptCounter3++;
      Serial.print("Counter C: ");
      P_R_I_N_T(interruptCounter3);
      showA = true;
      cA=interruptCounter3;

  }



//  showText();//crashes when external function is called from ISR
  
// Work with the value now.
//  Serial.println("button: " + String(reading));
  
}//ISR ENDS

//Ticker watchDogTicker;

//global timer variables
unsigned  long lastMsgTime = 0;
long INTERVAL = 1000;


void setup() {
    
    
    pinMode(sensor1Pin, INPUT);
    pinMode(sensor2Pin, INPUT);
    pinMode(sensor3Pin, INPUT);
  
  
    attachInterrupt(sensor1Pin, ISR1,  CHANGE);
    attachInterrupt(sensor2Pin, ISR2,  CHANGE);
    attachInterrupt(sensor3Pin, ISR3,  CHANGE);

    initOled();
    showText();
    
    Serial.begin(115200);
    P_R_I_N_T("READY");
    delay(500);
    
    
    setup_wifi();
    setup_mqtt();
//    watchDogTicker.attach(2, showText);// ISRwatchdog ticking every 1 second
}


void loop() {

    if(showA){
      showText();
      show = false;
    }
    
    setMqttReconnectInterval();
    
}//LOOP ENDS







void setup_wifi() {
    // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    P_R_I_N_T(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  P_R_I_N_T("");
  P_R_I_N_T("WiFi connected");
  P_R_I_N_T("IP address: ");
  P_R_I_N_T(WiFi.localIP());
}

void setup_mqtt(){      
   client.setServer(mqtt_server, mqttPort);
   client.setCallback(callback);
}



void setAlarm_and_Publish(const char *type, const int state){
  
  
  digitalWrite(ledPin, state);
  
  StaticJsonDocument<128> doc;
  char buffer[128];
  doc["did"] = did;
  doc["alarm"] = state;
  doc["type"] = type;
  size_t n = serializeJson(doc, buffer);
  Serial.print("Published:");
  P_R_I_N_T(client.publish(resTopic, buffer, n));
  delay(250);
  
}


void setMqttReconnectInterval(){      
    if (!client.connected()) {
      unsigned long now = millis();
      if (now - lastReconnectTime > 5000) {
          lastReconnectTime = now;
          P_R_I_N_T("Ticking every 5 seconds");

          //write your own code

//          P_R_I_N_T(WiFi.status());

          if(WiFi.status() == WL_CONNECTED){
              // Attempt to mqtt reconnect
              if (reconnect()) {
                lastReconnectTime = 0;//GOOD
              }
          }
        
      }//end of interval check
   }else{
      client.loop();
   }
  
}//setMqttReconnect




//
//
//void checkResumeTimer(){
//  unsigned long now = millis();
//  if(now - lastStopTime > stime){
//      SNOOZE = false;//resume radio
//  }
//}
