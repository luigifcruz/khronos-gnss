#include "soc/rtc_cntl_reg.h"
#include "soc/rtc_io_reg.h"
#include "soc/soc_ulp.h"

  .bss

  .global checkmark
checkmark:
  .long 0

  .text
  .global entry
entry:
  WRITE_RTC_REG(RTC_GPIO_OUT_W1TS_REG, RTC_GPIO_OUT_DATA_W1TS_S+12, 1, 1)
  jump checkin

pps_loop:
  READ_RTC_REG(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT_S+10, 1)
  jumpr enable_mirror, 1, lt
  jump pps_loop

enable_mirror:
  WRITE_RTC_REG(RTC_GPIO_OUT_W1TS_REG, RTC_GPIO_OUT_DATA_W1TS_S+12, 1, 1)
  jump pps_loop

checkin:
  /* Check Checkmark */
  move r3, checkmark
  ld r2, r3, 0
	add r2, r2, 1
  st r2, r3, 0

  /* Start PPS Loop */
  jump pps_loop