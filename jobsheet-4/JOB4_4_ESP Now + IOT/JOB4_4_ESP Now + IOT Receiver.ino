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
