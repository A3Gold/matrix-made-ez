// Purpose: To display a score and message on an LED matrix.
// Reference: http://darcy.rsgc.on.ca/ACES/TEI3M/1920/Tasks.html#MatrixMadeEZ
// Author: A. Goldman
// Date: Dec. 14, 2019
// Status: Working

#include <EEPROM.h>         // Includes the EEPROM library.
#include <TimerOne.h>       // Includes the TimerOne library.
uint8_t goalButton = 7;     // Assigns pin 7 for the button.
uint8_t dimmer = 3;         // Assigns pin 3 for output enable.
uint8_t data = 11;          // Assigns pin 11 for data.
uint8_t latch = 12;         // Assigns pin 12 for latch.
uint8_t clk = 13;           // Assigns pin 13 for clock.
uint8_t score = 48;         // Creates variable for the score.
uint8_t formerState = 0;    // Creates variable for former button state.
uint8_t count = 0;          // Creates variable for counting.
uint8_t countSpeed = 7;     // Creates variable for speed of counting.
uint8_t brightness = 5;     // Creates variable for display brightness.
uint8_t message[] = "Goal"; // Creates variable for message.

void setup()
{
    pinMode(dimmer, OUTPUT);         // Assigns pin to be output.
    pinMode(data, OUTPUT);           // Assigns pin to be output.
    pinMode(latch, OUTPUT);          // Assigns pin to be output.
    pinMode(clk, OUTPUT);            // Assigns pin to be output.
    pinMode(goalButton, INPUT);      // Assigns pin to be output.
    analogWrite(dimmer, brightness); // Assigns pin to be output.
    Timer1.initialize(10000);        // Initializes timer; in microseconds.
    Timer1.attachInterrupt(countUp); // Function to call each period.
}

void countUp()
{            // Function called once each time period.
    count++; // Adds one to count.
}

void loop()
{
    if (count == countSpeed)
    {              // If count equal to countSpeed,
        count = 0; // set it to zero.
    }
    for (uint8_t i = 0; i < 8; i++)
    { // Displays score on matrix.
        shiftOut(data, clk, LSBFIRST, (EEPROM.read((score << 3) + i)));
        shiftOut(data, clk, LSBFIRST, 1 << i);
        digitalWrite(latch, LOW);
        digitalWrite(latch, HIGH);
    }
    if ((digitalRead(goalButton) == HIGH) && formerState == 0)
    {
        score++;         // Adds one to score if button pressed.
        formerState = 1; // Sets formerState to 1.
        for (uint8_t letter = 0; letter < sizeof(message); letter++)
        {
            for (uint8_t shift = 0; shift < 12; shift++)
            {
                while (count <= countSpeed)
                { // Shifts out scrolling message.
                    for (uint8_t row = 0; row < 8; row++)
                    {
                        shiftOut(data, clk, LSBFIRST, (EEPROM.read((message[letter] << 3) + row) >> shift));
                        shiftOut(data, clk, LSBFIRST, 1 << row);
                        digitalWrite(latch, LOW);
                        digitalWrite(latch, HIGH);
                    }
                }
                count = 0; // Sets count to zero.
                if ((digitalRead(goalButton) == LOW) && formerState == 1)
                {
                    formerState = 0; // Sets former state to 0 when released.
                }
            }
        }
    }
}
