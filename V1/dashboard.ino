//Strings que geram os dados para a dashboard local
String readPM1() {
  float dashpm1;
  return String(dashpm1);
}

String readPM25() {
  float dashpm25;
  return String(dashpm25);
}

String readPM10() {
  float dashpm10;
  return String(dashpm10);
}

String readCO2() {
  float dashco2;
  return String(dashco2);
}

String readTVOCS() {
  float dashtvocs;
  return String(dashtvocs);
}

String readBMETemperature() {
  float t;
  return String(t);
}

String readBMEHumidity() {
  float h;
  return String(h);
}

String readBMEPress() {
  float p;
  return String(p);
}

void serverUpdate(){

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  
  server.on("/pm1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPM1().c_str());
  });
  
  server.on("/pm25", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPM25().c_str());
  });
  
  server.on("/pm10", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPM10().c_str());
  });

  server.on("/co2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readCO2().c_str());
  });

  server.on("/tvocs", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTVOCS().c_str());
  });

  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readBMETemperature().c_str());
  });
  
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readBMEHumidity().c_str());
  });

  server.on("/press", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readBMEPress().c_str());
  });

   server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  server.begin();

}