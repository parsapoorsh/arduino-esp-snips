// some help by: https://randomnerdtutorials.com/guide-for-ds18b20-temperature-sensor-with-arduino/

OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature Sensors(&oneWire);
DeviceAddress device_address;

void DS18B20_begin() {
  Sensors.begin();

  Serial.print(F("Locating devices...\n"));
  // Number of temperature devices found
  uint8_t number_of_devices = Sensors.getDeviceCount();
  Serial.printf_P(PSTR("Found %u devices.\n"), number_of_devices);

  // Send the command to get temperatures
  Sensors.requestTemperatures();

  for (uint8_t i = 0; i < number_of_devices; i++) {
    if (Sensors.getAddress(device_address, i)) {
      Serial.printf_P(PSTR("Found devices %u with address: "), i);
      Serial.println(string_device_address(device_address));

      uint8_t resolution = Sensors.getResolution();
      float tempC = Sensors.getTempC(device_address);
      Serial.printf_P(PSTR("Devices %u with resolution %u temp C: %.4f\n"), i, resolution, tempC);
    } else {
      Serial.printf_P(PSTR("Found ghost device at %u but could not detect address. Check power and cabling\n"), i);
    }
  }
}

float get_tempC() {
  Sensors.requestTemperatures();
  return Sensors.getTempC(device_address);
}

char* string_device_address(DeviceAddress device_address) {
  static char addressStr[51];
  char* ptr = addressStr;

  *ptr++ = '{';
  *ptr++ = ' ';
  for (uint8_t i = 0; i < 8; i++) {
    ptr += sprintf(ptr, "0x%02X", device_address[i]);
    if (i != (8 - 1)) {
      *ptr++ = ',';
      *ptr++ = ' ';
    }
  }
  *ptr++ = ' ';
  *ptr++ = '}';
  *ptr = '\0';

  return addressStr;
}
