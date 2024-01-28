#include <Arduino.h>
#include <WiFi.h>

// wifi
const char *ssid = "teknisee";
const char *password = "myteknisee";

// sensor
const int trig = 32; // define pin untrasonic
const int echo = 33;
const int tds = 36; // define pin tds

// aktuator
int relay_flow = 13;
int relay_mixA = 12;
int relay_mixB = 14;
int relay_stir = 27;

// define global variable
int nowtime, lasttime;
float jarak = 0, volume = 0;
int ppm = 0;
String line0 = "line 0", line1 = "line 1"; // max 16 char

bool blynk_auto = 0;
bool blynk_mixA = 0, blynk_mixB = 0, blynk_stir = 0;

void Wificon()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.print("Connecting to ");
        Serial.println(ssid);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println("");
        Serial.println("WiFi connected.");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        delay(5000);
    }
}

void setup()
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Wificon();

    pinMode(echo, INPUT);
    pinMode(trig, OUTPUT);
    pinMode(tds, INPUT);
    pinMode(relay_flow, OUTPUT);
    pinMode(relay_mixA, OUTPUT);
    pinMode(relay_mixB, OUTPUT);
    pinMode(relay_stir, OUTPUT);

    nowtime = millis();
    lasttime = nowtime;
}

void SerialPrint()
{
    Serial.println(line0);
    Serial.println(line1);

    Serial.print("Volume: ");
    Serial.print(volume);
    Serial.print(", Konsentrasi: ");
    Serial.println(ppm);

    Serial.print("MixA: ");
    Serial.print(blynk_mixA);
    Serial.print(", MixB: ");
    Serial.print(blynk_mixB);
    Serial.print(", Stir: ");
    Serial.println(blynk_stir);

    Serial.print("Auto: ");
    Serial.println(blynk_auto);

    Serial.println("");
}

void loop()
{
    nowtime = millis();
    if (nowtime - lasttime > 1000)
    {
        lasttime = nowtime;
        Wificon();
        SerialPrint();
    }
}