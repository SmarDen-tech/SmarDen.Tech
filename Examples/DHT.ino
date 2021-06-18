//=================All Library Files===============================
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<TaskScheduler.h>
#include "DHT.h"


#define Switch D4
#define DHTPIN D2
#define DHTTYPE DHT11
#define refresh_time 5000 //define the sensor value refresh time in miliseconds
//=========================Enter Device ID==========================
String DeviceID = "p10M9pA8XBGsSiKowFiWcA";

//=======================WiFi and Mqtt Credentials=================
const char* ssid = "............."; // Wifi SSID
const char* password = ".........";   // WiFi Pssword
const char* mqtt_server = "login.smarden.tech";  // Mqtt Server Address, please do not change
const char* username = ".........";    //your instance username
const char* mqtt_pass = "...........";      // your instance password

//=======================Pub/sub Topics==============================
char* Topic = ".................."; // The Topic that you made for your device 
String Pub_topic = "SMD_af619e492061ce2bdf17e2f86e0fe64c";  // Publishing topic ; do not change 

//=====================Add element ID=================================
String SwitchID = "........................." //Element ID for Switch 
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


//======================================MQTT Callback Function=========================================
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char sub[1000];
  int c = 0;
  char* payl = (char *)payload;
  while (c < length) {
    sub[c] = payl[c];
    c++;
  }
  sub[c] = '\0';
  Serial.println(sub);
  String msg = sub;
  Serial.println();

  if (msg == "on") {
    digitalWrite(Switch, HIGH);   // Turn the LED on (Note that LOW is the voltage level
    String Pub_msg = "smarden_dev_respo " + SwitchID + " " + msg;
    client.publish(Pub_topic.c_str(), Pub_msg.c_str(), true);
    Serial.println("LED On");   // but actually the LED is on; this is becauseit is active low on the ESP-01)

  } else if (msg == "off") {
    digitalWrite(Switch, LOW);  // Turn the LED off by making the voltage HIGH
    String Pub_msg = "smarden_dev_respo " + SwitchID + " " + msg;
    client.publish(Pub_topic.c_str(), Pub_msg.c_str(), true);
    Serial.println("LED OFF");
  }

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
      client.subscribe(Topic);
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
  pinMode(Switch, OUTPUT);     // Initialize the BUILTIN_LED pin as an outputs
  Serial.begin(115200);
  setup_wifi();
  runner.init(); // Initializing the runner and enebling the task 
  Serial.println("Initialized scheduler");
  runner.addTask(T1);
  Serial.println("Added T1");
  T1.enable();
  dht.begin();
  client.setServer(mqtt_server, 1884); //mqtt port will be 1884
  client.setCallback(callback);
}

== == == == == == == == == == == == == == == == = Temperature Function == == == == == == == == == == == == == == == == == == == == == ==
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
  Serial.print("Publishing Device status and Signal Strength");
  client.publish(Pub_topic.c_str(), signalstrength.c_str(), false); // Publish the Signal Strength 
  String Temp_msg = "smarden_dev_respo " + TempID + " " + String(t);
  String Hum_msg = "smarden_dev_respo " + HumID + " " + String(h);
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
