/*    _              __  _   
     | |            / _|| |  
 ___ | |__    __ _ | |_ | |_ 
/ __|| '_ \  / _` ||  _|| __|
\__ \| | | || (_| || |  | |_ 
|___/|_| |_| \__,_||_|   \__|
                          
 *   DESENVOLVIDO POR VITOR SHAFT EM 20-JUN-2022
 * https://github.com/vitorshaft/cyberplanta.git
 * https://instagram.com/shaftrobotica
 * shaftrobotica@gmail.com
 */

/* 1. Baixar e instalar libs WiFiManager https://github.com/tzapu/WiFiManager
    e Arduino JSON https://github.com/bblanchon/ArduinoJson
*/
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "DHT.h"
#include <BH1750.h>
#include <Wire.h>

#define DHTPIN 14

#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);
BH1750 luximetro;
WiFiServer server(80);

int sensor_solo = 0;

int contador = 0;
int umidade = 0;

void setup() {
  //pinMode(A0, INPUT);
  Serial.begin(9600);
  dht.begin();
  Wire.begin();
  luximetro.begin();
  delay(2000);
  // WiFiManager
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  
  // Uncomment and run it once, if you want to erase all the stored information
  //wifiManager.resetSettings();
  
  // set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "AutoConnectAP"
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("Cyber Planta");
  // or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();
  
  // if you get here you have connected to the WiFi
  Serial.println("Conectado.");
  
  server.begin();
}

void loop() {
  delay(1000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  float luz = luximetro.readLightLevel();

  if(isnan(t) || isnan(t)){
    Serial.println(F("Falhou ao ler o sensor DHT"));
    return;
  }
  if(contador>10)contador=0;umidade=0;
  contador++;
  int instantanea = analogRead(sensor_solo);
  umidade+= instantanea;
  int media = umidade/contador;
  int um_pct = map(instantanea,150,500,100,1);
  Serial.print(F("Umidade ar: "));
  Serial.print(h);
  Serial.println(F("%"));
  Serial.print(F("Temperatura do ar: "));
  Serial.print(t);
  Serial.println(F("°C"));
  /*
  Serial.print(F("Umidade solo abs: "));
  Serial.print(instantanea);
  Serial.print(F(" / "));
  */
  Serial.print(F("Umidade solo: "));
  Serial.print(um_pct);
  Serial.println(F("%"));
  Serial.print(F("Luz(lx): "));
  Serial.println(luz);
  //delay(500);
  WiFiClient client = server.available();
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");  // the connection will be closed after completion of the response
client.println("Refresh: 10");  // update the page after 10 sec
client.println();
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.println("<style>html { font-family: 'Trebuchet MS', sans-serif; display: block; margin: 0px auto; text-align: center;color: #333333; background-color: #8ccef3;}");
client.println("body{margin-top: 50px;}");
client.println("h1 {margin: 50px auto 30px; font-size: 50px; text-align: center;}");
client.println(".side_adjust{display: inline-block;vertical-align: left;position: relative;}");
client.println(".text1{font-weight: 180; font-size: 50px; width: 170px; text-align: left; color: #4d4d4d;}");
client.println(".data1{font-weight: 180; padding-left: 60px; font-size: 50px;color: #4d4d4d;}");
client.println(".text2{font-weight: 180; font-size: 50px; width: 170px; text-align: left; color: #4d4d4d;}");
client.println(".data2{font-weight: 180; padding-left: 150px; font-size: 50px;color: #4d4d4d;}");
client.println(".text3{font-weight: 180; padding-left: 15px; font-size: 50px; width: 170px; text-align: left; color: #4d4d4d;}");
client.println(".data3{font-weight: 180; padding-left: 80px; font-size: 50px;color: #4d4d4d;}");
client.println(".text4{font-weight: 180; padding-left: 15px; font-size: 50px; width: 170px; text-align: left; color: #4d4d4d;}");
client.println(".data4{font-weight: 180; padding-left: 80px; font-size: 50px;color: #4d4d4d;}");
client.println(".data{padding: 10px;}");
client.println("</style>");
client.println("</head>");
client.println("<body>");
client.println("<div id=\"webpage\">");   
client.println("<h1>Cyber Planta Server</h1>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text1\">Umidade:</div>");
//client.println("Umidade: ");
client.println("<div class=\"side_adjust data1\">");
client.print(h);
client.println("<div class=\"side_adjust text1\">%</div>");
client.println("</div>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text2\">Temperatura:</div>");
client.println("<div class=\"side_adjust data2\">");
client.print(t);
client.println("<div class=\"side_adjust text2\"> C</div>");
client.println("</div>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text3\">Luz:</div>");
client.println("<div class=\"side_adjust data3\">");
client.print(luz);
client.println("<div class=\"side_adjust text3\">lx</div>");
client.println("</div>");
client.println("</div>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text4\">Solo:</div>");
client.println("<div class=\"side_adjust data4\">");
client.print(um_pct);
client.println("<div class=\"side_adjust text4\">%</div>");
client.println("</div>");
client.println("</div>");
client.println("</body>");
client.println("</html>");
 delay(4000);
}
