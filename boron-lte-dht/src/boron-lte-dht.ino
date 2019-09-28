/*
 * Environment monitoring with Particle Device Cloud, InfluxDB, and Grafana
 * Particle Boron LTE with DHT sensor
 * Copyright 2019 HackTheBase - UCW Labs Ltd. All rights reserved.
 *
 * The example was created for the HackTheBase IoT Hub Lab.
 *
 * The HackTheBase IoT Hub Lab is a creative space dedicated to prototyping and inventing, 
 * all sorts of microcontrollers (Adafruit Feather, Particle Boron, ESP8266, ESP32 and more) 
 * with various types of connectivity (WiFi, LoRaWAN, GSM, LTE-M), and tools such as screwdriver,
 * voltmeter, wirings, as well as an excellent program filled with meetups, workshops, 
 * and hackathons to the community members. 
 *
 * https://hackthebase.com/iot-hub-lab
 * 
 * The HackTheBase IoT Hub will give you access to the infrastructure dedicated to your project,
 * and you will get access to our Slack community.
 *
 * https://hackthebase.com/iot-hub
 *
 * As a member, you will get access to the hardware and software resources 
 * that can help you to work on your IoT project.
 *
 * https://hackthebase.com/register
 *
 */

#include "Adafruit_DHT.h"

#define LED     D3
#define DHTPIN  D6
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Events
#define EVENT_SENSOR_DHT_DATA   "/sensor/dht/data"
#define EVENT_SENSOR_DHT_LED    "/sensor/dht/led"

// Variables to be registered on Particle Device Cloud
double humidity = 0;
double temperature = 0;
double temperatureF = 0;

unsigned long lastConnectionTime = 0;               // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 15L * 1000L;  // delay between updates, in milliseconds

void eventHandler(const char *event, const char *data); 

void setup() {
  Serial.begin(9600);
  Serial.println("Boron LTE sensor node with DHT sensor");
  Particle.publish("state", "Boron LTE sensor node with DHT sensor has been started!");
  
  Particle.keepAlive(120);

  pinMode(LED, OUTPUT);
  
  Particle.function("LED",ledToggle);
  digitalWrite(LED, LOW);

  // Registeration of variables
  Particle.variable("humidity", humidity);
  Particle.variable("temperature", temperature);
  Particle.variable("temperatureF", temperatureF);
  
  Particle.subscribe(EVENT_SENSOR_DHT_LED, eventHandler);
  
  dht.begin();
  
  delay(2000);
}

void loop() {
  collectData();
}

void collectData() {
  if ((millis() - lastConnectionTime) > postingInterval) {
    String data = "";

    Serial.print("Data = ");
    data = readData();
    Serial.print(data);
    Serial.println("");

    if (data.length() > 0) {
      sendData(EVENT_SENSOR_DHT_DATA, data);
    }
  }
}

String readData() {
  String data = "";
  
  // Read humidity
  humidity = dht.getHumidity();
  // Read temperature as Celsius
  temperature = dht.getTempCelcius();
  // Read temperature as Farenheit
  temperatureF = dht.getTempFarenheit();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature) || isnan(temperatureF)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return data;
  }
  
  data = String::format("{\"humidity\": %4.2f, \"temperature\": %4.2f, \"temperatureF\": %4.2f", humidity, temperature, temperatureF);
  return data;
}

void sendData(String event, String payload) {
  if (payload.length() < 1) {
    Serial.println("No data to send!");
    return;
  }
    
	Particle.publish(event, payload, PRIVATE);

  // Note the time that the connection was made
  lastConnectionTime = millis();
}

int ledToggle(String command) {
  if (command == "on") {
    digitalWrite(LED, HIGH);
    return 1;
  
  } else if (command == "off") {
    digitalWrite(LED, LOW);
    return 0;

  } else {
    return -1;
  }
}

void eventHandler(const char *event, const char *data) {
  Serial.print("Event arrived [");
  Serial.print(event);
  Serial.print("] ");
  
  if (data) {
    Serial.println(data);
  } else {
    Serial.println("NULL");
  }

  if (strcmp(data, "on") == 0) {
    digitalWrite(LED, HIGH);
  } else if (strcmp(data, "off") == 0) {
    digitalWrite(LED, LOW);
  }
}