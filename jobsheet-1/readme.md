
# Jobsheet 1 
DASAR PEMROGRAMAN ESP32 UNTUK PEMROSESAN DATA INPUT/OUTPUT ANALOG DAN DIGITAL

## Anggota Kelompok

- Dionysius Brammetya Yudhistira
- Noviantie Putriastuti

## 1. GPIO
### Program GPIO 1
Program di bawah ini mengendalikan led menggunakan push button.
<details>
    <summary>Program (click to open)</summary>
    
```c
// set pin numbers
const int buttonPin1 = 15;  // the number of the pushbutton pin 
const int buttonPin2 = 2;
const int buttonPin3 = 4;
const int ledPin1 =  5;    // the number of the LED pin
const int ledPin2 =  18;
const int ledPin3 =  19;
int button1, button2, button3;
// variable for storing the pushbutton status 
int buttonState = 0;
void setup() {
Serial.begin(115200);  
// initialize the pushbutton pin as an input 
pinMode(buttonPin1, INPUT);
pinMode(buttonPin2, INPUT);
pinMode(buttonPin3, INPUT);
// initialize the LED pin as an output 
pinMode(ledPin1, OUTPUT);
pinMode(ledPin2, OUTPUT);
pinMode(ledPin3, OUTPUT);
}
void loop() {
// read the state of the pushbutton value 
button1 = digitalRead(buttonPin1); 
button2 = digitalRead(buttonPin2); 
button3 = digitalRead(buttonPin3); 
// check if the pushbutton is pressed. 
// if it is, the buttonState is HIGH 
if (button1 == HIGH) {
    digitalWrite(ledPin1, HIGH);
}  else if (button2 == HIGH){
    digitalWrite(ledPin2, HIGH);
    delay(500);
    digitalWrite(ledPin2, LOW);
    delay(500);
} else if (button3 == HIGH) {
    digitalWrite(ledPin1, HIGH);
    delay(500);
    digitalWrite(ledPin1, LOW);
    delay(500);
    digitalWrite(ledPin2, HIGH);
    delay(500);
    digitalWrite(ledPin2, LOW);
    delay(500);
    digitalWrite(ledPin3, HIGH);
    delay(500);
    digitalWrite(ledPin3, LOW);
    delay(500);
}
}
```
</details>

https://user-images.githubusercontent.com/121749328/210791851-bdbed79e-9386-46c0-803f-116085364fd8.mp4

### Program GPIO 2
Tambahkan 1 LED dan 1 push button pada rangkaian. Kemudian tambahkan program agar ketika push button ke-2 ditekan, LED akan melakukan blink setiap 500 ms sekali.
<details>
    <summary>Program (click to open)</summary>
    
```c
// set pin numbers
const int buttonPin = 4;
const int buttonPin2 = 16;
const int ledPin = 5;
const int ledPin2 = 18;
int buttonState = 0;
int buttonState2 = 0;

void setup() {
 Serial.begin(115200);
 pinMode(buttonPin, INPUT);
 pinMode(buttonPin2, INPUT);
 pinMode(ledPin, OUTPUT);
 pinMode(ledPin2, OUTPUT);
}

void loop() {
 buttonState = digitalRead(buttonPin);
 buttonState2 = digitalRead(buttonPin2);
 Serial.println(buttonState);
 Serial.println(buttonState2);
 
 if (buttonState == HIGH) {
 digitalWrite(ledPin, HIGH);
 } else {
 digitalWrite(ledPin, LOW);
 }
 
 if (buttonState2 == HIGH) {
 digitalWrite(ledPin2, HIGH);
 delay(500);
 digitalWrite(ledPin2, LOW);
 delay(500);
 } else {
 digitalWrite(ledPin2, LOW);
 }
}
```

</details>

https://user-images.githubusercontent.com/121749328/210792374-627f1fc5-5a17-4922-96a3-61de993d3793.mp4

### Program GPIO 3
Tambahkan 3 LED dan 1 push button pada rangkaian, kemudian kembangkan program agar ketika push button ke-3 ditekan, LED akan menyala menjadi running led (menyala bergantian dari kiri ke kanan).
<details>
    <summary>Program (click to open)</summary>

```c
// set pin numbers
const int buttonPin = 4;
const int buttonPin2 = 16;
const int buttonPin3 = 17;
const int ledPin = 5;
const int ledPin2 = 18;
const int ledPin3 = 19;
const int ledPin4 = 21;
const int ledPin5 = 3;
int buttonState = 0;
int buttonState2 = 0;
int buttonState3 = 0;

void setup() {
 Serial.begin(115200);
 pinMode(buttonPin, INPUT);
 pinMode(buttonPin2, INPUT);
 pinMode(buttonPin3, INPUT);
 pinMode(ledPin, OUTPUT);
 pinMode(ledPin2, OUTPUT);
 pinMode(ledPin3, OUTPUT);
 pinMode(ledPin4, OUTPUT);
 pinMode(ledPin5, OUTPUT);
}

void loop() {
 buttonState = digitalRead(buttonPin);
 buttonState2 = digitalRead(buttonPin2);
 buttonState3 = digitalRead(buttonPin3);
 Serial.println(buttonState);
 Serial.println(buttonState2);
 Serial.println(buttonState3);
 
 if (buttonState == HIGH) {
 digitalWrite(ledPin, HIGH);
 } else {
 digitalWrite(ledPin, LOW);
 }
 
 if (buttonState2 == HIGH) {
 digitalWrite(ledPin2, HIGH);
 delay(500);
 digitalWrite(ledPin2, LOW);
 delay(500);
 } else {
 digitalWrite(ledPin2, LOW);
 }
 
 if (buttonState3 == HIGH) {
 digitalWrite(ledPin5, LOW);
 digitalWrite(ledPin3, HIGH);
 delay(100);
 digitalWrite(ledPin3, LOW);
 digitalWrite(ledPin4, HIGH);
 delay(100);
 digitalWrite(ledPin4, LOW);
 digitalWrite(ledPin5, HIGH);
 delay(100);
 } else {
 digitalWrite(ledPin3, LOW);
 digitalWrite(ledPin4, LOW);
 digitalWrite(ledPin5, LOW);
 }
}
```
    
