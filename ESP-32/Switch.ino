#include<PubSubClient.h>
#include<WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>

#define Switch 2 // Define the pin Number on which the switch is connected and default payloads are on and off
//=========================Add Device ID Here===============================
String DeviceID = "......"; // Enter the device ID

//============================WiFi and MQTT Credentials======================
const char* mqtt_server = "login.smarden.tech"; // This is the Mqtt server address, do not change.
const char* username = "smardentech"; // Enter your instance username from the Instance tab
const char* mqtt_pass = "smardentech1@3$"; // Enter your instance password from the Instance tab

//===========================Add topics here================================
char* Topic = "..........."; // Enter you topic for the device that you've created
String Pub_topic = "smardentechresponse"; // Enter Publish topic on which the data will be published.

//====================Add  Element ID Here===================================
String SwitchID = "............"; // Enter your switch ID
String willmessage = "dev_status " + DeviceID + " offline";
String mqttstatus = "dev_status " + DeviceID + " online";
String signalstrength ;
const int RSSI_MAX = -50; // define maximum strength of signal in dBm
const int RSSI_MIN = -100; // define minimum strength of signal in dBm
#define refresh_time 10000  //define the signal strength refresh time in miliseconds 


//==================================DEFINE THE OBJECT=====================================
WiFiClient espclient;
PubSubClient client(espclient);
AsyncWebServer server(80);
DNSServer dns;
void callback(char* topic, byte* Payload, unsigned int length);
TaskHandle_t Task1;

//=======================================SETUP PART=======================================
void setup() {
  pinMode(Switch, OUTPUT);
  Serial.begin(115200);
  AsyncWiFiManager wifiManager(&server, &dns);

  //================================WIFI MANAGER=============================================
  {
    wifiManager.autoConnect("ESP32 Smart Switch");
    //wifimanager.autoConnect();
    Serial.println("Conneted...");
  }
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);
  xTaskCreatePinnedToCore(
    Task1code,   /* Task function. */
    "Task1",     /* name of task. */
    1000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */

}

//==========================================MAIN LOOP========================================================================
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
//========================================Task1code=========================================================================
void Task1code( void * pvParameters ) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {
    client.publish(Pub_topic.c_str(), signalstrength.c_str(), false);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    Serial.println("Printing Signal Strength");
  }
}
//=========================================CALLBACK FUNCTION=============================
void callback(char* topic, byte* payload, unsigned int length)
{
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
    String Pub_msg = "smarden_dev_resp " + SwitchID + " " + msg; // Creating a message that will be published to the server.
    client.publish(Pub_topic.c_str(), Pub_msg.c_str(), true); // Publishing the response to the server
    Serial.println("Switch ON");

  } else if (msg == "off") {
    digitalWrite(Switch, LOW);
    String Pub_msg = "smarden_dev_resp " + SwitchID + " " + msg; // Creating a message that will be published to the server.
    client.publish(Pub_topic.c_str(), Pub_msg.c_str(), true);  // Publishing the response to the server
    Serial.println("Switch OFF");
  }

}

//==============================================RECONNECT FUNCTION TO MAINTAIN THE MQTT CONNECTION==============================
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
