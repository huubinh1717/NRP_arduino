/*
	This is a demo to test Grove - Sunlight Sensor library

*/

#include <Wire.h>
#include <Seeed_HM330X.h>
#include "Arduino.h"
#include "SI114X.h"

SI114X SI1145 = SI114X();
HM330X sensor;
uint8_t buf[30];
int logs[10], id = 0;


const char* str[] = { "sensor num: ", "PM1.0 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
					 "PM2.5 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
					 "PM10 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
					 "PM1.0 concentration(Atmospheric environment,unit:ug/m3): ",
					 "PM2.5 concentration(Atmospheric environment,unit:ug/m3): ",
					 "PM10 concentration(Atmospheric environment,unit:ug/m3): ",
};

HM330XErrorCode print_result(const char* str, uint16_t value) {
	if (NULL == str) {
		return ERROR_PARAM;
	}
	logs[id++] = value;
	return NO_ERROR;
}

/*parse buf with 29 uint8_t-data*/
HM330XErrorCode parse_result(uint8_t* data) {
	uint16_t value = 0;
	if (NULL == data) {
		return ERROR_PARAM;
	}
	for (int i = 1; i < 8; i++) {
		value = (uint16_t) data[i * 2] << 8 | data[i * 2 + 1];
		print_result(str[i - 1], value);

	}

	return NO_ERROR;
}

HM330XErrorCode parse_result_value(uint8_t* data) {
	if (NULL == data) {
		return ERROR_PARAM;
	}
	for (int i = 0; i < 28; i++) {
		Serial.print(data[i], HEX);
		Serial.print("  ");
		if ((0 == (i) % 5) || (0 == i)) {
			Serial.println("");
		}
	}
	uint8_t sum = 0;
	for (int i = 0; i < 28; i++) {
		sum += data[i];
	}
	if (sum != data[28]) {
		Serial.println("wrong checkSum!!");
	}
	Serial.println("");
	return NO_ERROR;
}

void setup() {

	Serial.begin(115200);
	delay(100);
	Serial.println("Beginning light and dust!");

	while (!SI1145.Begin()) {
		Serial.println("Si1145 is not ready!");
		delay(1000);
	}
	if (sensor.init()) {
		Serial.println("HM330X init failed!!");
		while (1);
	}
	Serial.println("Si1145 and HM330x is ready!");
}

void loop() {
	// Serial.print("//--------------------------------------//\r\n");
	// Serial.print("Vis: "); Serial.println(SI1145.ReadVisible());
	// Serial.print("IR: "); Serial.println(SI1145.ReadIR());
	// //the real UV value must be div 100 from the reg value , datasheet for more information.
	// Serial.print("UV: ");  Serial.println((float)SI1145.ReadUV() / 100);
	// delay(100);

	if (sensor.read_sensor_value(buf, 29)) {
		Serial.println("HM330X read result failed!!");
	}
	id = 0;
	// parse_result_value(buf);
	parse_result(buf);
	Serial.print(SI1145.ReadVisible());
	Serial.print(", ");
	Serial.print(SI1145.ReadIR());
	Serial.print(", ");
	Serial.print((float) SI1145.ReadUV() / 100);
	Serial.print(", ");
	Serial.print(logs[1]);
	Serial.print(", ");
	Serial.print(logs[2]);
	Serial.print(", ");
	Serial.print(logs[3]);
	Serial.print(", ");
	Serial.print(logs[4]);
	Serial.print(", ");
	Serial.print(logs[5]);
	Serial.print(", ");
	Serial.print(logs[6]);
	Serial.println("");

	delay(5000);
}
