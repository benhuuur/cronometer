#include <Arduino.h>

// Define os pinos para os segmentos dos displays de 7 segmentos
byte unitSegment[] = {5, 6, 7, 8};
byte tenSegment[] = {9, 10, 11, 12};

// Variáveis para exibição do tempo em dígitos
int unitsDigit = 0;
int tensDigit = 0;
int startTime;

// Variáveis de estado para os botões
boolean isRunning = false;
boolean lastStartButtonState = false;
boolean lastStopButtonState = false;
boolean lastResetButtonState = false;

boolean currentStartButtonState, currentStopButtonState, currentResetButtonState;

// Define os pinos para os botões
#define START_BUTTON 2
#define STOP_BUTTON 3
#define RESET_BUTTON 4
#define RUNNING_LED 13

void setup()
{
    // Inicializa a comunicação serial e configura os pinos dos botões e LEDs
    Serial.begin(9600);
    pinMode(START_BUTTON, INPUT);
    pinMode(STOP_BUTTON, INPUT);
    pinMode(RESET_BUTTON, INPUT);
    pinMode(RUNNING_LED, OUTPUT);

    // Configura os pinos dos segmentos dos displays como saída
    for (int i = 0; i < 4; i++)
    {
        pinMode(unitSegment[i], OUTPUT);
        pinMode(tenSegment[i], OUTPUT);
    }
}

void loop()
{
    // Lê o estado atual dos botões
    currentStartButtonState = digitalRead(START_BUTTON);
    currentStopButtonState = digitalRead(STOP_BUTTON);
    currentResetButtonState = digitalRead(RESET_BUTTON);

    // Inicia o temporizador quando o botão de início é pressionado
    if (currentStartButtonState == HIGH && lastStartButtonState == LOW)
    {
        Serial.println("Started");
        isRunning = true;
        startTime = millis();
    }

    // Para o temporizador quando o botão de parada é pressionado
    if (currentStopButtonState == HIGH && lastStopButtonState == LOW)
        isRunning = false;

    // Reinicia o temporizador e os dígitos quando o botão de reinício é pressionado
    if (currentResetButtonState == HIGH && lastResetButtonState == LOW)
    {
        isRunning = false;
        unitsDigit = tensDigit = 0;
    }

    // Atualiza os dígitos se o temporizador estiver em execução
    if (isRunning)
    {
        digitalWrite(RUNNING_LED, HIGH);

        int currentTime = millis();
        int elapsedSeconds = (currentTime - startTime) / 1000;

        if (elapsedSeconds > 0)
        {
            unitsDigit += elapsedSeconds;

            if (tensDigit == 5 && unitsDigit == 9)
               unitsDigit = tensDigit = 0;

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

    // Exibe os dígitos nos displays de 7 segmentos
    binaryOutput(unitSegment, unitsDigit);
    binaryOutput(tenSegment, tensDigit);

    // Atualiza os estados dos botões
    lastResetButtonState = currentResetButtonState;
    lastStopButtonState = currentStopButtonState;
    lastStartButtonState = currentStartButtonState;
}

// Função para exibir um número binário nos displays de 7 segmentos
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
