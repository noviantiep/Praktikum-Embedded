# Jobsheet 4 

## Anggota Kelompok
- Dionysius Brammetya Yudhistira
- Noviantie Putriastuti

## 1. CAYENNE (MQTT) + SENSOR (DHT11) + LED (ON/OFF) + BUTTON DI WEBSITE MONITORING
### Program
<details>
  <summary>Program (click to open)</summary>

```c
#include "CayenneMQTTESP32.h" // ganti tanda " dengan > jika program error
#include "DHT.h"
//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#define DHTPIN 4     // what digital pin we're connected to
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// WiFi network info.
char ssid[] = "home-mnl";
char wifiPassword[] = "h0m3-121";
const int ledPin = 16;

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "9f1901b0-7f54-11ed-b193-d9789b2af62b";
char password[] = "15d7c1af5defe0be2d748a44ebec74251ae13b9e";
char clientID[] = "82f43c00-8749-11ed-b193-d9789b2af62b";

unsigned long lastMillis = 0;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}
float h, t, f;
void loop() {
  Cayenne.loop();

  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

}

//Mengirimkan data ke MQTT Cayenne dan menampilkannya di Dashboard Aplikasi Cayenne
CAYENNE_OUT(1)
{
  CAYENNE_LOG("Send data for Virtual Channel %d Suhu %f C", 1, t);
  Cayenne.celsiusWrite(1, t);
}
CAYENNE_OUT(2)
{
  CAYENNE_LOG("Send data for Virtual Channel %d Hum %f ", 2, h);
  Cayenne.virtualWrite(2, h);
}
CAYENNE_IN(3)
{
  digitalWrite(ledPin, !getValue.asInt());  // to get the value from the website
}
```
</details>

https://user-images.githubusercontent.com/121760251/210358473-1bae9070-5118-448c-a296-1a2ef45dc1a3.mp4

## 2. ADAFRUIT.IO (MQTT) + IFTTT -> SENSOR (DHT11) + LED (ON/OFF) -> SUARA (GOOGLE ASSITANT)
### Program
<details>
  <summary>Program (click to open)</summary>

```c
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <DHT.h>

#define WLAN_SSID       "ALPHA 1" 
#define WLAN_PASS       "wologito37"     
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883              
#define AIO_USERNAME    "azpaska"
#define AIO_KEY         "aio_lFJJ47BKS9I94LwxwYjAxnT1N4Q2"
#define DHTPIN D5
#define led1 D0
#define led2 D1
#define led3 D2

DHT dht(DHTPIN, DHT11); 

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/LED-RED");
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LED-GREEN");
Adafruit_MQTT_Subscribe Light3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/LED-BLUE");
Adafruit_MQTT_Subscribe Light = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/LED");

float temp,humi;
uint32_t x=0;
void MQTT_connect();
//=========================================================
void setup() {
  Serial.begin(9600);
  delay(10);
  dht.begin();     

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  mqtt.subscribe(&Light1);
  mqtt.subscribe(&Light2);
  mqtt.subscribe(&Light3);
  mqtt.subscribe(&Light);
}
//=============================================
void loop() {  
  MQTT_connect();
  humi = dht.readHumidity();
  temp = dht.readTemperature();
  if (isnan(humi) || isnan(temp)) {
    Serial.println("DHT11 tidak terbaca... !");
    return;
  }
  else{ 
  // Now we can publish stuff!
    Serial.print("Temperature=");
    Serial.println(temp);
    //Serial.print("...");
    if (! temperature.publish(temp)) {
      Serial.println(F("Publish Temperature Failed"));
      } else {
      Serial.println(F("Publish Temperature OK!"));
    }
    Serial.print("Humidity=");
    Serial.println(humi);
    //Serial.print("...");
      if (! humidity.publish(humi)) {
        Serial.println(F("Publish humidity Failed"));
        } else {
        Serial.println(F("Publish Humidity OK!"));
      }
    delay(5000);  
    }
    
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &Light1) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light1.lastread);
      int Light1_State = atoi((char *)Light1.lastread);
      digitalWrite(led1, Light1_State);

    }
    if (subscription == &Light2) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light2.lastread);
      int Light2_State = atoi((char *)Light2.lastread);
      digitalWrite(led2, Light2_State);
    }
    if (subscription == &Light3) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light3.lastread);
      int Light3_State = atoi((char *)Light3.lastread);
      digitalWrite(led3, Light3_State);
    }
    if (subscription == &Light) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light.lastread);
      int Light_State = atoi((char *)Light.lastread);
      digitalWrite(led1, Light_State);
      digitalWrite(led2, Light_State);
      digitalWrite(led3, Light_State);
    }
  }
}

//===============================================
void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
```
</details>

