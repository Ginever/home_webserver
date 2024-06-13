#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Wifi and MQTT server infomation
#include "config.h"

WiFiClient espClient;
PubSubClient client(espClient);

//Define GPIO pins numbers 
#define TRIG 12
#define ECHO 14 
#define POWER 13

float distance;
char cstr[16];

void setup()
{
  //  Serial.begin(115200);

  // Wait for serial to initialize.
  //  while(!Serial) { }

  //Power pin is required as I am to lazy to get a wire and connect Vcc to ultrasound sensor power
  pinMode(POWER, OUTPUT);

  // Initializing Trigger Output and Echo Input
  pinMode(TRIG, OUTPUT);  
  pinMode(ECHO, INPUT_PULLUP);

  digitalWrite(POWER, HIGH);
  
  setup_wifi();

  //connect to MQTT broker
  client.setServer(mqtt_sever, 1883);
  client.setCallback(callback);
  reconnect();


  //? is this required
  delay(1000);

  //Trigger and read ultrasonic sensor
  digitalWrite(TRIG, LOW); // Set the trigger pin to low for 2uS 
  delayMicroseconds(2); 
    
  digitalWrite(TRIG, HIGH); // Send a 10uS  high to trigger ranging 
  delayMicroseconds(20); 
  
  digitalWrite(TRIG,  LOW); // Send pin low again 
  distance = pulseIn(ECHO, HIGH, 26000); //  Read in times pulse 

  distance = distance/58;

  //Serial.print("Distance: ");

  //Serial.println(distance);


  //Make sure MQTT client is still connected
  if (!client.connected()) {
    reconnect();
  }

  //Convert from float to string
  snprintf(cstr, sizeof(cstr), "%f", distance);

  //Publish data to MQTT broker
  client.publish("IoT/water-tank-depth", cstr);

  //Leave time for MQTT message to send before shuting down board
  delay(1000);

  //Serial.println("Going to sleep");
  ESP.deepSleep(10e6);
}

//Void loop is not requied as I am using deep sleep to save power
void loop()
{

}

// ____ RECONNECT ____
/*
 * @brief A function to ensure that WiFi is connected and that the device is connected to a MQTT broker
 * @return None
*/
void reconnect() {
  while (!client.connected()) {
    //reconnect to WiFi if required
    if ( WiFi.status() != WL_CONNECTED ) {
      setup_wifi();
    }

    //Try and connect to MQTT broker
    //Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      //Serial.println("connected");
    } else {
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// _____ CALLBACK _____
/*
 * @brief a callback that runs if a message is published on a subscribed MQTT channel
 * @param topic the topic of the published message
 * @param message the message received from MQTT broker
 * @param length the length of the message
 * @returns None
*/
void callback(String topic, byte* message, unsigned int length) {
  //Serial.print("Message arrived on topic: ");
  //Serial.print(topic);
  //Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    //Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  //Serial.println();
}

// _____ SETUP_WIFI _____
/*
 * @brief connects to the local wifi network
 * @return None 
 */
void setup_wifi() {
  delay(10);
  //Serial.print("\nConnecting to ");
  //Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  randomSeed(micros());
  //Serial.println("\nWiFi connected\nIP address: ");
  //Serial.println(WiFi.localIP());
  delay(1000);
}
