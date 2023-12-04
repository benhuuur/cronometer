#include <Arduino.h>
byte unitSegment[] = {5, 6, 7, 8};
byte tenSegment[] = {9, 10, 11, 12};

// Variables for displaying time as digits
int unitsDigit = 0;
int tensDigit = 0;
int startTime;

// int debounce = 50;

// State variables for buttons
boolean isRunning = false;
boolean lastStartState = false;
boolean lastStopState = false;
boolean lastZeroState = false;

boolean currentStartState, currentStopState, currentZeroState;

// Button pin definitions
#define BTN_START 2
#define BTN_STOP 3
#define BTN_ZERO 4
#define RUNNING_LED 13

void setup()
{
    Serial.begin(9600);
    pinMode(BTN_START, INPUT);
    pinMode(BTN_STOP, INPUT);
    pinMode(BTN_ZERO, INPUT);
    pinMode(RUNNING_LED, OUTPUT);

    for (int i = 0; i < 4; i++)
    {
        pinMode(unitSegment[i], OUTPUT);
        pinMode(tenSegment[i], OUTPUT);
    }
}

void loop()
{
    currentStartState = digitalRead(BTN_START);
    currentStopState = digitalRead(BTN_STOP);
    currentZeroState = digitalRead(BTN_ZERO);

    if (currentStartState == HIGH && lastStartState == LOW)
    {
        isRunning = !isRunning;
        startTime = millis();
    }

    if (currentStopState == HIGH && lastStopState == LOW)
        isRunning = false;

    if (currentZeroState == HIGH && lastZeroState == LOW)
    {
        isRunning = false;
        unitsDigit = tensDigit = 0;
    }

    if (isRunning)
    {
        digitalWrite(RUNNING_LED, HIGH);

        int currentTime = millis();
        int elapsedSeconds = (currentTime - startTime) / 1000;

        if (elapsedSeconds > 0)
        {
            unitsDigit += elapsedSeconds;

            if (unitsDigit > 9)
            {
                tensDigit += unitsDigit % 9;
                unitsDigit = 0;
            }

            startTime = currentTime;
        }
    }
    else
        digitalWrite(RUNNING_LED, LOW);

    Serial.print(unitsDigit);
    Serial.print(tensDigit);
    binaryOutput(unitSegment, unitsDigit);
    binaryOutput(tenSegment, tensDigit);

    lastZeroState = lastZeroState;
    lastStopState = lastStopState;
    lastStartState = lastStartState;
}

void binaryOutput(byte output[], int number)
{
    if (number % 2 == 0)
        digitalWrite(output[0], LOW);
    else
        digitalWrite(output[0], HIGH);

    if (number % 4 > 1)
        digitalWrite(output[1], HIGH);
    else
        digitalWrite(output[1], LOW);
    if (number % 8 > 3)
        digitalWrite(output[2], HIGH);
    else
        digitalWrite(output[2], LOW);

    if (number % 16 > 7)
        digitalWrite(output[3], HIGH);
    else
        digitalWrite(output[3], LOW);
}
