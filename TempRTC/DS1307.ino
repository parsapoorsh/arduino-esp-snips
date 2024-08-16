#define DS1307_ADDRESS 0x68

RTC_DS1307 RTC;
TwoWire wire;

void DS1307_begin() {
  wire.begin(RTC_SDA, RTC_SCL);
  RTC.begin(&wire);

check_RTC:
  if (RTC.isrunning()) {
    Serial.println(F("RTC is running"));
    DateTime now = get_now();
    Serial.printf_P(PSTR("unixtime: %lu\n"), now.unixtime());

    Serial.printf_P(
      PSTR("datetime: %02hu/%02hhu/%02hhu %02hu:%02hu:%02hu\n"),
      now.year(), now.month(), now.day(),
      now.hour(), now.minute(), now.second());
  } else {
    Serial.println(F("RTC is NOT running!"));
    Serial.println(F("setting RTC time to compile time..."));
    // following line sets the RTC to the date & time this sketch was compiled
    DateTime compile_time = DateTime(F(__DATE__), F(__TIME__));
    // compile time influenced by system timezone, there is no __UNIXTIME__ macro
    uint32_t compile_unixtime = compile_time.unixtime() - TIMEZONE_IN_SECOND;
    RTC.adjust(compile_unixtime);
    goto check_RTC;
  }
}

bool ping_rtc() {
  wire.beginTransmission(DS1307_ADDRESS);
  return (wire.endTransmission() == 0);
}

uint32_t get_unixtime() {
  static uint32_t rtc_last_unixtime = 0;
  static uint64_t rtc_last_update_millis = 0;

  uint64_t local_millis_diff = millis64() - rtc_last_update_millis;

  if (
    rtc_last_update_millis != 0 &&
    // less power consumption
    // this way function is only using 0.001MHz of 80MHz CPU Clocks
    local_millis_diff < RTC_REQUEST_INTERVAL) {
estimate_unixtime:
    uint32_t local_seconds_diff = local_millis_diff / 1000;
    uint32_t estimated_unixtime = rtc_last_unixtime + local_seconds_diff;

    return estimated_unixtime;
  } else {
    // this way function is using 0.12MHz of 80MHz CPU Clocks
    DateTime rtc_now = RTC.now();
    uint64_t rtc_new_update_millis = millis64();
    uint32_t rtc_new_unixtime = rtc_now.unixtime();

    // Communication error
    if (!ping_rtc()) {
#ifdef DEBUGING
      Serial.println(F("Communication error with RTC"));
#endif
      goto estimate_unixtime;
    }

    rtc_last_update_millis = rtc_new_update_millis;
    rtc_last_unixtime = rtc_new_unixtime;

    return rtc_last_unixtime;
  }
}

DateTime get_now() {
  uint32_t unixtime = get_unixtime();

  // the datetime object from RTC when communication errors happens:
  // months > 12 and days > 31
  // this would recalculate it
  DateTime result_now = DateTime(unixtime + TIMEZONE_IN_SECOND);
  return result_now;
}
