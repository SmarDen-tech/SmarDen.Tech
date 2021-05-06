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
const char* username = ".........";
const char* mqtt_pass = "........"; 
char* Topic = ".......";
```
- **ssid** - Here you'll provide the SSID of your existing WiFi network to which you want to connect your IoT Hardware
- **password** - Here you've to enter the password for your wifi network.
- **mqtt_server** - Here you don't need to change this field as this is the address of your Mqtt broker. 
> login.smarden.tech is the your mqtt server address. 
- **Username** - This is the your Instance username that you've created on the SmarDen.Tech Platfrom. 
- **mqtt_pass** - This is your mqtt instance password. 
- **Topic** - The topic that you've created while adding any UI Element on the SmarDen Dashboard. 

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

After connecting to the WiFi we need to use two basic functions of the PubSubClient Library, one is "callback" which is used to receive the values from a subscribed topic and another one is "reconnect" which is used to maintain the MQTT connection.

### Callback Function 

```
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
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    Serial.println("LED On");   // but actually the LED is on; this is becauseit is active low on the ESP-01)
  } else if (msg == "off") {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    Serial.println("LED OFF");
  }
```

Here we have 3 different things as the parameters - topic, payload and length. 
- Topic- here we get the topic on which the data is received. 
- Payload - It is defined as the data that is send by the Publisher. 
- Length - It defines the length of the payload.

**Step 1 :** Display the topic on which the message has arrived. 
**Step 2 :** Convert the payload from Char array to String, so that the publisher can send the data in the form of string and we can receive it. 
**Step 3 :** Using if else conditions to perform actions on different payloads. 

Here in this example sketch we are using the builtin LED of the Nodemcu and we are controling that LED with our Dashboard over MQTT protocol. 

### Reconnect Function

```
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),username,mqtt_pass)) {
      Serial.println("connected");
      client.subscribe(Topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
```
In the reconnect funciton we will first check if the client is connected to the mqtt server using the while loop. 
```
while (!client.connected())
```
If the client is not conncted, then fist we will create a random client ID and after that we will use 
```
(client.connect(clientId.c_str(),username,mqtt_pass))
```
this function to connect to the Mqtt server with user specific username and password. 

Now if the connection is successful then we need to subscribe to the topic using 
```
client.subscribe(Topic);
```
If by any chance the connection is unsuccessful then we will print the error report on the serial monitor and will try to reconnect in every 5 seconds. 

### The Setup Part 
There are two different part of amy arduino sketch, first one is Setup part and the second one is the loop part. The things that we only need to define once in our sketch must be written under the setup part and the things that we need to keep in loop throughout our program will go under the loop part.
```
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);
}
```
First we need to define the pins that we are using by using the pinMode() function. Here we've use the builtin LED of the nodemcu and using it as the OUTPUT pin. 

Then we need to call the setup_wifi(); fucntion which will call help the device to connect to the available wifi network. 

> client.setserver(mqtt_server,1884); - Here we are defining the server address and the port number. This has be the same as written here. 
> client.setcallback(callback); - Here we are declaring the Callback function, so everytime we receive any data over mqtt "callback" function will be called. > 

### The Loop Part
```
   if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
```
Here we only need to check if the client is connected to the mqtt server or not. If not, then we are calling the reconnect function to maintain the connection. 


This was the basic sketch and its explanation on how we can integrate our devices to the SmarDen.Tech Platform.
