/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the //Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on

 This example is written for a network using WPA2 encryption. For insecure
 WEP or WPA, change the Wifi.begin() call and use Wifi.setMinSecurity() accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 4

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin
 
 */

#include <WiFi.h>
#include <esp_wifi.h>

const char* ssid     = "Pina Colada";
const char* password = "sadcucumber";
String opened = "closed";

// Set your Static IP address
IPAddress local_IP(192, 168, 86, 137);
// Set your Gateway IP address
IPAddress gateway(192, 168, 86, 1);

IPAddress subnet(255, 255, 255, 0);
//uint8_t New_MAC_Address[] = {0xAD, 0x0B, 0xE1, 0x5B, 0x00, 0xBA};
//uint8_t newMACAddress[] = {0xAD, 0x0B, 0xE1, 0x5B, 0x00, 0xBA};
uint8_t newMACAddress[] = {0x00, 0x00, 0x0B, 0x00, 0xB1, 0xE5};
//Adobe is Booba

WiFiServer server(80);
#define LED_BUILTIN 2

void setup()
{
    WiFi.mode(WIFI_STA);
    delay(30);
    //esp_wifi_set_mac(WIFI_IF_STA, New_MAC_Address);
    esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]);
    //Serial.begin(9600);
    pinMode(4, OUTPUT);      // set the LED pin mode
    pinMode(LED_BUILTIN, OUTPUT);

    delay(10);

    // We start by connecting to a WiFi network

    //Serial.println();
    //Serial.println();
    //Serial.print("Connecting to ");
    //Serial.println(ssid);

  // Configures static IP address


  // if (!WiFi.config(local_IP, gateway, subnet)) {
  //   digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  //   delay(300);                      // wait for a second
  //   digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  //   delay(300);
  //   digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  //   delay(300);                      // wait for a second
  //   digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  //   delay(300);
  //   digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  //   delay(300);                      // wait for a second
  //   digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  //   delay(300);
  // }

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        //Serial.print(".");
    }

    //Serial.println("");
    //Serial.println("WiFi connected.");
    //Serial.println("IP address: ");
    //Serial.println(WiFi.localIP());
    
    server.begin();
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);

}

void loop(){

 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    //Serial.println("New Client.");           // print a message out the //Serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the //Serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<html><head><meta http-equiv=\"Refresh\" content=\"3; url='/'\" /></head><body>Click <a href=\"/H\">here</a> to toggle the garage door on pin 4.<br></body></html>");
            //The garage door is currently "+ opened + "

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(4, HIGH);               // GET /H turns the LED on
          delay(300);
          digitalWrite(4, LOW);
        }
      }
    }
    // close the connection:
    client.stop();
    //Serial.println("Client Disconnected.");
  }
}
