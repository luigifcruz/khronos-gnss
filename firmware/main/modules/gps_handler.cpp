#include "gps_handler.h"

GpsHandler::GpsHandler() {
	ESP_LOGI(CONFIG_SN, "[GPS] Service Stated...");

	char *line = "$GPZDA,160012.71,11,03,2004,-1,00*7D";
    printf("Result: %d\n", minmea_sentence_id(line, false));

    struct minmea_sentence_zda frame;
    if (minmea_parse_zda(&frame, line)) {
        printf("$xxZDA: %d:%d:%d %02d.%02d.%d UTC%+03d:%02d\n",
               frame.time.hours,
               frame.time.minutes,
               frame.time.seconds,
               frame.date.day,
               frame.date.month,
               frame.date.year,
               frame.hour_offset,
               frame.minute_offset);
	}
}