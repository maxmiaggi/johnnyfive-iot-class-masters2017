//#define LAB2
#define LAB3

#include <ArduinoJson.h>
#include <SPI.h>
#include <WiFi101.h>

#define PubNub_BASE_CLIENT WiFiClient
#define PUBNUB_DEBUG
#include <PubNub.h>

char ssid[] = "ssid";
char pass[] = "pass";

//char ssid[] = "MASTERS_GUEST";
//char pass[] = "";
int status = WL_IDLE_STATUS;

#ifdef LAB2
#define JSON_STRING_SIZE    50
char pubkey[] = "pub-c-cfd2c879-04ee-40ad-bf7f-6d4df977ff7a";
char subkey[] = "sub-c-801ccfbc-41a2-11e7-bc55-0619f8945a4f";
char channel[] = "led";
#endif

#ifdef LAB3
#define JSON_STRING_SIZE    200
#define LIGHT_THRESHOLD     800
char pubkey[] = "pub-c-43404d0c-78e6-4ebb-a9e7-ca0e54125a52";
char subkey[] = "sub-c-486be1d0-5c63-11e7-b272-02ee2ddab7fe";
char channel[] = "temperature-photoresistor";
char channel2[] = "potentiometer";
#endif


//Specify digital pins on the Arduino
#define LED_PIN     6
#define PIEZO_PIN   12

void setup() {
    /*Tone needs 2 arguments, but can take three
    1) Pin#
    2) Frequency - this is in hertz (cycles per second) which determines the pitch of the noise made
    3) Duration - how long teh tone plays
    */
    tone(PIEZO_PIN, 1000, 500); 
    pinMode(LED_PIN, OUTPUT);   // Set led pin as output
    
    delay(7000);    // Enough delay to fire up serial monitor
    Serial.begin(9600);
    while (status !=WL_CONNECTED) {
        Serial.println("Attempting to connect to network…");
        status = WiFi.begin(ssid, pass);
        delay(1000);
    }
    Serial.print("SSID:");
    Serial.println(WiFi.SSID());
    PubNub.begin(pubkey, subkey);
}

void loop() {
  char  json[JSON_STRING_SIZE] = "["; // Start with '[' because an addtional ']' is returned
  int   jsonIndx = 0;     // Start at location json[0]
  
  Serial.println("waiting for a message (subscribe)");
  PubSubClient  *pclient = PubNub.subscribe(channel);
  if (!pclient) {
    Serial.println("subscription error");
    delay(1000);
    return;
  }
  
  //Serial.print("Received: ");
  while (pclient->wait_for_data()) {
    json[++jsonIndx] = pclient->read();
    //Serial.print(json[jsonIndx]);
  }
  json[++jsonIndx] = NULL;
  pclient->stop();
  //Serial.println();

#ifdef LAB2
  // Parse JSON using ArduinoJson Library
  const size_t bufferSize = 2*JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonArray& root = jsonBuffer.parseArray(json);
  
  bool blinkstate = root[0][0]["blink"];

  // Print parsed values
  Serial.print("Blinkstate: ");
  Serial.println(blinkstate);

  // Do something with the parsed values
  // When blinkstate = true, turn led ON
  // Otherwise turn led OFF.
  // We cannot "blink" blink since wait_for_data()
  // is a blocking function call in subscribe.
  if(blinkstate) {
    digitalWrite(LED_PIN, HIGH);
  }
  else {
    digitalWrite(LED_PIN, LOW);
  }
#endif

#ifdef LAB3
  // Parse JSON using ArduinoJSON Library
  const size_t bufferSize = 2*JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 40;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonArray& root = jsonBuffer.parseArray(json);
  
  float temperature = root[0][0]["eon"]["temperature"];
  int light = root[0][0]["eon"]["light"];

  // Print parsed values
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("\tLight: ");
  Serial.println(light);

  // Do something with the parsed values
  // Sound the buzzer when light value drops below 800
  if(light < LIGHT_THRESHOLD) {
    tone(PIEZO_PIN, 1000);
    digitalWrite(LED_PIN, HIGH);
  }
  else {
    noTone(PIEZO_PIN);
    digitalWrite(LED_PIN, LOW);
  }
#endif
  
  delay(1000);
}
