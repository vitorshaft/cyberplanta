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

//Variável para armazenar o request HTTP
String header;

//Variaveis auxiliares para armazenar os estados do formulario e bomba
int estadoForm = 0;
String estadoBomba = "off";
int TIMER = 0;
int agora = 0;

//GPIO da bomba
//const int bomba = 2;
#define bomba LED_BUILTIN

int sensor_solo = 0;

int contador = 0;
int umidade = 0;
float h;
float t;
float luz;
int instantanea;
int media;
int um_pct;

/////////////// PRIMEIRA PARTE DA PAGINA

const char antes[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>Cyber Planta</title>
<style>
body {color: #434343; font-family: "Helvetica Neue",Helvetica,Arial,sans-serif; font-size: 14px; background-color: #393939; margin-top: 100px;}
.container {margin: 0 auto; max-width: 400px; padding: 30px; box-shadow: 0 10px 20px rgba(0,0,0,0.19), 0 6px 6px rgba(0,0,0,0.23); background-color: #ffffff; border-radius: 10px;}
h2 {text-align: center; margin-bottom: 20px; margin-top: 0px; color: #0ee6b1; font-size: 35px;}
#titleGreen {color: #0ca060;}
#titleBlack {color: #4D4D4D;}
h3 {text-align: center; margin-bottom: 40px; margin-top: 0px; color: #0ca060; font-size: 20px;}
h4 {text-align: left; margin-bottom: 20px; margin-top: 0px; color: #4d4d4d; font-size: 16px;}
form .field-group {box-sizing: border-box; clear: both; padding: 4px 0; position: relative; margin: 1px 0; width: 100%;}
.text-field {font-size: 15px; margin-bottom: 4%; -webkit-appearance: none; display: block; background: #fafafa; color: #636363; width: 100%; padding: 15px 0px 15px 0px; text-indent: 10px; border-radius: 5px; border: 1px solid #e6e6e6; background-color: transparent;}
.text-field:focus {border-color: #00bcd4; outline: 0;}
.button-container {box-sizing: border-box; clear: both; margin: 1px 0 0; padding: 4px 0; position: relative; width: 100%;}
.button {background: #0ca060; border: none; border-radius: 5px; color: #ffffff; cursor: pointer; display: block; font-weight: bold; font-size: 16px; padding: 15px 0; text-align: center; text-transform: uppercase; width: 100%; -webkit-transition: background 250ms ease; -moz-transition: background 250ms ease; -o-transition: background 250ms ease; transition: background 250ms ease;}
p {text-align: center; text-decoration: none; color: #87c1d3; font-size: 18px;}
a {text-decoration: none; color: #ffffff; margin-top: 0%;}
#status {text-align: center; text-decoration: none; color: #336859; font-size: 14px;}
</style>
<script>
function statusPlanta() {
var intervalo = document.forms["myForm"]["intervalo"].value;
}
</script>
</head>
<body>
<div class="container">
<h2><span id="titleGreen">Cyber</span><span id="titleBlack">Planta &#x1F331</span></h2>
<h3>Como está sua planta agora?</h3>
<h4>
)=====";

////////////////////////ULTIMA PARTE DA PAGINA
String lacuna = "&#9201 Intervalo de"+String(estadoForm)+"h";

const char depoisOn[] PROGMEM = R"=====(
</h4>
<form name="formTempo" action="/get">
<br>
<div class="field-group">
<input type="number" name="intervalo" maxlength="30" placeholder="&#9201 Mudar intervalo (h)"><br>
(Defina o intervalo de rega em horas)<br><br>
<div id="statusDiv">
<br>
</div>
<div class="button-container">
<a href=\"/timer/on\">
<input class="button" type="submit" value="Enviar">
</div>
</form>
</div>
</div>
</body>
</html>
)=====";

const char depoisOff[] PROGMEM = R"=====(
</h4>
<form name="formTempo" action="/get">
<br>
<div class="field-group">
<input type="number" name="intervalo" maxlength="30" placeholder="&#9201 Mudar intervalo (h)"><br>
(Defina o intervalo de rega em horas)<br><br>
<div id="statusDiv">
<br>
</div>
<div class="button-container">

<a href=\"/timer/off\">
<input class="button" type="submit" value="Enviar">
</a>
</div>
</form>
</div>
</div>
</body>
</html>
)=====";

void regar(int rele){
  digitalWrite(rele,LOW);
  while(millis()< (agora+1000)){
    
  }
  digitalWrite(rele,HIGH);
}

void atualizarTimer(){
  if(((int)header[19])-48 != estadoForm){
  //Serial.println(header.indexOf("GET /get?intervalo=1"));
  Serial.println(header[19]);
  estadoBomba = "on";
  Serial.println("Timer atualizado!");
  if(header[19]>47 && header[19]<58) estadoForm = ((int)header[19])-48;  //coleta tempo preenchido no formulario
  agora = millis();           //atualiza tempo atual
  TIMER = 3600000*estadoForm; //converte horas do timer em milissegundos
  //A linha de baixo é para testar em intervalos de 1 segundo (ver linha 246)
  //TIMER = 1000*estadoForm;
  TIMER = agora+TIMER;        //define o timer
  Serial.println("Timer atualizado!");
  }
}

void setup() {
  //pinMode(A0, INPUT);
  Serial.begin(115200);

  pinMode(bomba, OUTPUT);
  digitalWrite(bomba,HIGH);
  
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
  lacuna = "&#9201 Intervalo de"+String(estadoForm)+"h";
  h = dht.readHumidity();
  t = dht.readTemperature();

  luz = luximetro.readLightLevel();

  if(isnan(t) || isnan(t)){
    Serial.println(F("Falhou ao ler o sensor DHT"));
    return;
  }
  if(contador>10)contador=0;umidade=0;
  contador++;
  instantanea = analogRead(sensor_solo);
  umidade+= instantanea;
  media = umidade/contador;
  um_pct = map(instantanea,150,500,100,1);
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
  Serial.print("TIMER: ");
  Serial.println(TIMER);
  //delay(500);

  agora = millis();
  Serial.print("Agora: ");
  Serial.println(agora);
  if(TIMER > 0 && TIMER < millis()){
    regar(bomba);
    TIMER = 3600000*estadoForm; //converte horas do timer em milissegundos
    //A linha de baixo é para testar em intervalos de 1 segundo
    //1000*estadoForm;
    TIMER = agora+TIMER;        //define o timer
  }
  
  WiFiClient client = server.available();
  if (client) {                             // Se um novo cliente conecta,
    Serial.println("Novo Cliente.");          // exibe a mensagem na Serial
    String currentLine = "";                // Cria String para armazenar dados vindos do cliente
    while (client.connected()) {            // Enquanto cliente permanecer conectado...
      if (client.available()) {             // Se houver bytes a serem lidos do cliente,
        char c = client.read();             // le um byte e...
        Serial.write(c);                    // ...printa no monitor Serial
        header += c;
        if (c == '\n') {                    // se houver mudança de linha...
          // Se a linha atual estiver vazia, voce tem duas mudanças de linha.
          // Significa que a requisicao HTTP do cliente encerrou. Deve-se enviar uma resposta:
          if (currentLine.length() == 0) {
            // HTTP sempre começa com um codigo de resposta (ex.: HTTP/1.1 200 OK)

              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              client.println("Connection: close");  // the connection will be closed after completion of the response
              client.println("Refresh: 10");  // Atualiza o servidor a cada 20 segundos
              client.println();
              
              //JURO QUE A SOLUÇÃO MAIS ESTAVEL FOI USANDO IF/ELSE ;-;
              if(header.indexOf("GET /get?intervalo=1")>= 0){// HTTP/1.1"){
                atualizarTimer();
              }
              else if (header.indexOf("GET /get?intervalo=2") >= 0) {
                atualizarTimer();
            }
            else if (header.indexOf("GET /get?intervalo=3") >= 0) {
                atualizarTimer();
            }
            else if (header.indexOf("GET /get?intervalo=4") >= 0) {
                atualizarTimer();
            }
            else if (header.indexOf("GET /get?intervalo=5") >= 0) {
                atualizarTimer();
            }
            else if (header.indexOf("GET /get?intervalo=6") >= 0) {
                atualizarTimer();
            }
            else if (header.indexOf("GET /get?intervalo=7") >= 0) {
                atualizarTimer();
            }
            else if (header.indexOf("GET /get?intervalo=8") >= 0) {
                atualizarTimer();
            }
            else if (header.indexOf("GET /get?intervalo=9") >= 0) {
                atualizarTimer();
            }
            else if (header.indexOf("GET /get?intervalo=10") >= 0) {
                atualizarTimer();
            }
            else if (header.indexOf("GET /get?intervalo=11") >= 0) {
                atualizarTimer();
            }
            else if (header.indexOf("GET /get?intervalo=12") >= 0) {
                atualizarTimer();
            }
            else if (header.indexOf("GET /get?intervalo=0") >= 0) {
                atualizarTimer();
            }
              String primeiraParte = (const __FlashStringHelper*) antes;
              String ultimaParteOn = (const __FlashStringHelper*) depoisOn;
              String ultimaParteOff = (const __FlashStringHelper*) depoisOff;
              client.println(primeiraParte);
              client.println("&#x1F4A7 Umidade do ar: ");
              client.println(h);
              client.println("%<br>&#x1F321 Temperatura: ");
              client.println(t);
              client.println("ºC<br>&#x1F505 Luz: ");
              client.println(luz);
              client.println(" lx<br>&#x1F4A7 Umidade do solo: ");
              client.println(um_pct);
              client.println("%<br>&#9201 Intervalo de regas: ");
              client.println(estadoForm);
              client.println(" h");
              if (estadoBomba=="off") {
              client.println(ultimaParteOn);
              }
              else{
                client.println(ultimaParteOff);
              }
              client.println();
              break;
          }
          else{ //se houver uma nova linha, apagar a linha atual
            currentLine = "";
          }
      }
        else if (c != '\r'){  //se houver qualquer coisa exceto \r...
          currentLine+=c; //adicione essa cosia ao fim da linha atual
        }
      }
    }
    header = "";
    //Descomente as linhas abaixo para desconectar o cliente a cada update
    //fecha conexao
    //client.stop();
    //Serial.println("Cliente desconectado.");
    //Serial.println("");
  
  }
}
