#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

#define TRIG 12
#define ECHO 14 
#define POWER 13

char ssid[] = "common44";       // Replace with WiFi SSID
char pass[] = "a1b2c3d4e5";    // Replace with WiFi Password
const char * mqtt_sever = "192.168.1.40";

float distance;
char cstr[16];

void setup()
{
//  Serial.begin(115200);

  // Wait for serial to initialize.
//  while(!Serial) { }

  pinMode(POWER, OUTPUT);
  pinMode(TRIG, OUTPUT); // Initializing Trigger Output and  Echo Input 
  pinMode(ECHO, INPUT_PULLUP);

  digitalWrite(POWER, HIGH);
  
  setup_wifi();

  client.setServer(mqtt_sever, 1883);
  client.setCallback(callback);
  reconnect();

   delay(1000);


  digitalWrite(TRIG, LOW); // Set the trigger pin to low for 2uS 
  delayMicroseconds(2); 
    
  digitalWrite(TRIG, HIGH); // Send a 10uS  high to trigger ranging 
  delayMicroseconds(20); 
  
  digitalWrite(TRIG,  LOW); // Send pin low again 
  distance = pulseIn(ECHO, HIGH, 26000); //  Read in times pulse 

  distance = distance/58;

  //Serial.print("Distance: ");

  //Serial.println(distance);

  if (!client.connected()) {
    reconnect();
  }

  //Convert from float to string
  snprintf(cstr, sizeof(cstr), "%f", distance);

  client.publish("IoT/water-tank-depth", cstr);

  delay(1000);

  //Serial.println("Going to sleep");
  ESP.deepSleep(10e6);
}

void loop()
{
//  if (!client.connected()) {
//    reconnect();
//  }
//  client.loop();
//
//  client.publish("test", itoa(distance, cstr, 10));
//
//  delay(500);
}

void reconnect() {
  while (!client.connected()) {
    if ( WiFi.status() == WL_CONNECTED ) {
      //Serial.println("Wifi connected");
    }
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

/************* Connect to WiFi ***********/
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
