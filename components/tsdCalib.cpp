#include <Arduino.h>

// define pin
const int tds = 36;
float slope = 0.365, intercept = -22.686;

// define global variable
int nowtime, lasttime;
float ppm_value = 0, ppm_raw = 0;

void setup()
{
    Serial.begin(115200);
    pinMode(tds, INPUT);

    nowtime = millis();
}

// the loop function runs over and over again forever
void loop()
{
    // timer
    nowtime = millis();
    if (nowtime - lasttime > 1000)
    {
        lasttime = nowtime;
    }

    ppm_raw = analogRead(tds);
    ppm_value = (slope * ppm_raw) + intercept;
    if (ppm_value < 0)
    {
        ppm_value = 0;
    }

    Serial.print("ppm raw: ");
    Serial.print(ppm_raw);
    Serial.print(", ppm val: ");
    Serial.println(ppm_value);

    delay(300);
}
