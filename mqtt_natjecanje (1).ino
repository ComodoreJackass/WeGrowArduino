#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;
const char* mqttServer = "m24.cloudmqtt.com";
const int mqttPort = 10991;
const char* mqttUser = "id usera";
const char* mqttPassword = "sifra usera";
const char* ssid = "ssid rutera";
const char* password = "sifra rutera";
WiFiClient espClient;
PubSubClient client(espClient);
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D4
#define VccPin D6
OneWire oneWire(ONE_WIRE_BUS);
unsigned int ocitanje;
DallasTemperature sensors(&oneWire);
void setup() {
  pinMode(VccPin, OUTPUT);
  digitalWrite(VccPin, LOW);
  sensors.begin();
 unsigned status;
    status = bme.begin(0x76);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    if (client.connect("ESP8266Client",mqttUser,mqttPassword)) {
    }
  }
}
void loop() {
digitalWrite(VccPin, HIGH);
delay(50);
ocitanje = analogRead(A0);
digitalWrite(VccPin, LOW);
if(ocitanje<1002){
ocitanje = map(ocitanje, 400, 1002, 100, 0);
ocitanje = constrain(ocitanje, 0, 100);
client.publish("s1/vltlo", String(ocitanje).c_str());
}else {
  client.publish("s1/vltlo", 0);
}
sensors.requestTemperatures();
client.publish("s1/tmpzrak", String(bme.readTemperature()).c_str());
client.publish("s1/vlzrak", String(bme.readHumidity()).c_str());
if(sensors.getTempCByIndex(0) != DEVICE_DISCONNECTED_C)
{
client.publish("s1/tmptlo", String(sensors.getTempCByIndex(0)).c_str());
}
ESP.deepSleep(3600000000,WAKE_RF_DISABLED);
delay(100);
}