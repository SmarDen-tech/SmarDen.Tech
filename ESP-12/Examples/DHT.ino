#include "smarden_tech.h"
//==================Enter the details according to your instance=================================================
String deviceid = "----------------------";                    // Enter the device ID
String accesskey = "---------------------------";               // Enter the Access Key
char* wifi_ssid = "---------------";                            // Enter the WiFi SSID
char* wifi_pass = "----------";                                 // Enter WiFi Password
#define DHTPIN D2
#define DHTTYPE DHT11
#define refresh_time 10000 
void callback(char* topic, byte* Payload, unsigned int length);
DHT dht(DHTPIN, DHTTYPE);
//========================Enter Element IDs=======================================================================
 String tempid = "--------------";
 String humid = "---------------";
//=============================Setup Part==========================================================================
void setup() {
  dht.begin();          
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
  temperature();
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
  publish_sn_data(deviceid,tempid,t); // Publish the temperature to the server
  publish_sn_data(deviceid,humid,h); // publish the Humidity to the server
}
