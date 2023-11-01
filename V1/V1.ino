// Library includes
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <PMserial.h>
#include <OneWire.h>  
#include <DallasTemperature.h>  
#include <DFRobot_CCS811.h>  
#include <SoftwareSerial.h>
#include <ESP32Time.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <time.h>
#include <ESPAsyncWebSrv.h>
#include <SPIFFS.h>
#include <DNSServer.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define LED 2
#define WIFI_SSID "wifi-ssid"
#define WIFI_PASSWORD "wifi-password"

constexpr auto PMS_RX = 16;
constexpr auto PMS_TX = 17;
const int oneWireBus = 4;

ESP32Time rtc(-10800);  // GMT-3 offset in seconds

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
SerialPM pms(PMS3003, PMS_RX, PMS_TX);
DFRobot_CCS811 CCS811;

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;

uint8_t mac[6];
char macString[7];
String dataMessage;
File dataFile;

// Cria um AsyncWebServer na porta 80
// Essencial para uso da dashboard grafica local
AsyncWebServer server(80);

Adafruit_BME280 bme;
int rtcdia, rtcmes, rtcano, rtchora, rtcminuto, rtcsegundo{ 0 };
float temp, hum, pres, alt, temperatureC{ 0 };
int gpsUpdate = 0;

int pm1, pm2, pm10, co2, tvoc{ 0 };

void setup() {
  Serial.begin(9600);
  Serial.println(F("GreenLab Emission Tracker - Coded by Jairo Ivo"));

  pinMode(LED, OUTPUT);

  sensorBegin();
  initSDCard();
  checkSDFile();

  Serial.println(F("Initiating NTP synchronization for internal RTC!"));
  delay(500);
  synchronizeRTC();

  //Verifica e inicia a memoria flash local do esp que contem a dashboard gráfica
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

}

void loop() {
  if (rtc.getSecond() != rtcsegundo) {
    digitalWrite(LED, HIGH);
    displayDateTime();
    readBMEData();
    readPMSData();
    readCCSData();
    saveData();
  }
}

void synchronizeRTC() {
  while (rtc.getYear() < 2001) {
    Serial.print(F("."));
    rtcSyncWithNtp();
  }
}

void rtcSyncWithNtp() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)){
    rtc.setTimeStruct(timeinfo); 
  }
  delay(1000);
}

void displayDateTime() {
  Serial.print(F("- RTC date&time: "));
  Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));  // (String) returns time with specified format
  rtcmes = rtc.getMonth();
  rtcdia = rtc.getDay();
  rtcano = rtc.getYear();
  rtchora = rtc.getHour(true);
  rtcminuto = rtc.getMinute();
  rtcsegundo = rtc.getSecond();
}

void readBMEData() {
  if (bme.begin(0x76)) {
    temp = bme.readTemperature();
    hum = bme.readHumidity();
    pres = bme.readPressure();
    alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
  } else {
    temp = 0;
    hum = 0;
    pres = 0;
    alt = 0;
  }
}

void readPMSData() {

    pms.read();
    if (pms) {
      pm1 = (pms.pm01);
      pm2 = (pms.pm25);
      pm10 = (pms.pm10);
    } else {
      pm1 = 0;
      pm2 = 0;
      pm10 = 0;
    }
  

}

void readCCSData () {

    if (CCS811.checkDataReady() == true) {
      co2 = (CCS811.getCO2PPM());
      tvoc = (CCS811.getTVOCPPB());
      //CCS811.writeBaseLine(0x447B);
    } else {
      co2 = 0;
      tvoc = 0;
    }

}

// Inicia os sensores
void sensorBegin() {
  bme.begin(0x76);  //Sensor I2C Address
  Wire.setClock(100000);
  CCS811.begin();
  pms.init();
}

void sensorTest() {
  // Testa o DHT
  if (!bme.begin(0x76)) {
    delay(500);
    Serial.println("temp erro");

  } else {
    Serial.println("temp ok");
  }
  delay(500);

  // Testa o PMS
  pms.read();
  if (pms) {
    Serial.println("mp ok");
  } else {
    delay(500);
    Serial.println("mp erro");
  }
  delay(500);

  // Testa o sensor de TVOCs
  if (CCS811.checkDataReady() == true) {
    Serial.println("ccs ok");
  } else {
    delay(500);
    Serial.println("ccs erro");
  }
  delay(500);

  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  delay(500);
  if (temperatureC < 0) {
  } else {
    delay(500);
  }

}