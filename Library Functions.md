# SmarDen.Tech Library Functions 
Here in this file, you'll find the functions that you can use from the smarden_tech.h file in your arduino sketch. The library folder in this repository contains all the necessary arduino libraries that you need to install. 
Here is the list of libraries that you need to install
- PubSubClient - https://github.com/knolleary/pubsubclient
- ESP8266WiFi - Included by default when you install ESP8266 Boards in Arduino IDE
- TaskScheduler - https://github.com/arkhipenko/TaskScheduler
- ArduinoJson - https://github.com/bblanchon/ArduinoJson
- ESP8266 HTTPClient - https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPClient 

## Here is the list of functions that you can use from the smarden_tech.h file in your sketch. 
- void wifi_connect(char* wifi_ssid, char* wifi_pass) 
- void mqtt_connect(String accesskey, String deviceid)
- void task()
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

### void task()
Task is the function that you need to call in the setup part of your arduino sketch, this function will initialize the schedular and you can define the refresh time in miliseconds by yourself in the smarden_tech.h file , this will publish the current state and the signal strength of your IoT device on the platform in every 10 Seconds and will help you in real time device monitoring.

### void publish_sw_data(String deviceid, String Elementid, String Data)
This function will be used to publish the switch state to the server, if you're using the "toggle button" element in the Smarden.tech IoT platform, you must provide a callback to the platform after every toggle or when any switch state is changed.This will help the platfrom to get updated with the exact current state of any GPIO pin on your IoT device. 
- String deviceid - provide the device ID of the particular device 
- String elementid - here you need to provide the Element ID for which you're publishing the data
- String Data - here you need to publish the data that you need to send to the platform 

### void publish_sn_data(String deviceid, String Elementid, String Data)
This function will be used to publish the sensor data to the server, if you're using the sensor elements in the Smarden.tech IoT platform, you need to publish the data on the server so that you can monitor that data from the dashboard.This will help the platfrom to get updated with the exact sensor data coming from your IoT device. 
- String deviceid - provide the device ID of the particular device 
- String elementid - here you need to provide the element ID for which you're publishing the data
- String Data - here you need to publish the data that you need to send to the platform 

### void control(uint8_t pin, String rec_data, bool command, String com_data) 
When any data is received from the dashboard, callback function is called, this callback function helps to perform the actions on the device. control function is used to control the device according to the payloads that you have defined on the platfom while creating elements. 
- uint8_t pin - This is the pin number on which you want to perform action 
- String rec_data - There you need to pass the message that you received from the IoT platfrom 
- bool command - 0 or 1, to turn the pin LOW or HIGH
- String com_data - Here you need to pass the string that you defined as the payload 

In this function, the rec_data will be compared with the com_data and if both the string matches, your command will be performed on the defined pin number. 
