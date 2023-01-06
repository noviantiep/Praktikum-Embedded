# Jobsheet 2 
PROTOKOL KOMUNIKASI DAN SENSOR

## Anggota Kelompok

- Dionysius Brammetya Yudhistira
- Noviantie Putriastuti

## 1. ESP32 Capacitive Touch Sensor
### Program Capacitive Touch Sensor 1
Membaca sentuhan yang dilakukan dan menampilkan di serial monitor
<details>
    <summary>Program (click to open)</summary>

```c
// ESP32 Touch Test
// Just test touch pin - Touch0 is T0 which is on GPIO 4. 
void setup() {
Serial.begin(115200);
delay(1000); // give me time to bring up serial monitor 
Serial.println("ESP32 Touch Test");
}
void loop() {
Serial.println(touchRead(4));  // menampilkan nilai dari sentuhan yang ada di pin GPIO 4
delay(1000);
}
```
</details>

https://user-images.githubusercontent.com/121749328/210941385-4783b3da-e820-492d-955d-de255b124784.mp4

### Program Capacitive Touch Sensor 2
Jika disentuh maka LED akan menyala ketika disentuh dan tidak menyala jika tidak disentuh
<details>
    <summary>Program (click to open)</summary>

```c
// menginisiasi pin GPIO yang akan dipakai
const int touchPin = 4; //Pin GPIO yang mendeteksi sentuhan
const int ledPin = 16; //Pin GPIO yang akan menyalan LED sebagai keluaran

// change with your threshold value
const int threshold = 20;
// variable for storing the touch pin value 
int touchValue;

void setup(){
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  // Inisiasi LED sebagai keluaran
  pinMode (ledPin, OUTPUT);
}

void loop(){
  // membaca sensor sentuhan
  touchValue = touchRead(touchPin);
  Serial.print(touchValue);
  // mengecek apakah ada nilai dari sentuhan
  // jika ada maka LED akan menyala
  if(touchValue < threshold){
    // turn LED on
    digitalWrite(ledPin, HIGH);
    Serial.println(" - LED on");
  }
  else{
    // turn LED off
    digitalWrite(ledPin, LOW);
    Serial.println(" - LED off");
  }
  delay(500);
}
```
</details>

https://user-images.githubusercontent.com/121749328/210941602-cd761f2d-c13b-41ec-b5f8-37ec0f50f1b1.mp4

### Program Capacitive Touch Sensor 3
LED running (menyala bergantian dari kiri ke kanan)
<details>
    <summary>Program (click to open)</summary>

```c
// menginisiasi pin GPIO yang akan dipakai
const int touchPin = 4; 
const int ledPin1 = 16;
const int ledPin2 = 18;
const int ledPin3 = 19;

// change with your threshold value
const int threshold = 20;
// variable for storing the touch pin value 
int touchValue;

void setup(){
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  // menginisiasi pin GPIO LED sebagai keluaran
  pinMode (ledPin1, OUTPUT);
  pinMode (ledPin2, OUTPUT);
  pinMode (ledPin3, OUTPUT);
}

void loop(){
  // membaca nilai sentuhan
  touchValue = touchRead(touchPin);
  Serial.print(touchValue);
  // mengecek dengan logika IF ELSE dan akan menyalan LED jika terdapat sentuhan
  if(touchValue < threshold){
    // turn LED on
    Serial.println(" - LED on");
    digitalWrite(ledPin1, HIGH);
    delay(500);
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
    delay(500);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, HIGH);
    delay(500);
    digitalWrite(ledPin3, LOW);
       
  }
  else{
    // turn LED off
    digitalWrite(ledPin1, LOW);
digitalWrite(ledPin2, LOW);
digitalWrite(ledPin3, LOW);
    Serial.println(" - LED off");
  }
  delay(500);
}
```
</details>

https://user-images.githubusercontent.com/121749328/210941971-4a059997-7d3b-4cbd-8428-102068c465d5.mp4
    
