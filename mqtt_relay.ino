#include <PubSubClient.h>
#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char* mqtt_server = "***.***.***.***"; // IP ADDRESS BROKER MQTT
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xEA };
String _topic;
String _payload;
const int switch1 = 3; // SWITCH 1 ON ARDUINO PIN 3
const int switch2 = 4; // SWITCH 2 ON ARDUINO PIN 4
const int sensorTemperatura = 2; // DS18B20 SENSOR ON ARDUINO PIN 2, MAKE SURE TO PULL UP WITH A 4.7K RESISTOR

OneWire oneWire(sensorTemperatura);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;

EthernetClient ethClient;
PubSubClient client(ethClient);

void callback(char* topic, byte* payload, unsigned int length) {
  _topic = String((char*)topic);
  _payload = "";
  
  for(int i=0;i<length;i++){
    //Serial.print((char)payload[i]);
    _payload += (char)payload[i];    
  }
  
  // CONVIERTE PAYLOAD A STRING
  payload[length] = '\0';
  String strPayload = String((char*)payload);
  
  // CONDICIONAL SWITCHES
  if(_topic == "/switch/1"){
    Serial.println("Topic OK Switch 1");
    if(_payload == "ON"){
      digitalWrite(switch1,LOW);
      client.publish("/switch/1/state","1");
      Serial.println("Switch 1 Encendido");
    } else if (_payload == "OFF"){
      digitalWrite(switch1,HIGH);
      client.publish("/switch/1/state","0");
      Serial.println("Switch 1 Apagado");
    }
  }
  if(_topic == "/switch/2"){
    Serial.println("Topic OK");
    if(_payload == "ON"){
      digitalWrite(switch2,LOW);
      client.publish("/switch/2/state","1");
      Serial.println("Switch 2 Encendido");
    } else if (_payload == "OFF"){
      digitalWrite(switch2,HIGH);
      client.publish("/switch/2/state","0");
      Serial.println("Switch 2 Apagado");
    }
  }
}

void reconnect() {
  // Loop until we"re reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient","***USERNAME***","***PASSWORD***")) { // ON THIS FUNCTION IF YOU HAVE MQTT USERNAME AND PASSWORD, PUT THE USERNAME IN SECOND PARAMETER AND PASSWORD IN THIRD PARAMETER
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("openhab","himitsu sensor, reporting in");
      // ... and resubscribe
      client.subscribe("/switch/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void setup() {
  
  Serial.begin(115200);
  Ethernet.begin(mac);
  
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);

  sensors.begin();
  sensors.setResolution(insideThermometer, 9);

  Serial.println();
  
  getSensors();
  
  client.setServer(mqtt_server,1883);
  client.setCallback(callback);  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  
  readSensors();
  client.loop();
}

void getSensors(){
    int numeroSensoresConectados = sensors.getDeviceCount();
    Serial.print(numeroSensoresConectados);
    Serial.println(" sensores");  
}

void readSensors(){
  unsigned long tiempo;
  if(millis()-tiempo>=5000){
    sensors.requestTemperatures();
  }
  
  float temperatura = sensors.getTempCByIndex(0);
  char buffer[10];
  
  dtostrf(temperatura,2,1,buffer);
  client.publish("/temperatura/sensor1", buffer);
  Serial.println(temperatura);
}