</details>

https://user-images.githubusercontent.com/121749328/210792851-e5a80166-60de-4990-b809-b2b86587b080.mp4


## 2. PWM
### Program PWM 1
Pogram dibawah ini dapat mengatur intensitas cahaya LED
<details>
    <summary>Program (click to open)</summary>

```c
// the number of the LED pin
const int ledPin = 16; // 16 corresponds to GPIO16
// setting PWM properties
const int freq = 5000;
const int ledChannel = 0; //PWM Channel
const int resolution = 8; //resolution bit
void setup(){
 // configure LED PWM functionalitites
 ledcSetup(ledChannel, freq, resolution);
 
 // attach the channel to the GPIO to be controlled
 ledcAttachPin(ledPin, ledChannel);
}
void loop(){
 // increase the LED brightness
for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){ 
 // changing the LED brightness with PWM
 ledcWrite(ledChannel, dutyCycle);
 delay(15);
 }
 // decrease the LED brightness
 for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
 // changing the LED brightness with PWM
 ledcWrite(ledChannel, dutyCycle); 
 delay(15);
 }
}
```

</details>

https://user-images.githubusercontent.com/121749328/210793996-50a6af91-bae9-49bb-801a-47311c3e336d.mp4


### Program PWM 2
Program dibawah ini mengatur instensitas cahaya beberapa LED sekaligus
<details>
    <summary>Program (click to open)</summary>
    
```c
// the number of the LED pin
const int ledPin = 16; // 16 corresponds to GPIO16
const int ledPin2 = 17; // 17 corresponds to GPIO17
const int ledPin3 = 5; // 5 corresponds to GPIO5
// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
void setup(){
 // configure LED PWM functionalitites
 ledcSetup(ledChannel, freq, resolution);
 
 // attach the channel to the GPIO to be controlled
 ledcAttachPin(ledPin, ledChannel);
 ledcAttachPin(ledPin2, ledChannel);
 ledcAttachPin(ledPin3, ledChannel);
}
void loop(){
 // increase the LED brightness
 for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){ 
 // changing the LED brightness with PWM
 ledcWrite(ledChannel, dutyCycle);
 delay(15);
 }
 // decrease the LED brightness
 for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
 // changing the LED brightness with PWM
 ledcWrite(ledChannel, dutyCycle); 
 delay(15);
 }
}
```
    
</details>

https://user-images.githubusercontent.com/121749328/210794431-2c680792-1378-491e-8dfe-ffd8bd8f46c4.mp4


## 3. ADC DAC
### Program ADC DAC 1
Program ini dapat mengatur besaran resistansi menggunakan potensiometer dan menampilkan nilai resistansi pada serial monitor
<details>
    <summary>Program (click to open)</summary>
    
```c
// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potPin = 34;
// variable for storing the potentiometer value
int potValue = 0;
void setup() {
 Serial.begin(115200);
 delay(1000);
}
void loop() {
 // Reading potentiometer value
potValue = analogRead(potPin);
 Serial.println(potValue);
 delay(500);
}
```
    
</details>

https://user-images.githubusercontent.com/121749328/210798509-95330656-accf-404b-827e-ab85ef1e5b9b.mp4


### Program ADC DAC 2
Intensitas cahaya LED berubah - ubah karena nilai resistansi yang berubah
<details>
    <summary>Program (click to open)</summary>
    
```c
// These constants won't change. They're used to give names to the pins used:
const int analogInPin = 34; // Analog input pin that the potentiometer is attached to
const int analogOutPin = 5; // Analog output pin that the LED is attached to
// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
int sensorValue = 0; // value read from the pot
int outputValue = 0; // value output to the PWM (analog out)
int analogWrite = 2;
void setup() {
 Serial.begin(115200); // initialize serial communications at 115200 bps:
// configure LED PWM functionalitites
 ledcSetup(ledChannel, freq, resolution);
 
 // attach the channel to the GPIO to be controlled
 ledcAttachPin(analogOutPin, ledChannel);
}
void loop() {
 sensorValue = analogRead(analogInPin); // read the analog in value:
 outputValue = map(sensorValue, 0, 4095, 0, 255); // map it to the range of the analog out:
 analogWrite(analogOutPin, outputValue); // change the analog out value:
// print the results to the Serial Monitor:
 Serial.print("sensor = ");
 Serial.print(sensorValue);
 Serial.print("\t output = ");
 Serial.println(outputValue);
 // wait 2 milliseconds before the next loop for the analog-to-digital
 // converter to settle after the last reading:
 delay(2);
}
```

</details>


https://user-images.githubusercontent.com/121749328/210799068-87ec61e6-0804-47e5-90d6-b9ac8d9a93bd.mp4

