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

#define LED D7

int state = 0;

void setup() {
  // Tell the device we want to write to this pin
  pinMode(LED, OUTPUT);
}
void loop() {
  // Alternate the PIN between high and low
  digitalWrite(LED, (state) ? HIGH : LOW);

  // Invert the state
  state = !state;

  delay(1000);
}