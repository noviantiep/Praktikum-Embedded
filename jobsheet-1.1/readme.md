# Jobsheet 1.1
JARINGAN SENSOR NIRKABEL MENGGUNAKAN ESP-NOW

## Anggota Kelompok

- Dionysius Brammetya Yudhistira
- Noviantie Putriastuti

## 1. Memperoleh MAC Address ESP32 Receiver
### Program
Program ini berfungsi untuk mengetahui MAC Address yang terdapat di ESP32
<details>
    <summary>Program (click to open)</summary>

```c
#include "WiFi.h" //library yang digunakan hanya WiFi.h
void setup(){
Serial.begin(115200);
WiFi.mode(WIFI_MODE_STA); //mengaktifkan mode WiFi Station
Serial.println(WiFi.macAddress()); //print MAC Address di Serial Monitor
}
void loop(){ 
}
```
</details>

![MAC ADDRESS](https://user-images.githubusercontent.com/121749328/210868465-a0b0d40c-02e8-4499-ac7d-630a732dc0c7.png)

## 2. ESP-NOW One-Way Point-to-Point Communication
### Program Transmitter
<details>
    <summary>Program (click to open)</summary>
    
```c
//library yang dibutuhkan
#include <esp_now.h>
#include <WiFi.h>

// MAC Address Koordinator ESP
uint8_t broadcastAddress[] = {0x78, 0x21, 0x84, 0xBB, 0x45, 0xB8}; //78:21:84:BB:45:B8

//Struktur yang digunakan untuk transfer data, harus sama antara transmitter dan receiver
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

// membuat struktur dengan nama myData
struct_message myData;

// membuat interface peer
esp_now_peer_info_t peerInfo;

// memberikan feedback apabila data berhasil dikirimkan
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // inisialisasi Serial Monitor
  Serial.begin(115200);
 
  // membuat sebagai mode WiFi station
  WiFi.mode(WIFI_STA);

  // inisialisasi ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // mendapatkan status dari paket yang dikirim
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // menambah peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  //memberikan data yang akan dikirim
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = false;
  
  // mengirim data melalui ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}
```
</details>

![SENDER](https://user-images.githubusercontent.com/121749328/210869099-d1d631de-9244-4cd7-a3bf-a0efa4564ae9.png)
    
### Program Receiver
<details>
    <summary>Program (click to open)</summary>
    
```c
//library yang digunakan
#include <esp_now.h> 
#include <WiFi.h>

// Struktur pesan yang akan dikirim, struktur harus sama 
typedef struct struct_message {
char a[32]; 
int b; 
float c; 
bool d;
} struct_message;

// membuat variabel struktur menjadi myData 
struct_message myData;

// fungsi callback yang akan dieksekusi ketika ada pesan diterima
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) { 
memcpy(&myData, incomingData, sizeof(myData));
Serial.print("Bytes received: ");
Serial.println(len);
Serial.print("Char: ");
Serial.println(myData.a);
Serial.print("Int: ");
Serial.println(myData.b);
Serial.print("Float: ");
Serial.println(myData.c);
Serial.print("Bool: ");
Serial.println(myData.d);
Serial.println(); 
}
void setup() {
// Initialize Serial Monitor 
Serial.begin(115200);
// Set ESP32 sebagai station 
WiFi.mode(WIFI_STA);
// Init ESP-NOW
if (esp_now_init() != ESP_OK) {
Serial.println("Error initializing ESP-NOW"); 
return;
}
// Fungsi akses register cb untuk proses penerimaan data 
esp_now_register_recv_cb(OnDataRecv);
}
void loop() { 
}
```
</details>

![RECEIVER](https://user-images.githubusercontent.com/121749328/210868860-c3a4bbf9-1749-4a14-9fb7-04d12701f4e5.png)

## 3.  One-Way, One-to-Many Communication
### Program Transmitter
Mengirim pesan yang sama ke beberapa board ESP32
<details>
    <summary>Program (click to open)</summary>
    
```c
#include <esp_now.h>
#include <WiFi.h>
// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress1[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcastAddress2[] = {0xFF, , , , , };
uint8_t broadcastAddress3[] = {0xFF, , , , , };
typedef struct test_struct {
 int x;
 int y;
} test_struct;
test_struct test;
esp_now_peer_info_t peerInfo;
// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
 char macStr[18];
 Serial.print("Packet to: ");
 // Copies the sender mac address to a string
 snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
 mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4],
mac_addr[5]);
 Serial.print(macStr);
 Serial.print(" send status:\t");
 Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery
Fail");
}
void setup() {
 Serial.begin(115200);
 WiFi.mode(WIFI_STA);
 if (esp_now_init() != ESP_OK) {
 Serial.println("Error initializing ESP-NOW");
 return;
 }

 esp_now_register_send_cb(OnDataSent);

 // register peer
 peerInfo.channel = 0;
 peerInfo.encrypt = false;
 // register first peer
 memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
 if (esp_now_add_peer(&peerInfo) != ESP_OK){
 Serial.println("Failed to add peer");
 return;
 }
 // register second peer
 memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
 if (esp_now_add_peer(&peerInfo) != ESP_OK){
 Serial.println("Failed to add peer");
 return;
 }
 /// register third peer
 memcpy(peerInfo.peer_addr, broadcastAddress3, 6);
 if (esp_now_add_peer(&peerInfo) != ESP_OK){
 Serial.println("Failed to add peer");
 return;
 }
}
void loop() {
 test.x = random(0,20);
 test.y = random(0,20);
 esp_err_t result = esp_now_send(0, (uint8_t *) &test, sizeof(test_struct));

 if (result == ESP_OK) {
 Serial.println("Sent with success");
 }
 else {
 Serial.println("Error sending the data");
 }
 delay(2000);
}
```
</details>

<img width="394" alt="TRANSMITTERC" src="https://user-images.githubusercontent.com/121749328/210871011-1a05c5c7-1176-4bd0-935e-255cfdc218c1.png">

### Program Receiver
<details>
    <summary>Program (click to open)</summary>

```c
#include <esp_now.h>
#include <WiFi.h>
//Structure example to receive data
//Must match the sender structure
typedef struct test_struct {
 int x;
 int y;
} test_struct;
//Create a struct_message called myData
test_struct myData;
//callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
 memcpy(&myData, incomingData, sizeof(myData));
 Serial.print("Bytes received: ");
 Serial.println(len);
 Serial.print("x: ");
 Serial.println(myData.x);
 Serial.print("y: ");
 Serial.println(myData.y);
 Serial.println();
}
void setup() {
 //Initialize Serial Monitor
 Serial.begin(115200);

 //Set device as a Wi-Fi Station
 WiFi.mode(WIFI_STA);
 //Init ESP-NOW
 if (esp_now_init() != ESP_OK) {
 Serial.println("Error initializing ESP-NOW");
 return;
 }

 // Once ESPNow is successfully Init, we will register for recv CB to
 // get recv packer info
 esp_now_register_recv_cb(OnDataRecv);
}
void loop() {
}
```
</details>

<img width="605" alt="RECEIVERC" src="https://user-images.githubusercontent.com/121749328/210871249-6eaded05-1143-4557-a809-650d31692d6e.png">
    
## 4. One-Way, Many-to-One Communication
### Program Transmitter
<details>
    <summary>Program (click to open)</summary>

```c
#include <esp_now.h>
#include <WiFi.h>
// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
 int id; // must be unique for each sender board
 int x;
 int y;
} struct_message;
// Create a struct_message called myData
struct_message myData;
// Create peer interface
esp_now_peer_info_t peerInfo;
// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
 Serial.print("\r\nLast Packet Send Status:\t");
 Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery
Fail");
}
void setup() {
 // Init Serial Monitor
 Serial.begin(115200);
 // Set device as a Wi-Fi Station
 WiFi.mode(WIFI_STA);
 // Init ESP-NOW
 if (esp_now_init() != ESP_OK) {
 Serial.println("Error initializing ESP-NOW");
 return;
 }
 // Once ESPNow is successfully Init, we will register for Send CB to
 // get the status of Trasnmitted packet
 esp_now_register_send_cb(OnDataSent);

 // Register peer
 memcpy(peerInfo.peer_addr, broadcastAddress, 6);
 peerInfo.channel = 0;
 peerInfo.encrypt = false;

 // Add peer
 if (esp_now_add_peer(&peerInfo) != ESP_OK){
 Serial.println("Failed to add peer");
 return;
 }
}
void loop() {
 // Set values to send
 myData.id = 1;
 myData.x = random(0,50);
 myData.y = random(0,50);
 // Send message via ESP-NOW
 esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData,
sizeof(myData));

 if (result == ESP_OK) {
 Serial.println("Sent with success");
 }
 else {
 Serial.println("Error sending the data");
 }
 delay(10000);
}
```
</details>

<img width="362" alt="TRANSMITTERD" src="https://user-images.githubusercontent.com/121749328/210871844-65f8ba46-746a-4058-b67d-3c3fa980c166.png">

### Program Receiver
<details>
    <summary>Program (click to open)</summary>

```c
#include <esp_now.h>
#include <WiFi.h>
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
 int id;
 int x;
 int y;
}struct_message;
// Create a struct_message called myData
struct_message myData;
// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;
struct_message board3;
// Create an array with all the structures
struct_message boardsStruct[3] = {board1, board2, board3};
// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
 char macStr[18];
 Serial.print("Packet received from: ");
 snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
 mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4],
mac_addr[5]);
 Serial.println(macStr);
 memcpy(&myData, incomingData, sizeof(myData));
 Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
 // Update the structures with the new incoming data
 boardsStruct[myData.id-1].x = myData.x;
 boardsStruct[myData.id-1].y = myData.y;
 Serial.printf("x value: %d \n", boardsStruct[myData.id-1].x);
 Serial.printf("y value: %d \n", boardsStruct[myData.id-1].y);
 Serial.println();
}
void setup() {
 //Initialize Serial Monitor
 Serial.begin(115200);

 //Set device as a Wi-Fi Station
 WiFi.mode(WIFI_STA);
 //Init ESP-NOW
 if (esp_now_init() != ESP_OK) {
 Serial.println("Error initializing ESP-NOW");
 return;
 }

 // Once ESPNow is successfully Init, we will register for recv CB to
 // get recv packer info
 esp_now_register_recv_cb(OnDataRecv);
}
void loop() {
 // Acess the variables for each board
 /*int board1X = boardsStruct[0].x;
 int board1Y = boardsStruct[0].y;
 int board2X = boardsStruct[1].x;
 int board2Y = boardsStruct[1].y;
 int board3X = boardsStruct[2].x;
 int board3Y = boardsStruct[2].y;*/
 delay(10000);
}
```
</details>
    
![RECEIVERD](https://user-images.githubusercontent.com/121749328/210872536-6a89763e-f386-4be6-9813-53151a0e2536.png)
    
## 5. Two-Way Communication
### Program
<details>
    <summary>Program (click to open)</summary>
    
```c
#include "DHT.h"
#define DHTPIN 4 // Digital pin connected to the DHT sensor
// Uncomment whatever type you're using!
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21 // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);
void setup() {
 Serial.begin(9600);
 Serial.println(F("DHT11 Embedded System Test!"));
 dht.begin();
}
void loop() {
 // Wait a few seconds between measurements.
 delay(2000);
 // Reading temperature or humidity takes about 250 milliseconds!
 // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
 float h = dht.readHumidity();
 // Read temperature as Celsius (the default)
 float t = dht.readTemperature();
 // Read temperature as Fahrenheit (isFahrenheit = true)
 float f = dht.readTemperature(true);
 // Check if any reads failed and exit early (to try again).
 if (isnan(h) || isnan(t) || isnan(f)) {
 Serial.println(F("Failed to read from DHT sensor!"));
 return;
 }
 // Compute heat index in Fahrenheit (the default)
 float hif = dht.computeHeatIndex(f, h);
 // Compute heat index in Celsius (isFahreheit = false)
 float hic = dht.computeHeatIndex(t, h, false);
 Serial.print(F("Humidity: "));
 Serial.print(h);
 Serial.print(F("% Temperature: "));
 Serial.print(t);
 Serial.print(F("°C "));
 Serial.print(f);
 Serial.print(F("°F Heat index: "));
 Serial.print(hic);
 Serial.print(F("°C "));
 Serial.print(hif);
 Serial.println(F("°F"));
}
```
</details>

<img width="374" alt="SENSOR" src="https://user-images.githubusercontent.com/121749328/210873090-a2f72633-8c56-495c-838c-aa2d26315a76.png">

