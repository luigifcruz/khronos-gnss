#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"

#include "api_server.h"
#include "database.h"
#include "display.h"
#include "flash_storage.h"
#include "gps_handler.h"
#include "http_server.h"
#include "key_storage.h"
#include "lora_handler.h"
#include "mdns_responder.h"
#include "ntp_server.h"
#include "wireless.h"

static void LedNotifier(char *key, char *zone, void *value) {
  uint16_t *lvl = (uint16_t *)value;
  gpio_set_level(GPIO_NUM_2, (uint32_t)*lvl);
}

extern "C" {
void app_main();
}

void app_main() {
  static KeyStorage keys;
  static Database db(&keys);

  Display display(&db);

  FlashStorage flash;
  MdnsResponder mdns;
  Wireless wifi;

  HttpServer web;
  ApiServer ws(&db);
  NtpServer ntp;
  GpsHandler gps(&db);
  LoraHandler lora(&db);

  db.RegisterNotifier((char *)"led", LedNotifier);

  gpio_pad_select_gpio(GPIO_NUM_2);
  gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);

  ESP_LOGI(CONFIG_SN, "[MAIN] Loading state and settings...");
  db.LoadSettings();
  db.LoadState();
}
