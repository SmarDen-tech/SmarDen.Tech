#include "smarden_tech.h"
//==================Enter the details according to your instance=================================================
String deviceid = "-----------------";          // Enter the device ID
String accesskey = "----------------";          // Enter the Access Key
char* wifi_ssid = "-------------";              // Enter the WiFi SSID
char* wifi_pass = "----------------";           // Enter WiFi Password
#define LED D4
void callback(char* topic, byte* Payload, unsigned int length);
//========================Enter Element IDs=======================================================================
 String ledid = "-----------------" ; // Enter the Element ID of element that you created 
//=============================Setup Part==========================================================================
void setup() {
  pinMode(LED, OUTPUT);           
  Serial.begin(115200);
  wifi_connect(wifi_ssid, wifi_pass); //call this function to connect to your WiFi
  mqtt_connect(accesskey,deviceid);   //call this function to connect to Smarden.tech MQTT server   
  task();                             //start the task funtion to get the live device status 
  client.setCallback(callback);       //This "callback" function will be called everytime you receive data from the smarden.tech server
}
//=============================Loop Part============================================================================
void loop() {
  runner.execute();
  if (!client.connected()) {              // if the client is disconnected, try to connect again to the smarden.tech cloud
    mqtt_connect(accesskey,deviceid); 
  }
  client.loop();
}
//================================MQTT Callback Function=============================================================
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived");
  char sub[1000];
  int c = 0;
  char* payl = (char *)payload;
  while (c < length) {
    sub[c] = payl[c];
    c++;}
  sub[c] = '\0';
  Serial.println(sub); 
  String msg = sub;                               //receiving the payloads from the server
  control(D4,msg,1,"on");                        //Call this function to control the GPIO pins on the device 
  publish_sw_data(deviceid, ledid, msg);         //Publishing the switch state to the server
}
