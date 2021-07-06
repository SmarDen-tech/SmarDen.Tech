#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <TaskScheduler.h>

// Update these with values suitable for your network.
#define Switch D4 // Define the pin Number on which the switch is connected and default payloads are on and off
//=========================Add Device ID Here===============================
String DeviceID = "....................."; // Enter the device ID

//============================WiFi and MQTT Credentials======================
const char* ssid = ".............."; // Enter wifi ssid 
const char* password = "..........."; // Enter wifi Password 
const char* mqtt_server = "login.smarden.tech"; // This is the Mqtt server address, do not change. 
const char* username = "smardentech"; // Enter your instance username from the Instance tab
const char* mqtt_pass = "smardentech1@3$"; // Enter your instance password from the Instance tab 

//===========================Add topics here================================
char* Topic = "......................."; // Enter you topic for the device that you've created
String Pub_topic = "smardentechresponse"; // Enter Publish topic on which the data will be published. 

//====================Add  Element ID Here===================================
String SwitchID = "........................"; // Enter your switch ID 

//=========================Connection Status details=========================
String willmessage = "dev_status " + DeviceID + " offline";
String mqttstatus = "dev_status " + DeviceID + " online";
String signalstrength ;

const int RSSI_MAX = -50; // define maximum strength of signal in dBm
const int RSSI_MIN = -100; // define minimum strength of signal in dBm
#define refresh_time 10000  //define the signal strength refresh time in miliseconds 

//===================TaskSchedular==========================================
void dev_status();
Task T1(refresh_time, TASK_FOREVER, &dev_status);

Scheduler runner;
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);  // WiFi function to connect to specified WiFi Network 

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // Once the WiFi is Connected, this function will give the local IP.
  Serial.print(WiFi.RSSI());// Signal strength in dBm
  Serial.print("dBm (");
  Serial.print(dBmtoPercentage(WiFi.RSSI()));// Signal strength in %
  Serial.print("% )");
  signalstrength = "sig_strength " + DeviceID + " " + String(dBmtoPercentage(WiFi.RSSI())) + "%";
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");  // Here the topic will be printed 
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
    digitalWrite(Switch, HIGH);   
    String Pub_msg = "smarden_dev_respo " + SwitchID + " " + msg; // Creating a message that will be published to the server. 
    client.publish(Pub_topic.c_str(), Pub_msg.c_str(), true); // Publishing the response to the server 
    Serial.println("Switch ON"); 

  } else if (msg == "off") {
    digitalWrite(Switch, LOW);  
    String Pub_msg = "smarden_dev_respo " + SwitchID + " " + msg; // Creating a message that will be published to the server. 
    client.publish(Pub_topic.c_str(), Pub_msg.c_str(), true);  // Publishing the response to the server 
    Serial.println("Switch OFF");
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Creating a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), username, mqtt_pass, Pub_topic.c_str(), 1, 0, willmessage.c_str())) {
      Serial.println("connected");
      client.subscribe(Topic);
      client.publish(Pub_topic.c_str(), mqttstatus.c_str(), false); // Publishing the Mqtt Connected Status to the Server
      client.publish(Pub_topic.c_str(), signalstrength.c_str(), false); // Publishing the Signal Strength to the server 
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
int dBmtoPercentage(int dBm) {
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

void setup() {
  pinMode(Switch, OUTPUT);     // Initialize the BUILTIN_LED pin as an outputs
  Serial.begin(115200);
  setup_wifi();
  runner.init(); // Initializing the runner and enebling the task
  Serial.println("Initialized scheduler");
  runner.addTask(T1);
  Serial.println("Added T1");
  T1.enable();
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);
}

void dev_status() {
  client.publish(Pub_topic.c_str(), signalstrength.c_str(), false);
}
void loop() {
  runner.execute();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
