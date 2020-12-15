
// WiFi
#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present 
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(10, 11); // RX, TX 
#endif


// LCD
#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

// sound sensor pin
const int pinSound = A0;
// sound sensor threshold
int thresholdValue = 200;

// Define motion sensor connection pins:
#define pirPin 2
// Create variables for motion sensor
int val = 0;

// simulate camera using LED on pin 13
#define ledPin 13

bool soundDetect = false;
bool motionDetect = false;

char ssid[] = "MySpectrumWiFi88-2G"; // your network SSID (name) 
char pass[] = "wittysnake179"; // your network password 
int status = WL_IDLE_STATUS; // the Wifi radio's status 
char server[] = "3.16.218.192";

char var[] = "SoundDetected";
char var1[] = "MotionDetected";
char var2[] = "BothDetected";
char var3[] = "NoDetected";

char get_request[200];
// Initialize the Ethernet client object 
WiFiEspClient client;

void setup() {
  // put your setup code here, to run once:
  pinMode(pirPin, INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Welcome");

// initialize serial for debugging 
Serial.begin(115200);
// initialize serial for ESP module 
Serial1.begin(115200);
// initialize ESP module 
WiFi.init(&Serial1);
// check for the presence of the shield 
if (WiFi.status() == WL_NO_SHIELD) {
  Serial.println("WiFi shield not present"); 
  // don't continue
  while (true);
}
// attempt to connect to WiFi network 
while ( status != WL_CONNECTED) {
  Serial.print("Attempting to connect to WPA SSID: "); 
  Serial.println(ssid);
  // Connect to WPA/WPA2 network
  status = WiFi.begin(ssid, pass);
}
Serial.println("You're connected to the network"); 
printWifiStatus();

}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Read out the pirPin and store as val:
  val = digitalRead(pirPin);

  // If motion is detected (pirPin = HIGH)
  if (val == HIGH) {

    // Change the motion state to true (motion detected)
    if (motionDetect == false) {
      motionDetect = true;
    }

  } else {
    // no motion detected
    // Change the motion state to false (no motion):
    if (motionDetect == true) {  
      motionDetect = false;
    }
  }

  // run for sound sensor
  // Read the value of the sound sensor.
    int sensorValue = analogRead(pinSound);

    // If the measured sound level is above the threshold, sound detection is true.
    if(sensorValue > thresholdValue)
    {
      if (soundDetect == false) {
        // change the sound detection to true
        soundDetect = true;
      }
    } else {
        soundDetect = false;
    }

    

    // display on LCD
    lcd.setCursor(0, 0);
    if (soundDetect == true && motionDetect == false) {
      lcd.print("Sound Detected");
      digitalWrite(13, HIGH);
      //delay(1000);
    } else if (soundDetect == false && motionDetect == true) {
      lcd.print("Motion Detected");
      digitalWrite(13, HIGH);
      //delay(1000);
    } else if (soundDetect == true && motionDetect == true) {
      lcd.print("Both Detected");
      digitalWrite(13, HIGH);
      //delay(1000);
    } else {
      lcd.print("No detection");
      digitalWrite(13, LOW);
      //delay(1000);
    }

Serial.println();
if (!client.connected()){
Serial.println("Starting connection to server...");
client.connect(server, 5000); }
Serial.println("Connected to server");
// Make a HTTP request
if (soundDetect == true && motionDetect == false) {
      sprintf(get_request,"GET /?var=%s HTTP/1.1\r\nHost: 18.221.147.67\r\nConnection: close\r\n\r\n", var);
    } else if (soundDetect == false && motionDetect == true) {
      sprintf(get_request,"GET /?var=%s HTTP/1.1\r\nHost: 18.221.147.67\r\nConnection: close\r\n\r\n", var1);
    } else if (soundDetect == true && motionDetect == true) {
      sprintf(get_request,"GET /?var=%s HTTP/1.1\r\nHost: 18.221.147.67\r\nConnection: close\r\n\r\n", var2);
    } else {
      sprintf(get_request,"GET /?var=%s HTTP/1.1\r\nHost: 18.221.147.67\r\nConnection: close\r\n\r\n", var3);
    }
 
client.print(get_request);
delay(500);
while (client.available()) {
char c = client.read();
Serial.write(c); }
delay(1000);
    

    lcd.clear();


}
void printWifiStatus() {
// print the SSID of the network you're attached to 
Serial.print("SSID: ");
Serial.println(WiFi.SSID());
// print your WiFi shield's IP address
IPAddress ip = WiFi.localIP(); 
Serial.print("IP Address: "); 
Serial.println(ip);
// print the received signal strength 
long rssi = WiFi.RSSI(); 
Serial.print("Signal strength (RSSI):"); 
Serial.print(rssi);
Serial.println(" dBm"); }
