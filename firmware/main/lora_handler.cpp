#include "lora_handler.h"

static SemaphoreHandle_t x_ubx_command_semaphore;

uint8_t *readLine2(uart_port_t uart, size_t *len)
{
    static uint8_t line[1024];
    size_t size = 0;

    while (1)
    {
        if (uart_read_bytes(uart, line + size, 1, portMAX_DELAY) == 1)
        {
            size++;

            uint8_t *p = line + size;
            if (*(p - 4) == 0x1A && *(p - 3) == 0xCF && *(p - 2) == 0xFC && *(p - 1) == 0x1D)
            {
                *len = size - 4;
                return line;
            }
        }
    }
}

void LoraHandler::LoraChannel(void *pvParameters)
{
    uart_config_t uart_config;

    uart_config.baud_rate = 9600;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.parity = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.rx_flow_ctrl_thresh = 120;

    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, GPIO_NUM_5, GPIO_NUM_4, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_1, 1024 * 2, 0, 0, NULL, 0);

    while (1)
    {
        size_t length = 0;
        uint8_t *buf = readLine2(UART_NUM_1, &length);
        ESP_LOGI(CONFIG_SN, "[LORA] Incoming Message (Size %d)\n", length);
/*
        Main__Shuttle *msg;
        msg = main__shuttle__unpack(NULL, length, buf);
        if (msg != NULL)
        {
            printf("    Sender: %s\n", msg->sender);
            printf("    Receiver: %s\n", msg->receiver);
            printf("    Timestamp: %" PRIu64 "\n", msg->timestamp);
            printf("    Flightstamp: %" PRIu64 "\n", msg->flightstamp);
            printf("    Type: %" PRIu32 "\n", msg->type);

            if (msg->type == 0)
            {
                FILE *f = fopen(msg->file->name, "w");
                if (f == NULL)
                {
                    printf("Error opening file!\n");
                }

                fprintf(f, "%s", msg->file->blob.data);
                fclose(f);

                printf("\n  File Packet:\n");
                printf("    Name: %s\n", msg->file->name);
                printf("    Size: %d\n", msg->file->blob.len);
            }

            if (msg->type == 1)
            {
                printf("\n  File Packet:\n");
                printf("    Name: %s\n", msg->file->name);
                printf("    Size: %d\n", msg->file->blob.len);
            }
        }

        main__shuttle__free_unpacked(msg, NULL);
*/
    }
}

LoraHandler::LoraHandler(Database *db)
{
    ESP_LOGI(CONFIG_SN, "[LORA] Service Stated...");
    x_ubx_command_semaphore = xSemaphoreCreateMutex();

    this->db = db;
    xTaskCreatePinnedToCore(LoraHandler::LoraChannel, "LoraChannel", 8192, db, 2, NULL, 1);
}