## 2. Mengakses Sensor DHT 11 (Single Wire / BUS)
### Program
Sensor DHT11 berfungsi untuk membaca kelembapan dan suhu yang dapat di ESP32 untuk mengumpulkan data dan menjadikannya sebuah input untuk sebuah sistem seperti dalam contoh jika suhu dibawah 30° C maka LED akan berjalan runnning.
<details>
    <summary>Program (click to open)</summary>
    
```c
//Library yang dibutuhkan
#include "DHT.h"

//Pin yang terkoneksi dengan Sensor DHT
#define DHTPIN 4

//Tipe DHT yang digunakan
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21 // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE); //Inisialisasi DHT dengan PIN
void setup() {

//Inisialisasi Serial Monitor untuk debugging
 Serial.begin(9600);
 Serial.println(F("DHT11 Embedded System Test!"));
 // Inisialisasi DHT
 dht.begin();
}
void loop() {
 //Membaca nilai Sensor dari DHT11
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
 
 //Print nilai data yang telah dibaca dari sensor ke serial monitor
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
```c
//Library yang dibutuhkan
#include "DHT.h"

//Pin yang terkoneksi dengan sensor DHT
#define DHTPIN 4

//Tipe DHT yang digunakan
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21 // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

//Pin yang terkoneksi dengan LED
const int ledPin1 = 16;
const int ledPin2 = 18;
const int ledPin3 = 19;

void setup() {
//Inisialisasi Serial Monitor untuk debugging
 Serial.begin(9600);
 Serial.println(F("DHT11 Embedded System Test!"));
 dht.begin(); //Inisialisasi DHT
 
 //Memberikan perintah LED sebagai output
   pinMode (ledPin1, OUTPUT);
   pinMode (ledPin2, OUTPUT);
   pinMode (ledPin3, OUTPUT);

}
void loop() {
 //Membaca sensor DHT 
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
  
  //Logika IF untuk membuat jika suhu dibawah 30° C maka LED akan berjalan running, ketika suhu diatas 30° C maka LED akan mati
  if(t < 30){
Serial.println(" - LED on");
    digitalWrite(ledPin1, HIGH);
    delay(500);
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
    delay(500);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, HIGH);
    delay(500);
    digitalWrite(ledPin3, LOW);
  }
  else{
    // turn LED off
    digitalWrite(ledPin1, LOW);
digitalWrite(ledPin2, LOW);
digitalWrite(ledPin3, LOW);
    Serial.println(" - LED off");
  }
 
 //Print nilai sensor di serial monitor
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

https://user-images.githubusercontent.com/121749328/210943632-1783c7f1-904e-4585-bfff-72d7651df4c0.mp4

## 3. Mengakses Sensor RFID (SPI Communication)
### Program
Membaca RFID
<details>
    <summary>Program (click to open)</summary>

