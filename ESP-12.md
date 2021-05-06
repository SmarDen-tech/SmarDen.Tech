# ESP-12 Boards

The ESP-12 board is a widely used wifi based micro-controller from Espressif Systems. This micro-controller is the heart of IoT based projects and it is widely used by the Home Automation industry. Here you'll find the resources for the ESP-12 boards so that you can make the hardware based on these boards and integrate it with SmarDen Dashboard. We're using Arduino IDE here and this guide will help you to get started with the ESP12 boards with Arduino IDE and integrate it with the SmarDen Dashboard. 

## Libraries used in this project 
1. PubSubClient - The PubSubClient is used for all the mqtt connections and data transmission over MQTT protocol. 
2. ESP8266WiFi - This library is used to manage all the WiFi related tasks in the Esp8266 boards, i.e. scanning for available wifi networks, connecting to Wifi network, disconnecting, etc. 


Here for your reference we are creating a few devices like Light, Fan, Switch, Temperature Sensor. 

To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

Here is this repository you'll find different sketches for different devices, download those sketches, use your Instance Credentials and you're good to go with your IoT Hardware. 

## Skectch Explanation 
```

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
```
While starting our Arduino Sketch for out IoT device, these are the basic two libraries that we require, you can multiple libraries for multiple purposes but for these example sketches these two libraries are enough. 
```
const char* ssid = "........";
const char* password = "........";
const char* mqtt_server = "login.smarden.tech";
```
- **ssid** - Here you'll provide the SSID of your existing WiFi network to which you want to connect your IoT Hardware
- **password** - Here you've to enter the password for your wifi network.
- **mqtt_server** - Here you don't need to change this field as this is the address of your Mqtt broker. 
> login.smarden.tech is the your mqtt server address. 

```
WiFiClient espClient;
PubSubClient client(espClient);
```
Here we need to create the object so that we can access the fuctions from these two libraries. 

```
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
}
```
setup_wifi() is a function that we've created to establish the wifi network and to connect our IoT hardware to the wifi netowrk.

```
Serial.print();
Serial.print("Connecting to ");
Serial.println(ssid);
``` 
This command is used to print the data on the serial monitor so that we can debug our skectches and we can focus on what is actually going on inside the device.
We've printed "Connecting to *ssid* " and this will show your ssid that you've written above. 

```
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
``` 
WiFi.mode(WIFI_STA); function is used to assign the station mode to the wifi network.

WiFi.begin(ssid, password); function is used to connect to the user specified SSID with with specified PASSWORD.
```
while (WiFi.status() != WL_CONNECTED) {
 delay(500)
 Serial.print(".");
  }
  ```
This is the *while loop* and here till the wifi connection is not established successfully we're printing "." after every 500ms. 

WiFi.status() is a fucntion which gives the current wifi status of the device and here we've used the logic - till the wifi status is not connected, print a "." after every 500ms. 

After the WiFi connection is established, the program will come out of the while loop and print the basic information like the IP address and will show a message "WiFi Connected" on the serial monitor. 

