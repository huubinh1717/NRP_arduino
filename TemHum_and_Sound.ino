/*
    Demo name   : TH02_dev demo
    Usage       : DIGITAL I2C HUMIDITY AND TEMPERATURE SENSOR
    Author      : Oliver Wang from Seeed Studio
    Version     : V0.1
*/

#include <TH02_dev.h>
#include "Arduino.h"
#include "Wire.h"
#include "THSensor_base.h"

#define ShowSerial Serial
#ifdef __AVR__
    #include <SoftwareSerial.h>
    SoftwareSerial SSerial(2, 3); // RX, TX
    #define COMSerial Serial
    #define ShowSerial Serial
    TH02_dev TH02;
#endif

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define COMSerial Serial1
    #define ShowSerial SerialUSB
    TH02_dev TH02;
#endif

#ifdef ARDUINO_ARCH_STM32F4
    #define COMSerial Serial
    #define ShowSerial SerialUSB
    TH02_dev TH02;
#endif

const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

void setup() {
    ShowSerial.begin(115200);        // start serial for output

    ShowSerial.println("**** TH02 init ****");
    /* Power up,delay 150ms,until voltage is stable */
    delay(150);
    /* Reset HP20x_dev */
    TH02.begin();
    delay(100);

    /* Determine TH02_dev is available or not */
    ShowSerial.println("TH02_dev is available.\n");
}

void loop() {
    sensorValue = analogRead(analogInPin);        // read the analog in value
    outputValue = map(sensorValue, 0, 1023, 0, 255);       // map it to the range of the analog out
    analogWrite(analogOutPin, outputValue);               // change the analog out value

    float temper = TH02.ReadTemperature();
    float humidity = TH02.ReadHumidity();

    ShowSerial.print(temper);
    ShowSerial.print(", ");
    ShowSerial.print(humidity);
    ShowSerial.print(", ");
    ShowSerial.print(outputValue);
    ShowSerial.println("");
    delay(5000);
}
