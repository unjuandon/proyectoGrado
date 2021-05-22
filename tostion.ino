// Desarrollado por: Juan Camilo Cardona Castro
// Date  : 15 may 2020
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include "max6675.h"
#include <SPI.h>
#include<SD.h>
#include <TimeLib.h>

#ifndef APSSID
#define APSSID "access point"
#define APPSK  "hola1234"
#endif
int ktcSO =5;
int ktcCS = 2;
int ktcCLK = 4;


MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

const char* ssid = APSSID;

const char* password = APPSK;

AsyncWebServer server(80);

//tiempo1=millis();
int temp;
const int chipSelect = 4;
void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  setTime(0,0,1,0,0,2020);

  // Iniciar Spiffs
  if(!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  
  WiFi.softAP(ssid,password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Dirección IP:");
  Serial.println(IP);


   Serial.print("Inicializando tarjeta SD...");

  // Mira si la tarjeta SD está presente y la inicializa:
  if (!SD.begin(chipSelect)) {
    Serial.println("No se puede inicializar, o no está insertada la tarjeta sd.");
    // don't do anything more:
    while (1);
  }
  Serial.println("Tarjeta inicializada.");
  

  // Muestra la dirección IP local del ESP8266
  Serial.println(WiFi.localIP());

  // Rutas de las páginas, librerías y módulos que se van a cargar al node.
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html");
  });
  server.on("/tostion.html", HTTP_GET,[](AsyncWebServerRequest * request){
    request->send(SPIFFS, "/tostion.html");
  });
  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/bootstrap.min.css");
  });
  server.on("/jquery-3.3.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/jquery-3.3.1.min.js");
  });
  server.on("/bootstrap.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/bootstrap.min.js");
  });
  server.on("/popper.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/popper.min.js");
  });
  server.on("/raphael-2.1.4.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/raphael-2.1.4.min.js");
  });
  server.on("/justgage.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/justgage.js");
  });
  server.on("/highcharts.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/highcharts.js");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", temperaturaTermocupla().c_str());
  });
    
  // Start server
  server.begin();
}
 
void loop()
{  

/*tiempo2 = millis();
if(tiempo2 > (tiempo1+1000))
{
  tiempo1=millis();
  tiempoSegundos=tiempo1/1000;
}*/

time_t t = now();

if(t%2)
{
  Serial.print("Minuto: ");
  Serial.print(minute(t));
  Serial.print(" + ");
  Serial.print("Segundo: ");
  Serial.println(second(t));
}

int temperatura = ktc.readCelsius()-4;

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("Temperatura.csv ", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print("Temperatura: ");
    dataFile.print(temperatura);
    dataFile.print("Minuto: ");
    dataFile.print(minute(t));
    dataFile.print(" + ");
    dataFile.print("Segundo: ");
    dataFile.println(second(t));
    dataFile.close();
   // dataFile.println(tiempoSegundos);
   // dataFile.close();
    // print to the serial port too:
    Serial.println(temperatura);
   // Serial.println(tiempoSegundos);
  }
  // if the file isn't open, pop up an error:
 /* else {
    Serial.println("error opening datalog.txt");
  }*/


  delay(5000);
}




String temperaturaTermocupla()
{
  int t = (int)ktc.readCelsius()-4;
  
  if (isnan(t))
    temp = temp;
  else
    temp = t;
  
  return String(temp);
}
