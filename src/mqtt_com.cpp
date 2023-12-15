#include <Arduino.h>
#include <config.h>
#include <PubSubClient.h>
#include <wifi_com.h>
#include <utils.h>
#include <scroller.h>

WiFiClient espClient;
PubSubClient client(espClient);

String device_title;
String mqtt_server;
String mqtt_user;
String mqtt_password;
String mqtt_topic_base;
String mqtt_topic_io;
String mqtt_topic_out;
String mqtt_topic_ping;

bool isMQTTConnected()
{
  return client.connected();
}

String getDeviceTitle()
{
  return device_title;
}

String getMQTTServer()
{
  return mqtt_server;
}

String getMQTTUser()
{
  return mqtt_user;
}

String getMQTTPassword()
{
  return mqtt_password;
}

String getMQTTTopicBase()
{
  return mqtt_topic_base;
}

int lastConnectionAttempt = 0;

void publishEvent(String event_name, String payload)
{
  String topic;
  topic = mqtt_topic_out + "/" + event_name;
  if (client.connected()) {
   Serial.print("Publishing to ");
   Serial.print(topic);
   Serial.print(" ");
   Serial.println(payload);
   client.publish(topic.c_str(), payload.c_str());
  } else {
   Serial.print("MQTT disconnected. Cannot publish to ");
   Serial.print(topic);
   Serial.print(" ");
   Serial.println(payload);   
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  String strTopic = String(topic);
  String strPayload = "";
  Serial.print("Message arrived [");
  Serial.print(strTopic);
  Serial.print("] ");
  for (uint16_t i = 0; i < length; i++)
  {
    strPayload += (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if (strTopic.endsWith("io/message/set")) {
    setMessage(strPayload);
  }
  if (strTopic.endsWith("io/default_message0/set")) {
    setDefaultMessage0(strPayload);
  }  
  if (strTopic.endsWith("io/default_message1/set")) {
    setDefaultMessage1(strPayload);
  }    
  if (strTopic.endsWith("io/effect/set")) {
    setEffect(strPayload);
  }  
  if (strTopic.endsWith("io/delay/set")) {
    setDelay(strPayload);
  }    
}

void reconnectMQTT()
{
  if (mqtt_server.length() == 0)
  {
    Serial.print("MQTT server is not set...");
    return;
  }

  // Loop until we're reconnected
  while (!client.connected())
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "Protoype-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    client.setServer(mqtt_server.c_str(), 1883);
    if (client.connect(clientId.c_str(), mqtt_user.c_str(), mqtt_password.c_str()))
    {
      Serial.println("connected");
      client.setCallback(callback);
      // Once connected, publish an announcement...
      client.publish(mqtt_topic_out.c_str(), "hello world");
      publishEvent("message", getMessage());
      publishEvent("effect", getEffect());
      publishEvent("delay", getDelay());
      publishEvent("default_message0", "");
      publishEvent("default_message1", "");
      //setMessage("MQTT connected!");

      // ... and resubscribe
      client.subscribe(mqtt_topic_io.c_str());
      digitalWrite(LED_BUILTIN, LOW);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      return;
    }
  }
}

void handleMQTT()
{
  if (!client.connected())
  {
    if ((round(millis() / 1000) - lastConnectionAttempt) > 10)
    {
      lastConnectionAttempt = round(millis() / 1000);
      Serial.println("Reconnecting");
      reconnectMQTT();
    }
  }
  else
  {
    client.loop();
  }
}

void startMQTT()
{

  mqtt_server = readStringEEPROM(EEPROM_MQTT_SERVER);

  device_title = readStringEEPROM(EEPROM_MQTT_DEVICE_TITLE);

  mqtt_user = readStringEEPROM(EEPROM_MQTT_USER);
  mqtt_password = readStringEEPROM(EEPROM_MQTT_PASSWORD);
  mqtt_topic_base = readStringEEPROM(EEPROM_MQTT_BASE);

  if (mqtt_topic_base == "")
  {
    mqtt_topic_base = "devices/";
  }

  mqtt_topic_io = mqtt_topic_base + device_title + "/io/#";
  mqtt_topic_out = mqtt_topic_base + device_title + "/io";
  mqtt_topic_ping = mqtt_topic_base + device_title + "/ping";

  if (mqtt_server.length() > 0)
  {
    client.setServer(mqtt_server.c_str(), 1883);
    client.setCallback(callback);
  }
}