```c
//Library yang dibutuhkan
#include <SPI.h>
#include <MFRC522.h>

//Inisialisasi pin untuk RFID Reader
#define SS_PIN    21  // ESP32 pin GIO21 
#define RST_PIN   22 // ESP32 pin GIO22

//Inisialisasi RFID module
MFRC522 rfid(SS_PIN, RST_PIN);

//MAC Address yang ada di kartu
byte keyTagUID[7] = {0x05, 0x83, 0x4B, 0x7D, 0x30, 0xD1, 0x00}; //05 83 4B 7D 30 D1 00
void setup() {
//Inisialisasi serial monitor untuk debugging
Serial.begin(9600);

//Inisialisasi SPI bus
SPI.begin();

//Inisialisasi MFRC522
rfid.PCD_Init();

//Print tulisan ketika ESP pertama kali dijalankan
Serial.println("Tap RFID/NFC Tag on reader"); 
}
void loop() {


if (rfid.PICC_IsNewCardPresent()) { // Kartu baru ditambahkan
if (rfid.PICC_ReadCardSerial()) { // NUID ID telah terbaca
MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak); //Verifikasi pencocokan kartu dengan MAC yang telah diatur 
if (rfid.uid.uidByte[0] == keyTagUID[0] &&
rfid.uid.uidByte[1] == keyTagUID[1] &&
rfid.uid.uidByte[2] == keyTagUID[2] &&
rfid.uid.uidByte[3] == keyTagUID[3] &&
rfid.uid.uidByte[4] == keyTagUID[4] &&
rfid.uid.uidByte[5] == keyTagUID[5] &&
rfid.uid.uidByte[6] == keyTagUID[6] ) { 
Serial.println("Access is granted"); //Ketika kartu yang didaftarkan sesuai maka akan memberikan jawaban 
} 
else 
{
Serial.print("Access denied for user with UID:"); //Ketika kartu yang dibaca tidak sesuai dengan yang didaftarkan maka akan ditolak

//Print MAC Address kartu
for (int i = 0; i < rfid.uid.size; i++) {
  Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "); 
Serial.print(rfid.uid.uidByte[i], HEX);
}
Serial.println(); 
}
rfid.PICC_HaltA(); // halt PICC
rfid.PCD_StopCrypto1(); // stop encryption on PCD
}
}
}
```
Memberikan akses untuk RFID
```c
//Library yang digunakan
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

//Inisialisasi pin untuk RFID Reader
#define SS_PIN    21  // ESP32 pin GIO21 
#define RST_PIN   22 // ESP32 pin GIO22

//Inisialisasi pin untuk LED & Servo
const int ledPin1 = 32;
const int ledPin2 = 12;
static const int servoPin = 4;

//Memberi nama Servo
Servo servo1;

//Kartu yang didaftarkan
MFRC522 rfid(SS_PIN, RST_PIN);
byte keyTagUID[4] = {0x53, 0xB5, 0xE0, 0x16}; //53 B5 E0 16

void setup() {
//Inisialisasi serial monitor untuk debugging
Serial.begin(9600);

//Mengatur LED sebagai output
pinMode (ledPin1, OUTPUT);
pinMode (ledPin2, OUTPUT);

//Inisialisasi Servo
servo1.attach(servoPin);

//Inisialisasi RFID Module
SPI.begin(); // init SPI bus
rfid.PCD_Init(); // init MFRC522
Serial.println("Tap RFID/NFC Tag on reader"); 
delay(1000);

}
void loop() {

if (rfid.PICC_IsNewCardPresent()) { //Tag baru ditambahkan
if (rfid.PICC_ReadCardSerial()) { //Kartu baru saja dibaca

//Verifikasi kartu apakah sama dengan yang didaftarkan
MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak); 
if (rfid.uid.uidByte[0] == keyTagUID[0] &&
rfid.uid.uidByte[1] == keyTagUID[1] &&
rfid.uid.uidByte[2] == keyTagUID[2] &&
rfid.uid.uidByte[3] == keyTagUID[3] ) {
  Serial.println(); 
Serial.println("Access is granted"); //Akses diberikan ketika kartu yang dibaca sama dengan yang didaftarkan
Serial.println("SERVO BERGERAK"); //Print untuk memberitahukan bahwa servo bergerak
digitalWrite(ledPin2, HIGH); //LED Hidup karena kartu yang dibaca sesuai

//Servo bergerak karena kartu yang dibaca sesuai
    for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
        servo1.write(posDegrees);
        delay(5);
    }
delay(5000);
digitalWrite(ledPin2, LOW); //LED dimatikan

//Servo bergerak kembali ke posisi awal
    for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {
        servo1.write(posDegrees);
        delay(20);
    }
    
} 
else //Ketika kartu yang dibaca tidak sesuai maka tidak ada pergerakan 
{
  Serial.println();
Serial.print("Access denied for user with UID:");
for (int i = 0; i < rfid.uid.size; i++) {
  Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "); 
Serial.print(rfid.uid.uidByte[i], HEX);
}
digitalWrite(ledPin1, HIGH);
delay(3000); 
digitalWrite(ledPin1, LOW);

}
rfid.PICC_HaltA(); // halt PICC
rfid.PCD_StopCrypto1(); // stop encryption on PCD
}
}
}
```
</details>

https://user-images.githubusercontent.com/121749328/210944052-8c7465a4-5daf-4d0b-beea-1c47f7210ba0.mp4
