#include <FrequencyTimer2.h>

#define BYTE_LENGTH 8

const int latchPin = 5; //Pin connected to Pin 12 of 74HC595 (Latch)
const int clockPin = 6; //Pin connected to Pin 11 of 74HC595 (Clock)
const int dataPin = 4; //Pin connected to Pin 14 of 74HC595 (Data)
const int enablePin = 7; //Pin connected to Pin 13 of 74HC595 (OutputEnable)

// making a timer without using delay()
unsigned long startTime;
unsigned long displayStartTime;
unsigned long currentTime;
byte i = 3;
bool growing = false;

byte rPos = 0;
byte cPos = 0;

// Describe each digit in terms of display segments
const byte zeros = 0b00000000;
const byte ones = 0b11111111;

const byte rows =     0b00000001;
const byte columns =  0b00000010;

byte image [8] = {B00000000,
                  B01101100,
                  B11111110,
                  B11111110,
                  B01111100,
                  B00111000,
                  B00010000,
                  B00000000};

void setup()
{
  //set pins to output 
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  displayStartTime = startTime = millis();
  shiftMatrix(zeros, zeros);
}

void loop()
{
  currentTime = millis();
  if (currentTime - startTime > 500)
  {
    shiftImage(image);     
    startTime = currentTime; // reset startTime
  }

  currentTime = millis();
  if (currentTime - displayStartTime > 10)
  {
    draw(image);
    displayStartTime = currentTime; // reset startTime
  }
}

void draw(byte *image)
{
  int row;
  for (row = 0; row < 8; row++)
  {
    shiftMatrix(1 << row, image[7-row]);
    delay(2);
  } 
}

void shiftImage(byte *image)
{
  for (int row = 0; row < 8; row++)
  {
    image[row] = (image[row] << 1) | (image[row] >> (sizeof(image[row])*BYTE_LENGTH - 1));
  } 
}

// shift the data, eg. 0b10111110 (6) into the registor
void shiftMatrix (byte rowData, byte columnData)
{
  digitalWrite(enablePin, HIGH);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, columnData);
  shiftOut(dataPin, clockPin, LSBFIRST, ~rowData);
  digitalWrite(latchPin, HIGH);
  digitalWrite(enablePin, LOW);
}
