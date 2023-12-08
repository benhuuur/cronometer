#include <Arduino.h>

// Define pins for the segments of the 7-segment displays
byte unitSegment[] = {5, 6, 7, 8};
byte tenSegment[] = {9, 10, 11, 12};

// Variables for displaying time as digits
int unitsDigit = 0;
int tensDigit = 0;
int startTime;

// State variables for buttons
boolean isRunning = false;
boolean lastStartButtonState = false;
boolean lastStopButtonState = false;
boolean lastResetButtonState = false;

boolean currentStartButtonState, currentStopButtonState, currentResetButtonState;

// Define pins for buttons
#define START_BUTTON 2
#define STOP_BUTTON 3
#define RESET_BUTTON 4
#define RUNNING_LED 13

void setup()
{
    // Initialize serial communication and configure button and LED pins
    Serial.begin(9600);
    pinMode(START_BUTTON, INPUT);
    pinMode(STOP_BUTTON, INPUT);
    pinMode(RESET_BUTTON, INPUT);
    pinMode(RUNNING_LED, OUTPUT);

    // Configure pins for segments of the 7-segment displays as output
    for (int i = 0; i < 4; i++)
    {
        pinMode(unitSegment[i], OUTPUT);
        pinMode(tenSegment[i], OUTPUT);
    }
}

void loop()
{
    // Read the current state of the buttons
    currentStartButtonState = digitalRead(START_BUTTON);
    currentStopButtonState = digitalRead(STOP_BUTTON);
    currentResetButtonState = digitalRead(RESET_BUTTON);

    // Start the timer when the start button is pressed
    if (currentStartButtonState == HIGH && lastStartButtonState == LOW)
    {
        Serial.println("Started");
        isRunning = true;
        startTime = millis();
    }

    // Stop the timer when the stop button is pressed
    if (currentStopButtonState == HIGH && lastStopButtonState == LOW)
        isRunning = false;

    // Reset the timer and digits when the reset button is pressed
    if (currentResetButtonState == HIGH && lastResetButtonState == LOW)
    {
        isRunning = false;
        unitsDigit = tensDigit = 0;
    }

    // Update the digits if the timer is running
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

    // Display the digits on the 7-segment displays
    binaryOutput(unitSegment, unitsDigit);
    binaryOutput(tenSegment, tensDigit);

    // Update the states of the buttons
    lastResetButtonState = currentResetButtonState;
    lastStopButtonState = currentStopButtonState;
    lastStartButtonState = currentStartButtonState;
}

// Function to display a binary number on the 7-segment displays
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
