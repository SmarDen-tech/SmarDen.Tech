The communication protcol that we are using here is the MQTT Protocol and in the MQTT potocol there are two basic things - Publish and Subscribe. 

* Subscribing - When we subscribe to a topic that means we are listing to the topic and we will receive the data that we coming through that particular topic. In this case when we subscribe to the given topic, we will receive the data from the dashboard. you can choose the type of payload that you want to send to your IoT Device. 

* Publishing - When we want to send anything to the server we need to publish that data to a particular topic. For example is you are using any sensor with your IoT Hardware and you need to publish that data from the sensor to the dashboard, in that case you need to publish that data to a particular topic so that it will be reflected on the dashboard. 

# Publishing Data Format 

For Publishing the data the from the device to the platform, you'll need a Publsihing topic. In our Case the publishing Topic will be 

* Publishing Topic - smardentechresponse

You need to publish the data on this topic, you can also find this topic on the properties tab of any element.  

## Publishing the Data 

Whenever you'll receive a Callback or Payload for any switch, you need to update the state of the switch on the server as well, for this you need to publish the data to the dashboard. 

The Payload Format will be - 
* smarden_dev_resp(space)ElementID(Space)msg

For example if you're creating a toggle switch and you're sending "ON" as on payload, then you need to publish this: 
* smarden_dev_resp *element_id* ON as the payload to Publish Topic. 

## Publishing Sensor Data 

For publishing the sensor data you need to use this format: 
* smarden_dev_sens(space)DeviceID(Space)data

For example, if you're publishing the signal strength for a dedvice having Device ID as - "asifugib02iv9832g" the format will be 
* smarden_dev_sens asifugib02iv9832g true

## Publishing Signal Strength 

For publishing the Signal Strength you need to use this format: 
* sig_strength(space)DeviceID(Space)data

For example, if you're publishing the signal strength for a dedvice having Device ID as - "asifugib02iv9832g" the format will be 
* sig_strength asifugib02iv9832g 78%

## Publishing the Connection State 

The connection State is related to the device and to print the state of the device on the Dashboard the payload format will be: 

* dev_status(space)DeviceID(space)online/offline 

The online payload will be called as the mqttstatus in the example files and offline payload will be called as willmessage. 

For a device with the Device ID - "asifugib02iv9832g" the format will be
* mqttstatus : dev_status asifugib02iv9832g online 
* willmessage : dev_status asifugib02iv9832g offline 

