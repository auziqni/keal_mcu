#include <Arduino.h>

int relay_flow = 13;
int relay_mixA = 12;
int relay_mixB = 14;
int relay_stir = 27;

// define global variable
int nowtime, lasttime;
long duration = 0;

void setup()
{
    Serial.begin(115200);
    pinMode(relay_flow, OUTPUT);
    pinMode(relay_mixA, OUTPUT);
    pinMode(relay_mixB, OUTPUT);
    pinMode(relay_stir, OUTPUT);

    nowtime = millis();
}

void loop()
{
    // timer
    nowtime = millis();
    if (nowtime - lasttime > 1000)
    {
        lasttime = nowtime;
    }

    digitalWrite(relay_flow, LOW);
    digitalWrite(relay_mixA, HIGH);
    digitalWrite(relay_mixB, HIGH);
    digitalWrite(relay_stir, HIGH);
    delay(1000);

    digitalWrite(relay_flow, HIGH);
    digitalWrite(relay_mixA, LOW);
    digitalWrite(relay_mixB, LOW);
    digitalWrite(relay_stir, LOW);
    delay(1000);
}