Serial Monitor
  
![image](https://user-images.githubusercontent.com/118667288/210917732-da67957a-6d9e-484f-94e3-e3405bd348d5.png)

Adafruit.io

![image](https://user-images.githubusercontent.com/118667288/210917794-ad67d112-c173-4d89-873e-b48c4ac23068.png)

IFTTT
  
![image](https://user-images.githubusercontent.com/118667288/210917840-69cbf1be-0ecd-479e-aa53-56ed546da238.png)

## 3. THINGSPEAK (HTTP/REST) + SENSOR (DHT11)
### PROGRAM
<details>
  <summary>Program (click to open)</summary>

```c
#include <DHT.h>
#include <ESP8266WiFi.h>
 
String apiKey = "O54IVHCUBLMPKMWD";     //  Enter your Write API key from ThingSpeak
 
const char *ssid =  "ALPHA 1";     // replace with your wifi ssid and wpa2 key
const char *pass =  "wologito37";
const char* server = "api.thingspeak.com";
 
#define DHTPIN D5          //pin where the dht11 is connected
 
DHT dht(DHTPIN, DHT11);
 
WiFiClient client;
 
void setup() 
{
       Serial.begin(115200);
       delay(10);
       dht.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
       
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
}
 
void loop() 
{
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {     
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(h);
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
          Serial.println("Waiting...");
          
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
}
```
</details>

![image](https://user-images.githubusercontent.com/118667288/210918703-8c53ef2f-a569-4090-be45-a590ad2d2fc5.png)

![image](https://user-images.githubusercontent.com/118667288/210918736-db82c694-7daf-4cd9-bea8-33b8da672c2b.png)

## 4. ESP Now + IOT
### PROGRAM
SENDER
<details>
  <summary>Program (click to open)</summary>

```c
//Library yang dibutuhkan
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include "DHT.h"

//Mendefinisikan DHT11 di pin GPIO 4 dan jenis DHT yang digunakan
#define DHTPIN 4
#define DHTTYPE DHT11

//Mac Address yang akan dituju untuk menerima data
uint8_t broadcastAddress[] = {0x78, 0x21, 0x84, 0x9C, 0x13, 0x8C};

//Board ID ESP32 - digunakan sebagai identifikasi ESP yang terhubung ke koordinator
const int espNumber = 2;

//Struktur data yang akan dikirimkan, struktur data harus sama dengan receiver
typedef struct struct_message {
  int id;
  float temp;
  float hum;
} struct_message;

//membuat struktur data dengan nama myData
struct_message myData;

//Membuat interface peer
esp_now_peer_info_t peerInfo;

//Mengirim notifikasi apabila data berhasil dikirimkan
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Variable untuk membaca temperature dan humidity
float temperature;
float humidity;

WiFiClient  client;

//Mengisisiasi pin GPIO dan tipe DHT11 yang tersambung
DHT dht(DHTPIN, DHTTYPE);

//Memasukan nama SSID yang sama-sama tersambung
constexpr char WIFI_SSID[] = "k";

//Memindai channel wifi untuk ditumpangi dalam pengiriman data
int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
    for (uint8_t i = 0; i < n; i++) {
      if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
        return WiFi.channel(i);
      }
    }
  }
  return 0;
}
void setup() {
  Serial.begin(115200); //Memulai serial dengan 115200 baud rate
  dht.begin(); // Memulai fungsi DHT
  WiFi.mode(WIFI_STA); //Membuat ESP32 sebagai station
  
  //Mendapatkan channel wifi
  int32_t channel = getWiFiChannel(WIFI_SSID); 

  WiFi.printDiag(Serial); // Uncomment to verify channel number before
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  WiFi.printDiag(Serial);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  //Mengirimkan data ke ESP32 yang ditunjuk sebagai koordinator sekaligus mengirimkan feedback apabila paket diterima koordinator
  esp_now_register_send_cb(OnDataSent); 

  //Regristasi peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6); 
  //peerInfo.channel = 0;
  peerInfo.encrypt = false;

  //Menambahkan peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {

//Membaca nilai sensor temperature dan humidity dari DHT11 dan memverifikasi nilai data, apabila nilai sensor terbaca maka disimpan di variabel humidity & temperature
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
 //Menampilkan pembacaan nilai sensor
  Serial.print("Temperature (ºC): ");
  Serial.print(temperature);
  Serial.print(" Humidity (%): ");
  Serial.println(humidity);

//Mengirimkan data nilai sensor ke ESP32 Koordinator
  myData.id = espNumber;
  myData.temp = temperature;
  myData.hum = humidity;
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

//Memberitahukan bahwa pesan sukses/gagal dikirim
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }

  delay(1000);
}
```
</details>
                        
RECEIVER
<details>
  <summary>Program (click to open)</summary>

```c
//library yang dibutuhkan
#include <esp_now.h>
#include <WiFi.h>
#include <CayenneMQTTESP32.h>
#define CAYENNE_PRINT Serial

//Struktur data yang dibutuhkan untuk menerima data dari ESP32 lain
typedef struct struct_message {
  int id;
  float temp;
  float hum;
} struct_message;

//Membuat struktur data myData, board1, board2, board3
struct_message myData;
struct_message board1;
struct_message board2;
struct_message board3;
struct_message boardsStruct[3] = {board1, board2, board3};

//Autentikasi untuk tersambung ke WiFi dan Cayenne
const char* ssid = "k";   // your network SSID (name)
const char* password = "123456779";   // your network password
const char* username = "fa9de470-7f7b-11ed-b193-d9789b2af62b";
const char* mqtt_password = "3b449543f2b1413ddffea61e5828a05ff09f07d6";
const char* cliend_id = "40ce4d40-87f6-11ed-8d53-d7cd1025126a";
float temperature[3];
float humidity[3];

//Memberitahukan apabila data berhasil diterima akan tampil di serial monitor
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4],
           mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  boardsStruct[myData.id - 1].temp = myData.temp;
  boardsStruct[myData.id - 1].hum = myData.hum;
  Serial.printf("Board: %d \n", myData.id);
  Serial.printf("Temperature: %.2f \n", boardsStruct[myData.id - 1].temp);
  Serial.printf("Humidity: %.2f \n", boardsStruct[myData.id - 1].hum);
  Serial.println();
  temperature[myData.id - 1] = boardsStruct[myData.id - 1].temp;
  humidity[myData.id - 1] = boardsStruct[myData.id - 1].hum;
}

void setup() {
  Serial.begin(115200);//Inisiasli serial dengan 115200 baud rate
  WiFi.mode(WIFI_STA);//Inisiasi ESP32 menjadi station
  //Inisiasi ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //Mendapatkan informasi dari ESP32 yang mengirimkan data ke Koordinator
  esp_now_register_recv_cb(OnDataRecv);
  Cayenne.begin(username, mqtt_password, cliend_id, ssid, password);
}

void loop() {
  Cayenne.loop(); //Mebuat fungsi Cayenne berjalan berulang-ulang untuk mengirimkan data
  delay(1000);
}
//Mengirimkan data ke Cayenne yang diterima oleh ESP32 koordinator, terdapat 2 ESP32 yang menjadi transmitter
CAYENNE_OUT_DEFAULT()
{
  Cayenne.virtualWrite(1, temperature[0], "temperature", "Celcius");
  Cayenne.virtualWrite(2, humidity[0], "humidity", "%");
  Cayenne.virtualWrite(3, temperature[1], "temperature", "Celcius");
  Cayenne.virtualWrite(4, humidity[1], "humidity", "%");
  Cayenne.virtualWrite(5, temperature[2], "temperature", "Celcius");
  Cayenne.virtualWrite(6, humidity[2], "humidity", "%");
}
```
</details>

SENDER

![IMG20230101142840](https://user-images.githubusercontent.com/121760251/210786283-587824b3-bad9-4462-8346-36173179b553.jpg)

RECEIVER

![IMG20230101143016](https://user-images.githubusercontent.com/121760251/210786357-42498009-085a-40d1-9ce0-1043d8d53a8c.jpg)

HASIL

![image](https://user-images.githubusercontent.com/121760251/210786623-3974b22e-4f87-4cf7-99f2-0e2c3c43f57a.png)
