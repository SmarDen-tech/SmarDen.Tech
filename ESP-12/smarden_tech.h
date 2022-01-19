#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <TaskScheduler.h>

HTTPClient http;
WiFiClient espClient;
PubSubClient client(espClient);
#define refresh_time 10000 
//===================TaskSchedular==========================================
 void dev_status();
Task T1(refresh_time, TASK_FOREVER, &dev_status);
Scheduler runner;

String payload;               //payload variable for getting the data fron the HTTP
String host;                  // host URL
String signalstrength ;       // variable to store the signal Strength
const int RSSI_MAX = -50;     // define maximum strength of signal in dBm
const int RSSI_MIN = -100;    // define minimum strength of signal in dBm
const char* mq_user;          // MQTT Username
const char* mq_pass;          // MQTT Password
const char* mq_url;           // MQTT Host address
String pub_topic;

//====================== Function to convert signal strength to percentage==================
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
}
//=========================WiFi Function=====================================
void wifi_connect(char* wifi_ssid, char* wifi_pass) {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);                  //printing the SSID
  WiFi.mode(WIFI_STA);                        //defining the WIFI mode as Station
  WiFi.begin(wifi_ssid, wifi_pass);           // WiFi function to connect to specified WiFi Network
  while (WiFi.status() != WL_CONNECTED) {     // Printing "." till the wifi is not connected
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());               // Once the WiFi is Connected, this function will give the local IP.
  Serial.print(WiFi.RSSI());                    // Signal strength in dBm
  Serial.print("dBm (");
  Serial.print(dBmtoPercentage(WiFi.RSSI()));   // Signal strength in %
  Serial.print("% )");
  delay(500);

}
//=============================MQTT Connection Function==========================================================
void mqtt_connect(String accesskey, String deviceid)
{
  HTTPClient http;                              //Declare object of class HTTPClient
  host = "http://iot.smarden.tech/api/dashboard/getmqtt/" + String(accesskey);
  const char* url = host.c_str();
  http.begin(url);                              //Specify request destination
  int httpCode = http.GET();                    //Send the request
  payload = http.getString();                   //Get the response payload from server
  DynamicJsonBuffer jsonBuffer(300);
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (httpCode == 200)
  {
    if (!root.success()) {
      Serial.println(F("Parsing failed!"));
      return;
    }
    mq_user = root["user"];
    Serial.print("Mqtt username:");
    Serial.println(mq_user);
    mq_pass = root["password"];
    Serial.print("Mqtt Password:");
    Serial.println(mq_pass);
    mq_url = root["url"];
    Serial.print("Mqtt URL:");
    Serial.println(mq_url);
    client.setServer(mq_url, 1884);
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      String clientId = deviceid;
      String willmessage = "dev_status " + deviceid + " offline";                                     //payload for offline device status
      String mqttstatus = "dev_status " + deviceid + " online";                                       //payload for online device status
      delay (200);
      String pub_topic = "pub_" + String(deviceid) + "/smardentechresponsee/" + String(mq_user);
      String sub_topic = String(deviceid) + "/smardentechresponsee/" + String(mq_user);
      Serial.println(sub_topic);
      if (client.connect(clientId.c_str(), mq_user, mq_pass, pub_topic.c_str(), 1, 0, willmessage.c_str())) {
        Serial.println("connected");
        client.subscribe(sub_topic.c_str());
        Serial.println("Subscribed to defined topic");
        client.publish(pub_topic.c_str(), mqttstatus.c_str(), true); // Publishing the Mqtt Connected Status to the Server
        signalstrength = "sig_strength " + deviceid + " " + String(dBmtoPercentage(WiFi.RSSI())) + "%"; // Creating a signal strenth payload
        client.publish(pub_topic.c_str(), signalstrength.c_str(), true); // Publishing the Signal Strength to the server
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
    }
  }
  else
  {
    Serial.println("Error in response");
  }
  http.end();  //Close connection
  delay(2000);  //GET Data at every 5 seconds
}
//======================================Initializing the task==============================================
void task(){
  runner.init();                                                   // Initializing the runner and enebling the task
  Serial.println("Initialized scheduler");
  runner.addTask(T1);  
  Serial.println("Added Task T1");
  T1.enable();
}
//===========================================calling task function=====================
void dev_status() {
  publish_dev_data(deviceid, signalstrength);
}
//==========================================publish device data================================================================
void publish_dev_data(String deviceid, String signalstrength)
{
  String pub_topic = "pub_" + String(deviceid) + "/smardentechresponsee/" + String(mq_user);
  Serial.println(pub_topic);
  signalstrength = "sig_strength " + deviceid + " " + String(dBmtoPercentage(WiFi.RSSI())) + "%"; // Creating a signal strenth payload
  Serial.print("Device Signal Strength is :");
  Serial.println(dBmtoPercentage(WiFi.RSSI()));
  client.publish(pub_topic.c_str(), signalstrength.c_str(), true); // Publishing the Signal Strength to the server
}
//====================================Publish Switch Data==============================================================================
void publish_sw_data(String deviceid, String Elementid, String Data)
{
  String pub_topic = "pub_" + String(deviceid) + "/smardentechresponsee/" + String(mq_user);
  String  pub_data = "smarden_dev_resp " + Elementid + " " + Data;
  client.publish(pub_topic.c_str(), pub_data.c_str(), true);
}
//====================================Publish Sensor Data==============================================================================
void publish_sn_data(String deviceid, String Elementid, String Data)
{
  String pub_topic = "pub_" + String(deviceid) + "/smardentechresponsee/" + String(mq_user);
  String  pub_data = "smarden_dev_sens " + Elementid + " " + Data;
  client.publish(pub_topic.c_str(), pub_data.c_str(), true);
}
//=========================Controling the devices==================================
void control(uint8_t pin, String rec_data, bool command, String com_data) {
  if (rec_data == com_data) {
    digitalWrite(pin, command);
  }
}
