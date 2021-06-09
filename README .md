# SmarDen.Tech

## SmarDen For Developers 
Smarden.tech is a platform for Internet of Things and it is for the developers and the IoT enthusiasts who are working in the field of Internet of Things and want a complete one stop solution for the MQTT, Cloud, Back-end and Dashboard.

 For Any Internet of Things device it is important to have three basic things lined up. 
 - Hardware: The Hardware is the basic block for any IoT device, the device can be of two type, first one is where you can perfrom the operations and second one is from where you can get the data. consider Actuators and sensors are two different things that we need along with the microcontroller for making any IoT Hardware, so that we can control the device and get the data from the device remotely from anywhere in the world. 

 - Cloud: Then after deploying the hardware, our goal for the device is to control and monitor it remotely with our mobile application from any any part of the world. To achive this we will be needing a complete cloud infrastructure and we need to connect to connect our device and the Dashboard to the same server so that the communcation can happen, in the case of IoT we use the protocol of MQTT for this complete infrastructure and we use a realtime data base as the data storage unit. 

 - Application: After the data is sent to the cloud we need a front-end application to perform operations on the device and to motinor the data that the device is sharing. Then we need to design a particulat web-app or mobile app to do all these things. 

## What is SmarDen.tech? 

SmarDen for Developers is an online IoT platform designed for the Enthusiasts like you. It gives you complete freedom to design your projects from scratch, you take care of the devices and we’ll take care of the backend, cloud and dashboard.
As an electronic hobbyist, focusing on the device level and tinkering with the device is more important then to worry about the software. Develop your IoT devices and focus on the Hardware, rest all is taken care by us.
Integrating your end devices with the cloud and the making the front-end to access the data was never been much easier, With our platform it will take you less than 5 mins to onboard your device to cloud. 
Accessing the meaningful information is always the main concern, with our user friendly dashboard and mobile application, monitor and control your devices in a blink of an eye from any part of the world. 

### Shifting to SmarDen.tech 
1. <b>Registration: </b>The first step is to create your user account, where you will be asked to enter your name, email id and phone number, and please enter the correct email id, so that you can get the password to login and can verify your account.
  

2. <b>Login: </b>Here you can login into the dashboard with your correct Username and Password.
   

3.  <b>Dashboard: </b>On the Dashboard you will get the section of notifications, instance creation, instance status and definitions.
       - Create Instance: Here in create instance you have to enter your desired instance name with password. The instance creation success or failure message will be shown in Instance Status and one user can create only one instance.
       - definition: This section contains all the important infromation about your instance like the hostname, port numbers, etc. all the things that you'll be needing while programming the device. 
 
4. <b>Components:</b>Here you'll find different components like generate topics, list of topics, instance status and definition. 
      - Generate Topics: Here you need to type your existing instance name and click on 'genetate topic' button, then your topic will be generated and you can use this topic with you're device for pub/sub actions. 
      - Instance status: This will tell you the current status of your instance. 
      - Definition: Here you can check the port numbers, host and other important information. 
      
## Connection Panel
The Connection panel is provided to test the working of your current instance with the end-device, here you will find the fields like Host, Port, Username, Password, Keep Alive, Timeout, Last will Topic, Last will message.

 Here the Host, Port and Path will be provided to you and you cannot change that meanwhile you can add your "Client ID", "Topic", "Username", "Password" and other MQTT Details.
 - Client ID: Here you need to define the client ID of the Instance and it has to be Unique and does not match with any other client ID. 
 - Username: Here you need to enter the Username of the instance & it will act as a username for the MQTT Server. 
 - password: Here you need to enter the password for the instance and it will be the password for the MQTT broker.
 - Last Will Topic: The Last Will Topic is the topic where the device will publish its 'Will Message' when the connection breaks from the MQTT Server, Using the Last Will Topic and Last Will Message will give you the power to monitor the device connection status. 
 - QoS: Quality of Servive, MQTT Currently supports 3 level of QoS for its messages i.e.  QoS 0, QoS 1 and QoS 2, but the end devices which works on PubSubClient library currently support only QoS 0 & 1. 
 - Retain: Here you need to define the Retain Status of your messages, it can either be true or false. True in case you want to retain the Last will message and false in case if you don't want to retain any message. 
 - Last Will Message: The Last will Message is the Message published by the device on the Last Will Topic when the connection is broken from the MQTT Broker. For Example if you use "Device Offline" as a Last will message on "LastwillTopic" Topic then when the connection is broken from the MQTT broker, then the device will publish this message on LWT automatically.
 
### Subscribe Window
Here this subscribe window will give you the option to subscribe on a particular topic, set the Quality of Service for the subscibed topic and Monitor the messages coming on that particular topic. Here with this functionality you can monitor the data that is generated by your IoT device. In this case you need to publish the data on a particular topic from the device and then from this window you need to subscribe on the same topic and hence you'll be able to monitor the data coming from the device.

### Publish Window
The Publish window is used to publish the messages on a particular topic and then from here you can select the QoS of published messages and if you want to retain the messgaes you can set the retain value as true. Here you need to define the same topic on the device & your device needs to subscribe on the same topic so when you'll be publishing the messages from this window your device will receive all those messages and you can perform various actions on that device. 

      
      
