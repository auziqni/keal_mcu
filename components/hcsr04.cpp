#include <Arduino.h>

// define pin
const int trig = 32;
const int echo = 33;

// define global variable
int nowtime, lasttime;
long duration = 0;
float distance = 0;

void setup()
{
    Serial.begin(115200);
    pinMode(echo, INPUT);
    pinMode(trig, OUTPUT);

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
        Serial.print("Distance: ");
        Serial.println(distance);
    }

    digitalWrite(trig, LOW);
    delay(2);
    digitalWrite(trig, HIGH);
    delay(10);
    digitalWrite(trig, LOW);

    duration = pulseIn(echo, HIGH);
    distance = duration * 0.034 / 2;
}
