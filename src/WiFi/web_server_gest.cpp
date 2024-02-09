/*
web_server_gest.cpp
*/

#include <my_define.h>
//#include <MyWifi.h>

#include <soc/soc.h>           // Disable brownout problems
#include <soc/rtc_cntl_reg.h>  // Disable brownout problems
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>

#include <bmp280-driver.h>
#include <Var_Common/Var_Common.h>
#include <lib_clock_long.h>
#include "web_server_gest.h"

#define NO_BME280

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
// Create an Event Source on /events
AsyncEventSource events("/events");
// Json Variable to Hold Sensor Readings
JSONVar readings;


// Initialize SPIFFS
bool initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
    return false;
  }
  Serial.println("SPIFFS mounted successfully");
  return true;
}

// Get Sensor Readings and return JSON object
extern String getSensorReadings();
String getSensorReadings(){
  if (sensor_read.new_read){
    sensor_read.new_read = false;
    readings["pressure"] = String(sensor_read.press);
    readings["temperature"] = String(sensor_read.temp);
    readings["altitudine"] = String(sensor_read.alti);
  }
//  else {
//    readings["pressure"] = String(Lclk_to_sec(Main_Lclk));//String(789);
//    readings["temperature"] = String(Lclk_to_sec(Main_Lclk));//String(test_temperature);
//    readings["altitudine"] = String(Lclk_to_sec(Main_Lclk));
//  }
  /* converti clock in stringa*/
  char temp_char[50];
  char* p_temp_char_from = &temp_char[0];
  char* p_temp_char_to = &temp_char[25];
  *Lclk_to_char_no_inter(&Main_Lclk, temp_char)= 0;
  p_temp_char_from = &temp_char[0];
  p_temp_char_to = &temp_char[25];
  *p_temp_char_to++ = *p_temp_char_from++;
  *p_temp_char_to++ = *p_temp_char_from++;
  *p_temp_char_to++ = ':';
  *p_temp_char_to++ = *p_temp_char_from++;
  *p_temp_char_to++ = *p_temp_char_from++;
  *p_temp_char_to++ = ':';
  *p_temp_char_to++ = *p_temp_char_from++;
  *p_temp_char_to++ = *p_temp_char_from++;
  *p_temp_char_to++ = 0;
  readings["clock"] = String(&temp_char[25]);
  readings["my_txt"] = String("mio testo");
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

extern void web_server_start(void);
void web_server_start(void)
{
/***** start gestione web server *********************************************************/
  initSPIFFS();
  // Web Server Root URL
  server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.serveStatic("/", SPIFFS, "/");
  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    json = String();
  });
  events.onConnect([](AsyncEventSourceClient *client) {
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  // Start server
  server.begin();
/***** end gestione web server *********************************************************/
}

extern void web_server_run(void);
void web_server_run(void)
{
  //events.send("ping",NULL,millis());
  //Serial.println(getSensorReadings().c_str());
  events.send(getSensorReadings().c_str(),"new_readings" ,millis());
}


void web_server_restart(void)
{
  server.end();
  server.begin();
}