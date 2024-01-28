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
int nowtime, lasttime, minutecounting = 0;
float jarak = 0, volume = 0;
int ppm = 0;
int timeSTeering = 0;
String line0 = "line 0", line1 = "line 1"; // max 16 char

bool blynk_auto = true;
bool blynk_addWater = false, blynk_mixA = false, blynk_mixB = false, blynk_stir = false;

// konstanta
float slope = 0.365, intercept = -22.686;
const int boxLenght = 60, boxWidth = 40, boxHeight = 50;                // panjang lebar dalam cm // tmax =50 vmax=120000cm3=120l
const int maxVolume = (boxLenght * boxWidth * boxHeight) / 1000;        // dalam l
const int minLevelAir = maxVolume / 2, maxLevelAir = maxVolume * 5 / 6; // dalam liter
const int ppmMin = 600, ppmMax = 800;

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

void PrintConstant()
{
  Serial.print("boxLenght: ");
  Serial.println(boxLenght);
  Serial.print("boxWidth: ");
  Serial.println(boxWidth);
  Serial.print("boxHeight: ");
  Serial.println(boxHeight);
  Serial.print("maxVolume: ");
  Serial.println(maxVolume);
  Serial.print("minLevelAir: ");
  Serial.println(minLevelAir);
  Serial.print("maxLevelAir: ");
  Serial.println(maxLevelAir);
  delay(1000);
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

  PrintConstant();
  nowtime = millis();
  lasttime = nowtime;
}

void ReadSensor()
{
  // tds
  int ppm_raw = analogRead(tds);
  ppm = (slope * ppm_raw) + intercept;
  if (ppm < 0)
  {
    ppm = 0;
  }
  delay(50);

  // ultrasonic
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  jarak = pulseIn(echo, HIGH) * 0.034 / 2;

  if (jarak < 50)
  {

    volume = (boxLenght * boxWidth * (boxHeight - jarak)) / 1000;
  }
  else
  {
    volume = 120;
  }
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

void ActuateAuto()
{
  if (volume < minLevelAir) // ketika air di bawah level
  {
    digitalWrite(relay_flow, HIGH);
    timeSTeering = 5;
    line0 = "Add Water";
  }
  else
  {
    digitalWrite(relay_flow, LOW);
    line0 = "Water OK";
  }

  if (ppm < ppmMin && volume > minLevelAir && timeSTeering == 0) // tambah ppm
  {
    digitalWrite(relay_mixA, HIGH);
    digitalWrite(relay_mixB, HIGH);
    timeSTeering = 5;
    delay(5000);
  }
  else
  {
    digitalWrite(relay_mixA, LOW);
    digitalWrite(relay_mixB, LOW);
  }

  if (ppm > ppmMax && volume < maxLevelAir && timeSTeering == 0) // ketika ppm lebih dari batas tambah air
  {
    digitalWrite(relay_flow, HIGH);
    timeSTeering = 5;
  }

  if (volume > maxVolume)
  {
    digitalWrite(relay_flow, LOW);
  }

  if (timeSTeering > 0) // activate steering
  {
    digitalWrite(relay_stir, HIGH);
  }
  else
  {
    digitalWrite(relay_stir, LOW);
  }
}

void ActuateManual()
{
  if (blynk_addWater)
  {
    digitalWrite(relay_flow, HIGH);
  }
  else
  {
    digitalWrite(relay_flow, LOW);
  }

  if (blynk_mixA)
  {
    digitalWrite(relay_mixA, HIGH);
  }
  else
  {
    digitalWrite(relay_mixA, LOW);
  }

  if (blynk_mixB)
  {
    digitalWrite(relay_mixB, HIGH);
  }
  else
  {
    digitalWrite(relay_mixB, LOW);
  }

  if (blynk_stir)
  {
    digitalWrite(relay_stir, HIGH);
  }
  else
  {
    digitalWrite(relay_stir, LOW);
  }
}

void SerialPrint()
{
  Serial.println('"' + line0 + '"');
  Serial.println('"' + line1 + '"');

  Serial.print("Volume: ");
  Serial.print(volume);
  Serial.print(", Jarak: ");
  Serial.print(jarak);
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
  Blynk.run();
  nowtime = millis();
  if (nowtime - lasttime > 1000)
  {
    lasttime = nowtime;
    Wificon();
    ReadSensor();
    SerialPrint();

    minutecounting++;

    if (timeSTeering > 0)
    {
      timeSTeering--;
      line1 = "Steering: " + String(timeSTeering);
    }

    if (minutecounting == 60)
    {
      Blynk.virtualWrite(V7, 1); // make sure auto is on // do every 1 minutes
      blynk_auto = true;

      minutecounting = 0;
    }
  }

  BlynkVirtualWrite();
  blynk_auto ? ActuateAuto() : ActuateManual();
}