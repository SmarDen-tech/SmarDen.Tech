This Guide will help you in adding the feature of Signal Strenth in your device, this will reflect the wifi signal strength to the SmarDen Dashboard.

## Signal Strength

* RSSI - Received Signal Strength Indicator, It shows the power present in the received radio signal. 

Here we will use the function to get the signal strength of connected wifi network and then we will publish the data to the SmarDen Dashboard. RSSI or this signal value is measured in decibels from 0 (zero) to -120 (minus 120). The closer the value to 0 (zero), the stronger the signal will be. 

* dBm - RSSI and dBm may have different units of measurement but both of them represent the same thing. They both represent the signal strength of a network. The difference between the two is that RSSI is a relative index while dBm is considered as an absolute number representing power levels in mW (milliwatts). Hence, the closer to 0dBm, the better the signal is.

For this, you need to add a few things in your Program. 

```
const int RSSI_MAX = 0; // define maximum strength of signal in dBm
const int RSSI_MIN = -120; // define minimum strength of signal in dBm
#define refresh_time 10000  //define the signal strength refresh time 
```

Now we need to define a funciton that will convert the dBm to Percentage and will return the signal strength in percentage. 
```
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
}//dBmtoPercentage
```


While connecting to the wifi we also need a function that will return the signal strength, so in the wifi part we'll add this. 
```
Serial.print(WiFi.RSSI());//Signal strength in dBm
  Serial.print("dBm (");
  Serial.print(dBmtoPercentage(WiFi.RSSI()));//Signal strength in %
  Serial.print("% )");
  ```
  Here this WiFi.RSSI() Funciton will give the signal Strength in dBm and then with the help of dBmtopercentage() funciton we'll convert it to percentage. 

So far we've received the signal strength in dBm and converted it to the Percentage, now we need to publish this data to the server. 

## Publishing Signal Strength to the Server 

For publishing the Data to the SmarDen Dashboard you'll need to follow the payload format and then publish the data to a particular topic that is given to you. 

### Payload Format for Signal Strength 
* sig_strength(space)DeviceID(space)Data
* For eg. sig_strength p10M9pA8XBGsSiKow 80%

Here the keyword is sig_strength, now we need to put this complete data into the given format and we'll use String Concatenation here. Right after getting the signal strength use this snippet in your code for combining all the strings together to make a complete payload. 

```
signalstrength = "sig_strength " + DeviceID + " " + String(dBmtoPercentage(WiFi.RSSI())) + "%";

```
Now we need to publish this payload on the particular topic that you'll get from the admin panel and to publish this data use this funtion inside the reconnect function just after the Mqtt connetion part. 
```
client.publish(Pub_topic.c_str(), signalstrength.c_str(), false);
```
This will publish the data on the server and you'll be able to see the signal strength on the dashboard. 


# Creating a task for publishing signal strength at a specific interval 

We also need to create a task so that we can update the signal strength after a particular interval of time, for this we'll be using "TaskScheduler" Library. You can download this library from the Arduino Library Manager. 

First we need to include the library 
```
#include<TaskScheduler.h>
```
Now we need to declare a function and create a task.
Here declared a function by the name of dev_status(); 
```
void dev_status();
Task T1(10000, TASK_FOREVER, &dev_status);
Scheduler runner; // create an object for accessing the member function of this library.
```
In the Setup Part, We need to initialize the runner and enable the task.
```
runner.init(); // Initializing the runner and enebling the task 
Serial.println("Initialized scheduler");
runner.addTask(T1);
Serial.println("Added T1");
T1.enable();
```

In the Loop Part, we need to use this funcition, 
```
  runner.execute();
```
Now finally we need to define the dev_status() Function. 

```
void dev_status() {
  delay(refresh_time);
  Serial.print("Publishing Device status and Signal Strength");
  client.publish(Pub_topic.c_str(), signalstrength.c_str(), false);

}
```
Here you need to update the refresh time in Miliseconds and after that particular interval of time, this will publish the signal strength to the SmarDen Dashboard. 

  
