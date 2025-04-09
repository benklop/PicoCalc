#include "power.h"

/*
| Bit    | Name             | Description                                                        |
| ------ |:----------------:| ------------------------------------------------------------------:|
| 7      | SHUTDOWN_NOW     | Shutdown now. If set, the PMU will shutdown immediately.           |
| 6      | OS_POWER_HANDLE  | Let the OS handle power                                                        |
| 5      | PWROFF_RESTART   | If set, the PMU will restart. If not set, the PMU will shutdown.   |
| 4      | LONG_PRESS_EN    | If set, the power key long press shutdown is enabled               |
| 3      | XPOWERS_POWERON  | Bit 1 Power on delay. 0 = 128ms, 1 = 512ms, 2 = 1s, 3 = 2s.        |
| 2      | XPOWERS_POWERON  | Bit 0 Power on delay. 0 = 128ms, 1 = 512ms, 2 = 1s, 3 = 2s.        |
| 1      | XPOWERS_POWEROFF | Bit 1 Power off delay. 0 = 4s, 1 = 6s, 2 = 8s, 3 = 10s.            |
| 0      | XPOWERS_POWEROFF | Bit 0 Power off delay. 0 = 4s, 1 = 6s, 2 = 8s, 3 = 10s.            |
 */

void power_key_update_reg() {
  // Bit 7 is shutdown now
  bool shutdown_now = reg_is_bit_set(REG_ID_POWER, 7);
  // If shutdown_now is set, the PMU will shutdown immediately
  if (shutdown_now) {
    shutdown_now();

    return;
  }

  // bits 0, 1 are the power off delay
  // 0 = XPOWERS_POWEROFF_4S
  // 1 = XPOWERS_POWEROFF_6S
  // 2 = XPOWERS_POWEROFF_8S
  // 3 = XPOWERS_POWEROFF_10S
  uint8_t power_off_delay = reg_get_bits(REG_ID_POWER, 0, 2);
  //set the power off delay
  PMU.setPowerKeyPressOffTime(power_off_delay);

  // bits 2, 3 are the power on delay
  // 0 = XPOWERS_POWERON_128MS
  // 1 = XPOWERS_POWERON_512MS
  // 2 = XPOWERS_POWERON_1S
  // 3 = XPOWERS_POWERON_2S
  uint8_t power_on_delay = reg_get_bits(REG_ID_POWER, 2, 2);
  //set the power on delay
  PMU.setPowerKeyPressOnTime(power_on_delay);

  // bit 4 sets the long press shutdown behavior.
  bool long_en = reg_is_bit_set(REG_ID_POWER, 4);
  if (long_en) {
    // Enable long press shutdown
    PMU.enableLongPressShutdown();
  } else {
    // Disable long press shutdown
    PMU.disableLongPressShutdown();
  }

  // bit 5 sets the power off restart behavior.
  bool restart = reg_is_bit_set(REG_ID_POWER, 5);
  if (restart) {
    // Enable power off restart
    PMU.setLongPressRestart();
  } else {
    // Disable power off restart
    PMU.setLongPressPowerOFF();
  }
}

void shutdown_now() {
  digitalWrite(PA13, LOW);
  digitalWrite(PA14, LOW);
  PMU.setChargingLedMode(XPOWERS_CHG_LED_CTRL_CHG);
  PMU.shutdown();
}

// no idea if this is correct yet.
void sleep_now() {
  //PMU.setChargingLedMode(XPOWERS_CHG_LED_CTRL_CHG);
  //PMU.sleep();
}
