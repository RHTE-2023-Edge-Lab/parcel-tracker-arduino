#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

MFRC522 mfrc522;
WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  Serial.begin(9600);     // Initialize serial communications with the PC
  while (!Serial);        // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  Serial.println("Initializing...");
  SPI.begin();            // Init SPI bus
  mfrc522.PCD_Init();     // Init MFRC522
  delay(4);               // Optional delay.

  if (!mfrc522.PCD_PerformSelfTest()) {
    Serial.println("MFRC522 not ready !");
  }
  Serial.println("Ready !");

  // Wifi initialization
  WiFi.mode(WIFI_STA);
  WiFi.begin("ESP-AP", "ShadowmanRHTE");
  Serial.print("Connecting to Wifi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  randomSeed(micros());

  // MQTT initialization
  client.setServer("test.mosquitto.org", 1883);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop()
{
  // MQTT connection to the broker + protocol handling
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Compute and print UID
  char uid[30];
  char * buffer = uid;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    int n = sprintf(buffer, "%s%02x", i > 0 ? ":" : "", mfrc522.uid.uidByte[i]);
    if (n > 0) {
      buffer += n;
    }
  }
  Serial.println(uid);
  client.publish("/test/nmasse@redhat.com/label-reader", uid);
  delay(500);
}
