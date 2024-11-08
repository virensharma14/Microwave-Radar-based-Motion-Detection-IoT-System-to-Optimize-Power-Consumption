/**************************************************
Home automation code for ESP32 - Arduino IDE
Connect LED + 330 ohm resistor to D2
Connect Potentiometer wiper to D34, 3.3V and gnd
Code for 1st ESP32
*************************************************/
#include <ESP8266WiFi.h> // for ESP8266 nodemcu
//#include <WiFi.h> //included in ESP32 boards package
#include <PubSubClient.h> //Install with Arduino Library manager or download at https://github.com/knolleary/pubsubclient


#define DEVICE_1_OUTPUT "device1/esp32/output"
#define DEVICE_2_OUTPUT "device2/esp32/output"
#define DEVICE_1_STRING "device1/esp32/string"
#define DEVICE_2_STRING "device2/esp32/string"


// Replace the next variables with your SSID/Password combination
const char* ssid = "LAPTOP";//Add here your Wifi SSID name
const char* password = "12345678910"; //Add here your Wifi password
const char* mqtt_server = "172.22.170.154";// your MQTT broker IPv4 adress - THIS IS DIFFERENT FOR YOU, pls check yours with "ipconfig"  in cmd line
const char *States[] = {"HIGH2", "HIGH3", "LOW1", "HIGH1", "LOW2", "OFF"};
WiFiClient espClient1;
PubSubClient client(espClient1);// To connect more ESP32's change "client" to for instance "client2" here and in the rest of the code
//WiFiClient espClient2;
//PubSubClient client(espClient2);// To connect more ESP32's change "client" to for instance "client2" here and in the rest of the code


long lastMsg = 0;
char msg[50];
int Value = 0;
//int Sensor =13; //esp32
int Sensor =13; //esp8266
int Bulb=14;//yellow//esp32//esp8266


unsigned long previousmillis=0;
unsigned long previous2millis=0;
int interval=5000;
int values[5]={0,0,0,0,0};
int interval1=200;
bool SensValue=LOW; 
bool sensorreadings(){
  bool aur=0;
  unsigned long current2millis=millis();
  unsigned long previous2millis=current2millis;
  int i=1;
  while(1){
    unsigned long current2millis=millis();
    if(current2millis-previous2millis >= interval1)
    {
      previous2millis=current2millis;
      values[i]=digitalRead(Sensor);
      aur=aur||values[i];
      i++;
    }
    if(i==6){
      break;
    }
  }
  for(int i=0;i<5;i++){
    Serial.print(values[i]);
  }
  Serial.println();
  Serial.println(aur);
  return aur;
}





void setup() {
  Serial.begin(1200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode (Sensor, INPUT);  // RCWL-0516 as input
  pinMode (Bulb, OUTPUT);    // LED as OUTPUT 
}



void setup_wifi() {  // Connect to WiFi network
  delay(100);
  Serial.println();
  Serial.print("Connecting to Wifi");
  //Serial.println(ssid); //uncomment if you want to see your SSID in serial monitor
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}





void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == DEVICE_1_OUTPUT) {//Check if the MQTT message matches the subscription, if add more ESP32 change topic esp32 to for instance esp33
    Serial.print("Changing output to ");
    if(messageTemp == "true"){//if the message is "true", switch on
      Serial.println("on");
      Value=1;
    }
    else if(messageTemp == "false"){//if the message is "false", switch off
      Serial.println("off");
      Value=0;
    }
  }
}



void reconnect() {
  while (!client.connected()) {   // Loop until connected to the MQTT server
    Serial.print("Trying MQTT connection...");
    if (client.connect("ESP8266Client")) {     // Try to connect
      Serial.println("connected");
      client.subscribe(DEVICE_1_OUTPUT);      // Subscribe to topic, if add more ESP32 change topic esp32 to for instance esp33
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);      // Wait 5 seconds before retrying
    }
  }
}




void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  unsigned long currentmillis=millis();
  if (SensValue == HIGH && Value == 1) {    
    digitalWrite(Bulb, HIGH);
    Serial.println("HIGH1");
    client.publish(DEVICE_1_STRING, States[3]);
    if(currentmillis-previousmillis >= interval){
      Serial.print("curr :");
      Serial.println(currentmillis);
      Serial.print("prev :");
      Serial.println(previousmillis);
      bool sense=sensorreadings();
      if(sense==1){
        Serial.println("HIGH2");
        client.publish(DEVICE_1_STRING, States[0]);
        digitalWrite(Bulb, HIGH);
        previousmillis=currentmillis;
      }
      else{
        SensValue=LOW;
        Serial.println("LOW2");
        client.publish(DEVICE_1_STRING, States[4]);
        digitalWrite(Bulb, LOW);
        previousmillis=currentmillis;
      }
     }
     else{
       if(digitalRead(Sensor)){
         currentmillis=millis();
         previousmillis=currentmillis;
         Serial.print("curr :");
         Serial.println(currentmillis);
         Serial.print("prev :");
         Serial.println(previousmillis);
       }
     }
  }
  else {
    if(Value==1){     
      previousmillis=currentmillis;
      SensValue = digitalRead(Sensor);
      digitalWrite(Bulb,SensValue);
      if(SensValue){
        Serial.println("HIGH3");
        client.publish(DEVICE_1_STRING, States[1]);
      }
      else{
        Serial.println("LOW1");
        client.publish(DEVICE_1_STRING, States[2]);
      }
    }
    else{
      Serial.println("OFF");
      client.publish(DEVICE_1_STRING, States[5]);
      SensValue = 0;
      digitalWrite(Bulb,SensValue);
    }
  }
}
