# SmarDen.Tech Library Functions 
Here in this file, you'll find the functions that you can use from the smarden_tech.h file in your arduino sketch. The library folder in this repository contains all the necessary arduino libraries that you need to install. 
Here is the list of libraries that you need to install
- PubSubClient
- ESP8266WiFi
- TaskScheduler
- ArduinoJson
- ESP8266 HTTPClient 

## Here is the list of functions that you can use from the smarden_tech.h file in your sketch. 
- void wifi_connect(char* wifi_ssid, char* wifi_pass) 
- void mqtt_connect(String accesskey, String deviceid)
- void task()
- void dev_status()
- void publish_dev_data(String deviceid, String signalstrength)
- void publish_sw_data(String deviceid, String Elementid, String Data)
- void publish_sn_data(String deviceid, String Elementid, String Data)
- void control(uint8_t pin, String rec_data, bool command, String com_data)

### void wifi_connect(char* wifi_ssid, char* wifi_pass) 
This function is used to connect your device to wifi network, it takes two parameters;
- char* wifi_ssid - will be the ssid of your wifi router.
- char* wifi_pass - will be the password of your router. 

### void mqtt_connect(String accesskey, String deviceid)
This is the mqtt function and it will help your device to establish a MQTT connection with the SmarDen.tech IoT Cloud. When you register on the platfrom and you make an instance, you'll get one accesskey that you can copy from the profile menu. You need to define that access key in your program. 
Once you create a device on the the platform, you'll get a deviceID associated with that device, you'll also need that device ID to establish the mqtt connection to SmarDen.tech Iot Platform. 
- String accesskey - This is a string type variable containing your access key. 
- String deviceid - THis will be the device ID of this specific IoT device that you created in the platform.

