There are some additional features that you can also integrate within your IoT hardware that will give you a better control over the devices and you can have more data coming out from your IoT Hardware. 

## Device Connection Status 

The device connection status is an important parameter when it comes to monitoring or controlling any hardware. You can now implement a new feature that will give you the Device Connection Status whenever the device is connected/disconnected to the Internet, this will help you in tracking the real time connection status of the device. 

### How it Works? 
Assume that you are making any IoT device that you need to control and monitor with the SmarDen Dashboard. Now when the device is connected to our servers, it will send the connection status on a specific topic that you'll get on your admin panel over the MQTT protocol.

Once we receive the data or the connection status from the device, we will redirect the connection status on your dashboard specific to that device and it will show you the real time connection status.
 
In case if the device is disconnected to the Internet, there is a Last Will Message that we will receive from the device which ensures that the device is no longer connected to the Internet. 
```
String Pub_topic = ".....................";
String willmessage = "dev_status " + DeviceID + " offline"; //do not change 
String mqttstatus = "dev_status " + DeviceID + " online";  // do not change
```
These are the few things that you need to define in your program for maintaining the connection status. 
Let's focus on each of them. 
- **Pub_topic** : This is going to the topic where you'll be pusblishing all the data from device to the server. This is going to be the one end from which the device will communicate to the Server. The user will use this topic for publishing the Last Will Message and also for publishing the connection status.   
- **willmessage** : Will message is stored by the mqtt and when the client is disconnected from the MQTT broker it will be pubished on the "Pub_topic" automatically, which will indicate that the device is now disconnected from the Internet. THe format of the payload must be like this "deviceID/Disconnected", here the device id will help us locating the exact device and the Disconnected message will tell the state. 
- **mqttstatus** : Mqtt status is another variable that we are using when the MQTT connection is successful. The status message will also be published on the "Pub_topic". Once the device is connected we'll push this payload to the server. 

**Changes that you need to make in the Reconnect Fucntion** 
```
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),username,mqtt_pass,Pub_topic.c_str(), 1, 0, willmessage.c_str())) {
      Serial.println("connected");
      client.subscribe(Topic);
      client.publish(Pub_topic.c_str(), mqttstatus.c_str(), false);
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
While using the function "client.connect()" we are adding some more parameters along with the client ID, Username & password. 
```
(client.connect(clientId.c_str(),username,mqtt_pass,Pub_topic, 1, 0, willmessage))
```
- **clientID**    :  The client ID to use when connecting to the server(Const Char[]).
- **username**    :  The username to use. If NULL, no username or password is used(Const Char[]). 
- **mqtt_pass**   :  The password to use. If NULL, no password is used(Const Char[]).
- **Pub_topic**   :  The topic to be used by the will message(Const Char[]).
- **willQoS**     :  0,1 or 2 - the quality of service to be used by the will message.
- **willRetain**  :  Whether the will should be published with the retain flag(0 or 1 / boolean).
- **willmessage** :  The payload of the will message(Const Char[]).

After successfully connected to the device, the Last will message will be stored and when the device is disconnected it will be automatically published on the Pub_topic. 
Now that we have connected to the Server, we need to tell the connection status and need to maintain it on the dashboard. 

For this we are publishing the data on the Pub_topic in a particular format. 
```
client.publish(Pub_topic,(char*)mqttstatus.c_str(), true);
```
- **Pub_topic**  : The publishing topic on which the message should be published. 
- **mqttstatus** : The device connection status that we are publishing on the Pub_topic, it has to be in the same format as "deviceID/connected". 
- **retained**   : Whether the message should be published with the retain flag(0 or 1 / boolean).


After making these certain changes in the sketch you'll be able to maintain the state of your IoT Device. 


