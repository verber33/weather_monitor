#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "ArduinoJson.h"
#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display
#include <Icons.h>
#include <string>

Adafruit_SSD1306 display(128, 64);  // Create display

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

const char* wifi_ssid = "TP-LINK_3B05F8";
const char* wifi_password = "0665266160";
const char* weatherUrl = "http://api.openweathermap.org/data/2.5/weather?q=kharkiv,kharkiv,ua&units=metric&appid=e0f294f1feeb79c64f812259e750be62";

const int LED = 14; // Assigning Pin 4 as the name LED

// HELPERS DEFINITION
void print(int number);
void print(float number);
void print(const char text);
void print(const char * text);
void print(String text);
void clearPrint(int number);
void clearPrint(const char * text);

// SETUP
void setup ()  
{
  pinMode (LED, OUTPUT); // Declaring LED pin as an output.

  // Start Display setup
  delay(100);  // This delay is needed to let the display to initialize
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize display with the I2C address of 0x3C
  display.clearDisplay();  // Clear the buffer
  display.setTextColor(WHITE);  // Set color of the text
  display.setRotation(0);  // Set orientation. Goes from 0, 1, 2 or 3
  // display.setTextWrap(false);  // By default, long lines of text are set to automatically “wrap” back to the leftmost column.
  display.dim(0);  //Set brightness (0 is maximun and 1 is a little dim)
  // End diaplay setup

  display.clearDisplay();
  // display.setFont(&FreeSans9pt7b);  // Set a custom font
  display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0
  display.setCursor(20, 30);
  display.println("[VERBER:TECH]");
  display.display();
  
  delay(2000);

  // Start Wi-Fi connection seput  
  Serial.begin(115200);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    // delay(100);
    // clearPrint("|");
    // delay(100);
    // clearPrint("/");
    // delay(100);
    // clearPrint("-");
    // delay(100);
    // clearPrint("\\");


    display.clearDisplay();
    display.drawBitmap(32, 0, earthBitmap, 64, 64, WHITE);
    display.display();

    delay(1000);
  }

  display.setFont();

  // clearPrint("WiFi connected:");
  // print(wifi_ssid);
  
  // IPAddress ipno;
  // char ipno2[26] ;
  // ipno = WiFi.localIP();
  // sprintf(ipno2, "%d.%d.%d.%d", ipno[0], ipno[1], ipno[2], ipno[3]);
  // print(ipno2);

  // End Wi-Fi connection seput
}

