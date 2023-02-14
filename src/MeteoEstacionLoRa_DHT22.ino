#include "DHTesp.h"
#include "Arduino.h"

//#include "heltec.h"
DHTesp dht;
 
float currentTemp;
float currentHumidity;
    
void setup()
{
	Serial.begin(115200);
  Serial.println("Starting");

  dht.setup(30, DHTesp::DHT22);
   
  currentTemp = 1;
  currentHumidity = 1;
 
  //pinMode(LED,OUTPUT);
  //digitalWrite(LED,HIGH);
 
  //Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, false /*Serial Enable*/);
}
 
 
    
void loop()
{
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
 
  if (temperature != currentTemp || humidity != currentHumidity) {
    currentTemp = temperature;
    currentHumidity = humidity;
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.println(humidity);
  }
    
  delay(2000);
}