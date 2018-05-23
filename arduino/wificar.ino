#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "Molly_2.4G";
const char* password = "qazwsx741";

WiFiUDP wUDP;

const int motoLeftForward = 13;
const int motoLeftBackward = 12;
const int motoRightForward = 16;
const int motoRightBackward = 14;

const int speedSensorLeft = 5;
const int speedSensorRight = 4;

const int disSensorTrig = 2;
const int disSensorEcho = 15;

int lSpeed, rSpeed;
#define MLEFT 0
#define MRIGHT 1

long timer;

int setMotoSpeed(bool moto, int iSpeed) {
  if (iSpeed >= 1024 || iSpeed <= -1024)
    return -1;
  int pinA, pinB;
  switch (moto) {
    case MLEFT:
      pinA = motoLeftForward;
      pinB = motoLeftBackward;
      lSpeed = iSpeed;
      break;
    case MRIGHT:
      pinA = motoRightForward;
      pinB = motoRightBackward;
      rSpeed = iSpeed;
      break;
  }
  if (iSpeed >= 0) {
    analogWrite(pinA, iSpeed);
    digitalWrite(pinB, LOW);
  } else {
    analogWrite(pinB, iSpeed);
    digitalWrite(pinA, LOW);
  }
  return 0;
}

double getMotoSpeed(bool moto) { // mm/s
  long lTime = micros();
  int pin;
  if (moto == MLEFT) pin = speedSensorLeft;
  else pin = speedSensorRight;
  while (digitalRead(pin) == HIGH) {
    if (micros() - lTime >= 100000)
      return 0;
  }
  while (digitalRead(pin) == LOW) {
    if (micros() - lTime >= 100000)
      return 0;
  }
  lTime = pulseIn(pin, HIGH);
  lTime += pulseIn(pin, LOW);
  return 10.84 / ((double)lTime / 1000000.0); //10.84: 69mm * pi / 20
}

IPAddress localIp(192,168,10,1), gateway(192,168,10,1), subnet(255,255,255,0);
void setup() {
  pinMode(motoLeftForward, OUTPUT);
  pinMode(motoLeftBackward, OUTPUT);
  pinMode(motoRightForward, OUTPUT);
  pinMode(motoRightBackward, OUTPUT);
  digitalWrite(motoLeftForward, LOW);
  digitalWrite(motoLeftBackward, LOW);
  digitalWrite(motoRightForward, LOW);
  digitalWrite(motoRightBackward, LOW);

  pinMode(speedSensorLeft, INPUT);
  pinMode(speedSensorRight, INPUT);
  pinMode(disSensorTrig, OUTPUT);
  pinMode(disSensorEcho, INPUT);

  Serial.begin(115200);
  Serial.println("\nConnecting WiFi.");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.softAPConfig(localIp, gateway, subnet);
  WiFi.softAP("ESP_CAR", "bakaFANCY520", 1, false);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.print(".");
    delay(2000);
  }

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  wUDP.begin(4210);
  timer = millis();
}
char buff[255], cmd[16], para1[16], para2[16];
bool bConnected = false;
IPAddress remoteIP;
int remotePort;
void loop() {
  ArduinoOTA.handle();
  int packetSize = wUDP.parsePacket();
  if (packetSize) {
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, wUDP.remoteIP().toString().c_str(), wUDP.remotePort());
    bConnected = true;
    remoteIP = wUDP.remoteIP();
    remotePort = wUDP.remotePort();
    int len = wUDP.read(buff, 255);
    if (len > 0)
    {
      buff[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", buff);
    wUDP.beginPacket(wUDP.remoteIP(), wUDP.remotePort());
    
    memset(cmd, 0, sizeof(cmd));
    memset(para1, 0, sizeof(para1));
    memset(para2, 0, sizeof(para2));
    int pos = 0, tmp = 0;
    while (buff[pos] != ' ' && buff[pos] != 0 && pos < 255) {
      cmd[pos] = buff[pos];
      pos++;
    }
    pos++;
    tmp = pos;
    while (buff[pos] != ' ' && buff[pos] != 0 && pos < 255) {
      para1[pos - tmp] = buff[pos];
      pos++;
    }
    pos++;
    tmp = pos;
    while (buff[pos] != ' ' && buff[pos] != 0 && pos < 255) {
      para2[pos - tmp] = buff[pos];
      pos++;
    }
    if (strcmp(cmd, "setspeed") == 0) {
      bool moto = strcmp(para1, "left") ? MLEFT : MRIGHT;
      int iSpeed;
      sscanf(para2, "%i", &iSpeed);
      setMotoSpeed(moto, iSpeed);
      wUDP.write("Received.");
    } else if (strcmp(cmd, "disconnect") == 0) {
      bConnected = false;
    } else if (strcmp(cmd, "isavailiable") == 0) {
      wUDP.write("Availiable");
    }
    wUDP.endPacket();
    memset(buff, 0, sizeof(buff));
  }
  if (millis() - timer >= 500) {
    if(bConnected) {
      wUDP.beginPacket(remoteIP, remotePort);
      double dSpeed = getMotoSpeed(MLEFT);
      sprintf(buff, "Leftspeed: %lf ", dSpeed);
      wUDP.write(buff);
      memset(buff, 0, sizeof(buff));
      dSpeed = getMotoSpeed(MRIGHT);
      sprintf(buff, "Rightspeed: %lf", dSpeed);
      wUDP.write(buff);
      wUDP.endPacket();
      memset(buff, 0, sizeof(buff));
    }
    timer = millis();
  }
}