void loop() {

    // digitalWrite (LED, HIGH); // Making LED High.
    // delay(1000);              // Some Delay
    // digitalWrite (LED, LOW);  // Making LED LOW.
    // delay(1000);   


  // display.clearDisplay();
  // display.setFont(&FreeSans9pt7b);
  display.setTextSize(0);
  // display.setCursor(0, 0);

  // Get weather data
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin(weatherUrl);  //Specify request destination
    int httpCode = http.GET(); //Send the request

    // clearPrint("Loading...");
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      print(payload);             //Print the response payload
      
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      const int temp = (int)doc["main"]["temp"];
      const String humiditySign = "%";
      const int humidity = (int)doc["main"]["humidity"];
      const String windSign = " m/s";
      const int wind = (int)doc["wind"]["speed"];
      const char* iconType = doc["weather"][0]["icon"];
      const char* tempSign = "";
      
      if (temp > 0 ) {
        tempSign = "+";
      }
      else if (temp < 0) {
        tempSign = "-";
      }

      display.clearDisplay();
      display.setFont(&FreeSans12pt7b);  // Set a custom font
      display.setCursor(0, 21);
      display.println(tempSign);
      display.setFont(&FreeSansBold18pt7b);  // Set a custom font
      display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0
      display.setCursor(15, 24);
      display.println(temp);  // TEMPERATURE

      display.setFont(&FreeSans9pt7b);  // Set a custom font
      display.drawBitmap(-2, 31, humidity16Bitmap, 16, 16, WHITE);
      display.setCursor(18, 44);
      display.println(humidity);
      // display.setFont();
      // display.setCursor(37, 37);
      // display.println(humiditySign);

      display.setFont(&FreeSans9pt7b);  // Set a custom font
      display.drawBitmap(-3, 49, wind16Bitmap, 16, 16, WHITE);
      display.setCursor(18, 63);
      display.println(wind);
      // display.setCursor(30, 57);
      // display.println(windSign);

      const int imageX = 60;
      const int imageY = 0;

      if (strcmp("01d", iconType) == 0) {
        display.drawBitmap(imageX, imageY, sunBitmap, 64, 64, WHITE);
      }
      else if (strcmp("01n", iconType) == 0) {
        display.drawBitmap(imageX, imageY, moonBitmap, 64, 64, WHITE);
      }
      else if (strcmp("02d", iconType) == 0) {
        display.drawBitmap(imageX, imageY, sunCloudBitmap, 64, 64, WHITE);
      }
      else if(strcmp("02n", iconType) == 0) {
        display.drawBitmap(imageX, imageY, moonCloudBitmap, 64, 64, WHITE);
      }
      else if (strcmp("03d", iconType) == 0 || strcmp("03n", iconType) == 0) {
        display.drawBitmap(imageX, imageY, cloudBitmap, 64, 64, WHITE);
      }
      else if (strcmp("04d", iconType) == 0 || strcmp("04n", iconType) == 0) {
        display.drawBitmap(imageX, imageY, twoCloudsBitmap, 64, 64, WHITE);
      }
      else if (strcmp("09d", iconType) == 0 || strcmp("09n", iconType) == 0) {
        display.drawBitmap(imageX, imageY, showerRainBitmap, 64, 64, WHITE);
      }
      else if (strcmp("10d", iconType) == 0 || strcmp("10n", iconType) == 0) {
        display.drawBitmap(imageX, imageY, rainBitmap, 64, 64, WHITE);
      }
      else if (strcmp("11d", iconType) == 0 || strcmp("11n", iconType) == 0) {
        display.drawBitmap(imageX, imageY, thunderstormBitmap, 64, 64, WHITE);
      }
      else if (strcmp("13d", iconType) == 0 || strcmp("13n", iconType) == 0) {
        display.drawBitmap(imageX, imageY, snowFlakesBitmap, 64, 64, WHITE);
      }
      else if (strcmp("50d", iconType) == 0 || strcmp("50n", iconType) == 0) {
        display.drawBitmap(imageX, imageY, mistBitmap, 64, 64, WHITE);
      }

      display.display();  // Print everything we set previously

    } else {

      display.clearDisplay();
      display.setFont();
      display.setTextSize(0);
      display.setCursor(0, 10);
      print("Retrying in 30 sec");
    }
    http.end();   //Close connection
    display.display();
 
  } else {

    display.clearDisplay();
    display.setFont();
    display.setTextSize(1);
    display.setCursor(0, 10);
    clearPrint("---------");
    print("No WiFi detected:");
    print("Please relaunch me");
    print("---------");
  }
 
  delay(30000);    //Send a request every 30 seconds
}

// PRINT TO SCREEN
void print(int number)
{
  // Text style
  // display.setFont(&FreeSansBold9pt7b);  // Set a custom font
  display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0
  display.println(number);  // Text or value to print
  display.display();  // Print everything we set previously
}
void print(float number)
{
  // Text style
  // display.setFont(&FreeSansBold9pt7b);  // Set a custom font
  display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0
  display.println(number);  // Text or value to print
  display.display();  // Print everything we set previously
}
void print(const char * text)
{
  // display.setFont(&FreeSansBold9pt7b);  // Set a custom font
  display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0
  display.println(text);  // Text or value to print
  display.display();  // Print everything we set previously

}
void print(String text)
{
  // display.setFont(&FreeSansBold9pt7b);  // Set a custom font
  display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0
  display.println(text);  // Text or value to print
  display.display();  // Print everything we set previously

}

// CLEAR AND PRINT TO SCREEN
void clearPrint(int number)
{
  display.clearDisplay();  // Clear the display so we can refresh
  // display.setFont(&FreeSansBold9pt7b);  // Set a custom font
  display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0
  display.setCursor(0, 10);  // (x,y)
  display.println(number);  // Text or value to print
  display.display();  // Print everything we set previously
}

void clearPrint(const char * text)
{
  display.clearDisplay();  // Clear the display so we can refresh
  // display.setFont(&FreeSansBold9pt7b);  // Set a custom font
  display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0
  display.setCursor(0, 0);  // (x,y)
  display.println(text);  // Text or value to print
  display.display();  // Print everything we set previously
}
