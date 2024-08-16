#include <Wire.h>
#include <RTClib.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define DEBUGING
#define RTC_REQUEST_INTERVAL 5000
#define TIMEZONE_IN_SECOND 12600  // +3:30 Asia/Tehran

#define ONE_WIRE_BUS_PIN D8
#define RTC_SDA D14
#define RTC_SCL D15

void setup() {
  Serial.begin(115200);
  Serial.println();

  DS18B20_begin();
  delay(1000);  // fixes no devices identified
  DS1307_begin();

  Serial.println(F("Ready."));
}

void loop() {
  delay(1000);
  DateTime now = get_now();
  float temp = get_tempC();

  Serial.printf_P(PSTR("unixtime: %lu"), now.unixtime());
  Serial.print(F(", "));

  Serial.printf_P(
    PSTR("date: %02hu/%02hhu/%02hhu"),
    now.year(), now.month(), now.day());
  Serial.print(F(", "));

  Serial.printf_P(
    PSTR("time: %02hu:%02hu:%02hu"),
    now.hour(), now.minute(), now.second());
  Serial.print(F(", "));

  Serial.printf_P(PSTR("temp: %.4f"), temp);

  Serial.println();
}
