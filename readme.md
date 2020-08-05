# ARDUINO DS18B20 SENSOR + 2 X RELAY WITH MQTT CLIENT/SERVER

## MQTT SERVER IP ADDRESS
---

#### MAKE SURE TO PUT THE IP ADDRESS OF THE BROKER IN THIS LINE
  
```ARDUINO
const char* mqtt_server = "***.***.***.***"; 
```

## PIN CONFIGURATION
---

#### ARDUINO SWITCH PINS

```ARDUINO
const int switch1 = 3;`
const int switch2 = 4; 
```

##### ARDUINO DS18B20 PIN

MAKE SURE TO PUT A PULLUP RESISTOR ON PIN 2 AND +5V

```ARDUINO
const int sensorTemperatura = 2;
```

## MQTT TOPIC CONFIGURATION

---

#### TOPIC TO SUSCRIBE FOR MQTT RELAY MESSAGES AND STATES

```ARDUINO
client.subscribe("/switch/#");
```  

#### CONDITIONAL TO POWER ON AND OFF THE SWITCH AND PUBLISH THE STATE


```ARDUINO
  if(_topic == "/switch/1"){ // SWITCH TOPIC TO SEND THE PAYLOAD
    Serial.println("Topic OK Switch 1");
    if(_payload == "ON"){
      digitalWrite(switch1,LOW);
      client.publish("/switch/1/state","1"); // STATE TOPIC
      Serial.println("Switch 1 Encendido");
    } else if (_payload == "OFF"){
      digitalWrite(switch1,HIGH);
      client.publish("/switch/1/state","0");
      Serial.println("Switch 1 Apagado");
    }
  }
```

PAYLOAD "ON" PUT THE ARDUINO PIN TO LOW

PAYLOAD "OFF" PUT THE ARDUINO PIN TO HIGH

YOU CAN COPY THIS CODE TO ADD MORE SWITCHS

#### DS18B20 TOPIC

```ARDUINO
client.publish("/temperatura/sensor1", buffer);
```
