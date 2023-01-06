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
