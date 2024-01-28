#include <Arduino.h>

// define pin
const int tds = 36;

// define global variable
int nowtime, lasttime;
float ppm = 0;

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

    ppm = analogRead(tds);
    Serial.print("tds: ");
    Serial.println(ppm);
}
