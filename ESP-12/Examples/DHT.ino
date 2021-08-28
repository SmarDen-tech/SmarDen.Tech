//=================All Library Files===============================
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<TaskScheduler.h>
#include "DHT.h"


#define DHTPIN D2
#define DHTTYPE DHT11
#define refresh_time 5000 //define the sensor value refresh time in miliseconds
//=========================Enter Device ID==========================
String DeviceID = "........"; // Enter you device ID 

//=======================WiFi and Mqtt Credentials=================
const char* ssid = "............."; // Wifi SSID
const char* password = ".........";   // WiFi Pssword
const char* mqtt_server = "login.smarden.tech";  // Mqtt Server Address, please do not change
const char* username = "smardentech";    //your instance username
const char* mqtt_pass = "smardentech1@3$";      // your instance password

//=======================Pub/sub Topics==============================
String Pub_topic = "smardentechresponse";  // Publishing topic, click on the element properties to find your publishing topic. 

//=====================Add element ID=================================
String TempID =  "........................."; // Element ID for the Temperature
String HumID = "..........................."; // Element ID for the Humidity 

//=====================Connection Status messages=====================
String willmessage = "dev_status " + DeviceID + " offline"; //do not change
String mqttstatus = "dev_status " + DeviceID + " online";  // do not change

const int RSSI_MAX = -50; // define maximum strength of signal in dBm
const int RSSI_MIN = -100; // define minimum strength of signal in dBm
String signalstrength ;

//===================TaskSchedular==========================================
void temperature();
Task T1(10000, TASK_FOREVER, &temperature);

Scheduler runner;
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print(WiFi.RSSI());//Signal strength in dBm
  Serial.print("dBm (");
  Serial.print(dBmtoPercentage(WiFi.RSSI()));//Signal strength in %
  Serial.print("% )");
  signalstrength = "sig_strength " + DeviceID + " " + String(dBmtoPercentage(WiFi.RSSI())) + "%";
}


//===============================================MQTT Reconnect function==============================================
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), username, mqtt_pass, Pub_topic.c_str(), 1, 0, willmessage.c_str())) {
      Serial.println("connected");
      client.publish(Pub_topic.c_str(), mqttstatus.c_str(), false);
      client.publish(Pub_topic.c_str(), signalstrength.c_str(), false);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//=========================Convert dBm to Percentage============================================
int dBmtoPercentage(int dBm)
{
  int quality;
  if (dBm <= RSSI_MIN) {
    quality = 0;
  }
  else if (dBm >= RSSI_MAX) {
    quality = 100;
  }
  else {
    quality = 2 * (dBm + 100);
  }
  return quality;
}//dBmtoPercentage

//========================================Setup Part============================================
void setup() {
  Serial.begin(115200);
  setup_wifi();
  runner.init(); // Initializing the runner and enebling the task 
  Serial.println("Initialized scheduler");
  runner.addTask(T1);
  Serial.println("Added T1");
  T1.enable();
  dht.begin();
  client.setServer(mqtt_server, 1884); //mqtt port will be 1884
}

//===============================Temperature Function ========================================
void temperature () {
  delay(refresh_time);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  
  Serial.println(" ");
  Serial.print("Publishing Device status and Signal Strength");
  client.publish(Pub_topic.c_str(), signalstrength.c_str(), false); // Publish the Signal Strength 
  
  String Temp_msg = "smarden_dev_sens " + TempID + " " + String(t);
  String Hum_msg = "smarden_dev_sens " + HumID + " " + String(h);
  client.publish(Pub_topic.c_str(), Temp_msg.c_str(), true); // Publish the temperature to the server
  client.publish(Pub_topic.c_str(), Hum_msg.c_str(), true); // publish the Humidity to the server
}
//===================================Loop Part======================================================
void loop() {
  runner.execute();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
