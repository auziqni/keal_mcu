#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>

// wifi
const char *ssid = "teknisee";
const char *password = "myteknisee";

// blynk
#define BLYNK_TEMPLATE_ID "TMPL6DMgr5bNR"
#define BLYNK_TEMPLATE_NAME "ini"
#define BLYNK_AUTH_TOKEN "D8A8JuYj8_7qC0ejGsEueT61967DCI-V"
#include <BlynkSimpleEsp32.h>
WidgetLCD lcd(V0);

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

bool blynk_auto = true;
bool blynk_addWater = false, blynk_mixA = false, blynk_mixB = false, blynk_stir = false;

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
        Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
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
    Serial.println('"' + line0 + '"');
    Serial.println('"' + line1 + '"');

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

void BlynkVirtualWrite()
{
    lcd.print(0, 0, line0);
    lcd.print(0, 1, line1);

    Blynk.virtualWrite(V1, volume);
    Blynk.virtualWrite(V2, ppm);
}
BLYNK_WRITE(V3)
{
    int val = param.asInt();
    val == 1 ? blynk_mixA = true : blynk_mixA = false;
}

BLYNK_WRITE(V4)
{
    int val = param.asInt();
    val == 1 ? blynk_mixB = true : blynk_mixB = false;
}
BLYNK_WRITE(V5)
{
    int val = param.asInt();
    val == 1 ? blynk_stir = true : blynk_stir = false;
}
BLYNK_WRITE(V6)
{
    int val = param.asInt();
    val == 1 ? blynk_addWater = true : blynk_addWater = false;
}
BLYNK_WRITE(V7)
{
    int val = param.asInt();
    val == 1 ? blynk_auto = true : blynk_auto = false;
}

void loop()
{
    Blynk.run();
    nowtime = millis();
    if (nowtime - lasttime > 1000)
    {
        lasttime = nowtime;
        Wificon();
        SerialPrint();
    }

    // volume = random(0, 100);
    // ppm = random(0, 1000);

    BlynkVirtualWrite();
}