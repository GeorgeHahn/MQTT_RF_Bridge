#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>

WiFiClient wclient;

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte server[] = { 192, 168, 0, 1 }; // Server IP address

#define       WIFI_SSID         "WIFI SSID"
#define       WIFI_PWD          "WIFI PASSWORD"

#define       MQTT_CLIENTNAME   "CC3200LPD"
#define       MQTT_USERNAME     "MQTT USERNAME"
#define       MQTT_PASSWORD     "MQTT PASSWORD"
#define       MQTT_TOPIC        "MQTT TOPIC"

PubSubClient client(server, 1883, callback, wclient);

// Long:  640us
// Short: 140us
// Sync: 5980us

#define longdel 2630
#define shortdel 575
#define syncdel 24578

inline void longdelay()
{
  int us = longdel;
  for(int i = 0; i < us; i++) { }
}

inline void shortdelay()
{
  int us = shortdel;
  for(int i = 0; i < us; i++) { }
}

inline void sendsync() {
  digitalWrite(pin, LOW); // Pin is inverted
  shortdelay();
  digitalWrite(pin, HIGH);
  int us = syncdel;
  for(int i = 0; i < us; i++) { }
}

void writePacket(byte* payload, unsigned int length) { 
  for(byte i = 0; i < length; i++)
  {
    if(payload[i] == '0')
    {
      digitalWrite(pin, LOW); // Pin is inverted
      shortdelay();
      digitalWrite(pin, HIGH);
      longdelay();
      digitalWrite(pin, LOW); // Pin is inverted
      shortdelay();
      digitalWrite(pin, HIGH);
      longdelay();
    }
    else if(payload[i] == '1')
    {      
      digitalWrite(pin, LOW); // Pin is inverted
      longdelay();
      digitalWrite(pin, HIGH);
      shortdelay();
      digitalWrite(pin, LOW); // Pin is inverted
      longdelay();
      digitalWrite(pin, HIGH);
      shortdelay();
    }
    else if(payload[i] == 'F')
    {
      digitalWrite(pin, LOW);
      shortdelay();
      digitalWrite(pin, HIGH);
      longdelay();
      digitalWrite(pin, LOW);
      longdelay();
      digitalWrite(pin, HIGH);
      shortdelay();
    }
  }
  
  // Send sync
  sendsync();
}

void callback(char* inTopic, byte* payload, unsigned int length){
  Serial.println("Got RF data");
  Serial.write(payload, length);
  Serial.println();
  
  digitalWrite(10, HIGH);
  for(int i = 0; i < 10; i++) {
    writePacket(payload, length);
  }
  digitalWrite(10, LOW);
  
  Serial.println("RF data sent");
}

void setup()
{
  Serial.begin(115200);
 
  pinMode(pin, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  digitalWrite(pin, LOW);
  digitalWrite(10, LOW);
  
  Serial.println("Start WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  while(WiFi.localIP() == INADDR_NONE) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("");
}

void loop()
{
  if(!client.loop())
  {
    digitalWrite(9, HIGH);
    Serial.println("Connecting to MQTT broker");
    if (client.connect(MQTT_CLIENTNAME, MQTT_USERNAME, MQTT_PASSWORD)) {
      client.subscribe(MQTT_TOPIC);
    }
    else
      Serial.println("Failed to connect to MQTT broker");
      
    digitalWrite(9, LOW);
  }
  else
    digitalWrite(9, HIGH);
}

